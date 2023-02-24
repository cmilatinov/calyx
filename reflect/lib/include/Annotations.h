#pragma once

// Annotation names
#define CX_REFLECT_ANNOTATION_SERIALIZE serialize
#define CX_REFLECT_ANNOTATION_NAME name

// Annotation definitions
#ifdef __clang__
#define CX_PROPERTY(...) __attribute__((annotate(CX_XSTR_ALL(__VA_ARGS__))))
#define CX_SERIALIZE CX_REFLECT_ANNOTATION_SERIALIZE
#define CX_NAME(x) CX_REFLECT_ANNOTATION_NAME=#x
#else
#define CX_PROPERTY(...)
#define CX_SERIALIZE
#define CX_NAME(...)
#endif