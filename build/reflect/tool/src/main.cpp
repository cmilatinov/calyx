#include "templates/Comment.h"
#include "templates/Registration.h"

#include "serializers/HeaderSerializer.h"

bool CheckArgs(cxxopts::ParseResult* args) {
    if (!args->count("file")) {
        std::cerr << "Missing input header file!" << std::endl;
        return false;
    }

    if (!args->count("root")) {
        std::cerr << "Missing root include directory!" << std::endl;
        return false;
    }

    if (!args->count("output")) {
        std::cerr << "Missing output directory!" << std::endl;
        return false;
    }

    return true;
}

void ParseArgs(int argc, const char** argv, cxxopts::ParseResult* result) {
    cxxopts::Options options("reflect", "Generates reflection data for C++ header files.");
    options.add_options()
        (
            "h,help",
            "Show this help dialog."
        )
        (
            "r,root",
            "The root include directory relative to which to make the #include directives.",
            cxxopts::value<std::string>()
        )
        (
            "o,output",
            "The output directory into which the reflection files should be generated.",
            cxxopts::value<std::string>()
        )
        (
            "args",
            "The compiler arguments to use when parsing the given header.",
            cxxopts::value<std::vector<std::string>>()
        )
        (
            "file",
            "The header file for which to generate the metadata.",
            cxxopts::value<std::string>()
        );

    // Parse command-line arguments
    options.parse_positional("file");
    *result = options.parse(argc, argv);

    // Print help dialog
    if (result->count("help")) {
        std::cout << options.help() << std::endl;
        exit(0);
    }

    // Check we have all arguments
    if (!CheckArgs(result)) {
        exit(1);
    }
}

int main(int argc, const char** argv) {
    using namespace Calyx::Reflect::Tooling;

    // Parse command-line arguments
    cxxopts::ParseResult cmdArgs;
    ParseArgs(argc, argv, &cmdArgs);

    // Construct argument list for clang tool
    auto argList = cmdArgs["args"].as<std::vector<std::string>>();
    std::vector<const char*> args = { argv[0], cmdArgs["file"].as<std::string>().c_str(), "--" };
    std::transform(
        argList.begin(), argList.end(),
        std::back_inserter(args),
        [](const auto& str) { return str.c_str(); }
    );

    // Parse clang and LLVM options
    cl::OptionCategory category("Options");
    int nargs = args.size();
    auto optionsParser = CommonOptionsParser::create(nargs, args.data(), category);
    if (!optionsParser) {
        llvm::errs() << optionsParser.takeError() << "\n";
        return 1;
    }

    // Create clang tool
    auto inputHeader = optionsParser->getSourcePathList()[0];
    std::vector<std::string> sourcePaths = { inputHeader };
    ClangTool tool(optionsParser->getCompilations(), sourcePaths);

    // Create matcher
    HeaderSerializer serializer(inputHeader);
    MatchFinder finder;
    DeclarationMatcher classMatcher =
        cxxRecordDecl(
            has(
                friendDecl(
                    has(
                        functionDecl(
                            hasName(CX_XSTR(CX_REFLECT_REGISTRATION_FN_NAME)))))))
            .bind("classes");
    finder.addMatcher(classMatcher, &serializer);

    // Run tool
    auto start = ch::high_resolution_clock::now();
    tool.run(newFrontendActionFactory(&finder).get());

    // Create needed directories
    auto includeRoot = cmdArgs["root"].as<std::string>();
    auto outputDir = cmdArgs["output"].as<std::string>();
    auto relativeHeaderPath = fs::relative(fs::path(inputHeader), fs::path(includeRoot));
    auto relativeHeaderDir = relativeHeaderPath.parent_path();
    auto outDirPath = fs::path(outputDir) / relativeHeaderDir;
    auto outFilePath = outDirPath / relativeHeaderPath.stem();
    fs::create_directories(outDirPath);

    // Load templates
    inja::Environment env;
    inja::Template commentTemplate = env.parse(TEMPLATE_COMMENT);
    env.include_template("comment", commentTemplate);
    inja::Template registrationTemplate = env.parse(TEMPLATE_REGISTRATION);

    // Render generated source
    json data = json::object(
        {
            { "header", relativeHeaderPath.string() },
            { "classes", serializer.GetJSONObject() }
        }
    );
    std::ofstream sourceFile(outFilePath.string() + ".gen.cpp");
    env.render_to(sourceFile, registrationTemplate, data);
    sourceFile.close();

    // Time elapsed
    auto end = ch::high_resolution_clock::now();
    std::cout << ch::duration_cast<ch::milliseconds>(end - start).count() << " ms" << std::endl;

    return 0;
}