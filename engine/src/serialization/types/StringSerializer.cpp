#include "serialization/types/StringSerializer.h"

namespace Calyx {

    void StringSerializer::CX_ON_SERIALIZE(std::ostream* stream, const String& value) {
        uint16 size = value.size();
        stream->write((char*)&size, sizeof(size));
        stream->write(value.data(), size);
    }

    void StringSerializer::CX_ON_DESERIALIZE(std::istream* stream, String& value) {
        uint16 size;
        stream->read((char*)&size, sizeof(size));
        value.resize(size);
        stream->read(value.data(), size);
    }

}