#pragma once
#include "Pass.h"
#include <simd/simd.h>
#include <vector>

class DrawTextureToFramebufferPass : public Pass {
private:
    std::shared_ptr<MTL::RenderPipelineState> pso;
    std::shared_ptr<MTL::Function> vertex_shader;
    std::shared_ptr<MTL::Function> frag_shader;
    
    std::vector<simd::float3> vertices_data {
        simd::make_float3(-1.0f, 1.0f, 0.0f), simd::make_float3(-1.0f, -1.0f, 0.0f), simd::make_float3(1.0f, 1.0f, 0.0f),
        simd::make_float3(-1.0f, -1.0f, 0.0f), simd::make_float3(1.0f, -1.0f, 0.0f), simd::make_float3(1.0f, 1.0f, 0.0f)
    };
    
    std::shared_ptr<MTL::Buffer> vertices_buffer;
    
    std::shared_ptr<MTL::Texture> render_target_texture;
    std::shared_ptr<MTL::Texture> texture;
    bool is_greyscale = false;
    
public:
    void initialize_persistent_resources(GlobalGPUResource& gpu) override;
    
    // specific setups
    void use_greyscale(bool p);
    
    void set_render_target_texture(std::shared_ptr<MTL::Texture> tex);
    
    void set_texture(std::shared_ptr<MTL::Texture> _texture);
    
    /*
     Encode a render pass
     */
    void encode_pass(std::shared_ptr<MTL::CommandBuffer> cmd_buffer,
                     std::shared_ptr<MTL::Texture> framebuffer_texture) override;
};
