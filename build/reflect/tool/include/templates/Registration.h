const char* TEMPLATE_REGISTRATION =
    R"(## include "comment"

#include "{{header}}"

CX_REFLECT_REGISTRATION {
    using namespace Calyx::Reflect;
    using namespace entt::literals;

## for class in classes
    // Class '{{class.fullName}}'
    entt::meta<{{class.fullName}}>()
        .type("{{class.fullName}}"_hs)
        .prop(CX_REFLECT_FIELD_NAMES, Core::FieldNameMap({
## for field in class.fields
            { "{{field.name}}"_hs, "{{field.name}}" }{% if not loop.is_last %},{% endif %}
## endfor
        }))
        .prop(CX_REFLECT_FIELD_OFFSETS, Core::FieldOffsetMap({
## for field in class.fields
            { "{{field.name}}"_hs, static_cast<int32_t>(offsetof({{class.fullName}}, {{field.name}})) }{% if not loop.is_last %},{% endif %}
## endfor
        }))
## for base in class.bases
        .base<{{base}}>()
## endfor
## if length(class.constructors) > 0
## for constructor in class.constructors
        .ctor<{% for param in constructor.parameters %}{{param}}{% if not loop.is_last %},{% endif %}{% endfor %}>()
## endfor
## endif
## for field in class.fields
        .data<{{field.pointer}}>("{{field.name}}"_hs)
## endfor
## for method in class.methods
        .func<entt::overload<{{method.type}}>({{method.pointer}})>("{{method.name}}"_hs)
## endfor
        ;

## for base in class.bases
    Core::RegisterDerivedClass(entt::resolve<{{base}}>(), entt::resolve<{{class.fullName}}>());
## endfor

## if length(class.ref_conversions) > 0
    entt::meta<Ref<{{class.fullName}}>>()
## for ref in class.ref_conversions
        .conv<entt::overload<Ref<{{ref}}> (const Ref<{{class.fullName}}>&)>(&std::static_pointer_cast<{{ref}}, {{class.fullName}}>)>()
## endfor
        ;
## endif
## endfor
})";