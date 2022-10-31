#pragma once

// Automatic registration function
#define CX_REFLECT_REGISTRATION_STRUCT_NAME reflect__auto__register__
#define CX_REFLECT_REGISTRATION_STRUCT_VARIABLE auto_register__
#define CX_REFLECT_REGISTRATION_FN_NAME reflect_auto_register_reflection_function_
#define CX_REFLECT_REGISTRATION                                                             \
static void CX_REFLECT_REGISTRATION_FN_NAME();                                              \
namespace {                                                                                 \
    struct CX_REFLECT_REGISTRATION_STRUCT_NAME {                                            \
        CX_REFLECT_REGISTRATION_STRUCT_NAME() {                                             \
            CX_REFLECT_REGISTRATION_FN_NAME();                                              \
        }                                                                                   \
    };                                                                                      \
}                                                                                           \
static const CX_REFLECT_REGISTRATION_STRUCT_NAME CX_REFLECT_REGISTRATION_STRUCT_VARIABLE;   \
static void CX_REFLECT_REGISTRATION_FN_NAME()

// Class registration macro and forward decl
#define CX_BEGIN_REFLECT() friend void ::CX_REFLECT_REGISTRATION_FN_NAME()
static void CX_REFLECT_REGISTRATION_FN_NAME();