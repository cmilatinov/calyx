#include "serializers/HeaderSerializer.h"

using namespace reflect::tooling;

static cl::opt<std::string> IncludeRoot("include-root",
                                        cl::desc("Specify the directory which to use as the root include directory"),
                                        cl::value_desc("directory"));

static cl::opt<std::string> OutputDir("output-dir",
                                      cl::desc(
                                          "Specify the directory in which the resulting reflection files will be generated"),
                                      cl::value_desc("directory"));

int main(int argc, const char** argv) {
    cl::OptionCategory cat("Parsing Options", "");
    auto optionsParser = CommonOptionsParser::create(argc, argv, cat);
    auto inputHeader = optionsParser->getSourcePathList()[0];
    std::vector<std::string> sourcePaths = { inputHeader };
    auto& compilations = optionsParser->getCompilations();
    ClangTool tool(compilations, sourcePaths);

    HeaderSerializer serializer;
    MatchFinder finder;
    DeclarationMatcher classMatcher =
        cxxRecordDecl(
            has(
                friendDecl(
                    has(
                        classTemplateDecl(
                            has(
                                cxxRecordDecl(
                                    hasName("Class"))))))))
            .bind("classes");
    finder.addMatcher(classMatcher, &serializer);

//    DeclarationMatcher enumMatcher = enumDecl();

    auto start = ch::high_resolution_clock::now();
    tool.run(newFrontendActionFactory(&finder).get());
    auto end = ch::high_resolution_clock::now();

    std::cerr << "Parsing - " << ch::duration_cast<ch::milliseconds>(end - start).count() << " ms" << std::endl;

    auto includeRoot = IncludeRoot.getValue();
    auto outputDir = OutputDir.getValue();
    auto relativeHeaderPath = fs::relative(fs::path(inputHeader), fs::path(includeRoot));
    auto relativeHeaderDir = relativeHeaderPath.parent_path();
    json data = json::object(
        {
            { "header", relativeHeaderPath.string() },
            { "generatedHeader",
                (relativeHeaderDir / fs::path(relativeHeaderPath.stem().string() + ".gen.h")).string() },
            { "classes", serializer.GetJSONObject() }
        }
    );

    // Load templates
    inja::Environment env;
    inja::Template commentTemplate = env.parse_template("./templates/comment.j2");
    env.include_template("comment", commentTemplate);

    inja::Template headerTemplate = env.parse_template("./templates/header.j2");
    inja::Template sourceTemplate = env.parse_template("./templates/source.j2");

    fs::path outDirPath = fs::path(OutputDir.getValue()) / relativeHeaderDir;
    fs::path outFilePath = outDirPath / relativeHeaderPath.stem();
    fs::create_directories(outDirPath);

    std::ofstream headerFile(outFilePath.string() + ".gen.h");
    std::ofstream sourceFile(outFilePath.string() + ".gen.cpp");

    env.render_to(headerFile, headerTemplate, data);
    env.render_to(sourceFile, sourceTemplate, data);

    return 0;
}