#include "UUIDUtils.h"

namespace Calyx::UUIDUtils {

    uuids::uuid Generate() {
        static uuids::uuid_system_generator gen{};
        return gen();
    }

}