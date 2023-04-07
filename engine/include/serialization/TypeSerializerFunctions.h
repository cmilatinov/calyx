#pragma once

#define CX_ON_SERIALIZE                 OnSerialize
#define CX_ON_SERIALIZE_HS              CX_CONCAT(CX_XSTR(CX_ON_SERIALIZE), _hs)

#define CX_ON_DESERIALIZE               OnDeserialize
#define CX_ON_DESERIALIZE_HS            CX_CONCAT(CX_XSTR(CX_ON_DESERIALIZE), _hs)

#define CX_ON_POST_DESERIALIZE          OnPostDeserialize
#define CX_ON_POST_DESERIALIZE_HS       CX_CONCAT(CX_XSTR(CX_ON_POST_DESERIALIZE), _hs)
