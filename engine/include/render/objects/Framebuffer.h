#pragma once

#include "render/objects/IRenderTarget.h"

namespace Calyx {

    class CALYX_API Framebuffer {

    public:
        class Attachment {

        public:
            struct Specification {
                Ref<IRenderTarget> target = nullptr;
                IRenderTarget::Type type;
                TextureFormat format;

                Specification(IRenderTarget::Type type, TextureFormat format)
                    : type(type), format(format) {}
                Specification(const Ref<IRenderTarget>& target)
                    : target(target), type(target->GetType()), format(target->GetTextureFormat()) {}
            };

        };

        struct Specification {
            uint32 width, height;
            uint32 samples;
            List<Attachment::Specification> attachments;
        };

    public:
        virtual ~Framebuffer() = default;

        virtual void Bind() const = 0;
        virtual void Unbind() const = 0;

        virtual void Resize(uint32 width, uint32 height) = 0;
        virtual void ClearColorAttachment(uint32 attachmentIndex, uint32 value) = 0;
        virtual void ClearDepthAttachment(uint32 value) = 0;
        virtual void Blit(const Ref<Framebuffer>& other, uint32 srcAttachment, uint32 dstAttachment) = 0;

        virtual const IRenderTarget& GetColorAttachment(uint32 attachmentIndex) const = 0;
        virtual const IRenderTarget& GetDepthAttachment() const = 0;

        virtual const Framebuffer::Specification& GetSpecification() const = 0;
        virtual uint32 GetRendererID() const = 0;
        virtual bool IsComplete() const = 0;

        static Ref<Framebuffer> Create(const Framebuffer::Specification& spec);

    };

}