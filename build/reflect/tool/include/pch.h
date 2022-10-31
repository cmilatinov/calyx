#include <iostream>
#include <fstream>
#include <sstream>
#include <numeric>
#include <chrono>
#include <filesystem>

#include <vector>
#include <string>
#include <unordered_map>

#include <clang/AST/DeclBase.h>
#include <clang/ASTMatchers/ASTMatchers.h>
#include <clang/ASTMatchers/ASTMatchFinder.h>
#include <clang/AST/RecursiveASTVisitor.h>

#include <clang/Frontend/FrontendActions.h>
#include <clang/Tooling/CommonOptionsParser.h>
#include <clang/Tooling/Tooling.h>

#include <llvm/Support/CommandLine.h>

#include <nlohmann/json.hpp>
#include <inja/inja.hpp>

using json = nlohmann::json;
using namespace llvm;
using namespace clang;
using namespace clang::ast_matchers;
using namespace clang::tooling;
namespace ch = std::chrono;
namespace fs = std::filesystem;

#include "interfaces/IDeclProcessor.h"
#include "interfaces/IJSONSerializer.h"
#include "definitions/Member.h"
#include "definitions/Utils.h"
#include "reflect.h"