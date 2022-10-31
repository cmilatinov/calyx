#include "serializers/HeaderSerializer.h"
#include "templates/Comment.h"
#include "templates/Registration.h"

using namespace Calyx::Reflect::Tooling;

static cl::opt<std::string> IncludeRoot(
    "include-root",
    cl::desc("Specify the directory which to use as the root include directory"),
    cl::value_desc("directory")
);

static cl::opt<std::string> OutputDir(
    "output-dir",
    cl::desc("Specify the directory in which the resulting reflection files will be generated"),
    cl::value_desc("directory")
);

int main(int argc, const char** argv) {
    // Get source paths and create clang tool
    cl::OptionCategory cat("Parsing Options", "");
    auto optionsParser = CommonOptionsParser::create(argc, argv, cat);
    auto inputHeader = optionsParser->getSourcePathList()[0];
    std::vector<std::string> sourcePaths = { inputHeader };
    auto& compilations = optionsParser->getCompilations();
    ClangTool tool(compilations, sourcePaths);

    // Create matcher
    HeaderSerializer serializer(inputHeader);
    MatchFinder finder;
    DeclarationMatcher classMatcher =
        cxxRecordDecl(
            has(
                friendDecl(
                    has(
                        functionDecl(
                            hasName("reflect_auto_register_reflection_function_"))))))
            .bind("classes");
    finder.addMatcher(classMatcher, &serializer);

    // Run tool
    auto start = ch::high_resolution_clock::now();
    tool.run(newFrontendActionFactory(&finder).get());

    // Create needed directories
    auto includeRoot = IncludeRoot.getValue();
    auto outputDir = OutputDir.getValue();
    auto relativeHeaderPath = fs::relative(fs::path(inputHeader), fs::path(includeRoot));
    auto relativeHeaderDir = relativeHeaderPath.parent_path();
    auto outDirPath = fs::path(OutputDir.getValue()) / relativeHeaderDir;
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
    std::cerr << ch::duration_cast<ch::milliseconds>(end - start).count() << " ms" << std::endl;

    return 0;
}