#pragma once

// Annotation names
#define CX_REFLECT_SERIALIZE_ANNOTATION "serialize"
#define CX_REFLECT_NAME_ANNOTATION "name"

// Annotation definitions
#ifdef __clang__
#define CX_REFLECT_SERIALIZE __attribute__((annotate(CX_REFLECT_SERIALIZE_ANNOTATION)))
#define CX_REFLECT_NAME(name) __attribute__((annotate(CX_REFLECT_NAME_ANNOTATION "=" #name)))
#else
#define CX_REFLECT_SERIALIZE
#define CX_REFLECT_NAME(...)
#endif