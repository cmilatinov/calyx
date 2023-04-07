#pragma once

#define CX_REFLECT_TYPE_TRAITS  "type_traits"_hs
#define CX_REFLECT_FIELD_META   "field_meta"_hs

#define CX_TRAIT_TRIVIALLY_COPYABLE     CX_BIT(0)
#define CX_TRAIT_DEFAULT_CONSTRUCTIBLE  CX_BIT(1)

#define CX_TRAIT(trait, cond) (uint32_t)(cond ? trait : 0)