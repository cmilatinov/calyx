#pragma once

#include "render/objects/IRenderTarget.h"

namespace Calyx {

    class Framebuffer {

    public:
        class Attachment {

        public:
            struct Spec {
                Ref<IRenderTarget> target = nullptr;
                IRenderTarget::Type type;
                TextureFormat format;

                Spec(IRenderTarget::Type type, TextureFormat format)
                    : type(type), format(format) {}
                Spec(const Ref<IRenderTarget>& target)
                    : target(target), type(target->GetType()), format(target->GetTextureFormat()) {}
            };

        };

        struct Spec {
            uint32 width, height;
            uint32 samples;
            List<Attachment::Spec> attachments;
        };

    public:
        virtual ~Framebuffer() = default;

        virtual void Bind() const = 0;
        virtual void Unbind() const = 0;

        virtual void Resize(uint32 width, uint32 height) = 0;
        virtual void ClearAttachment(uint32 attachmentIndex, uint32 value) = 0;
        virtual void Blit(const Ref<Framebuffer>& other, uint32 srcAttachment, uint32 dstAttachment) = 0;

        virtual const Framebuffer::Spec& GetSpec() const = 0;
        virtual uint32 GetRendererID() const = 0;
        virtual bool IsComplete() const = 0;

        static Ref<Framebuffer> Create(const Framebuffer::Spec& spec);

    };

}