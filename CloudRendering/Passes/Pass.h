#pragma once

#include <Metal/Metal.hpp>
#include <Rendering/GlobalGPUResource.h>

class Pass {
public:
    virtual void initialize_persistent_resources(GlobalGPUResource& gpu) {}
    virtual void encode_pass(std::shared_ptr<MTL::CommandBuffer> cmdbuffer,
                             std::shared_ptr<MTL::Texture> framebuffer_texture) {}
};


