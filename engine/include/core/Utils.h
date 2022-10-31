#pragma once

namespace Calyx::Utils {

    struct MetaTypeHash {
        std::size_t operator()(const entt::meta_type& value) const {
            return value.info().hash();
        }
    };

}