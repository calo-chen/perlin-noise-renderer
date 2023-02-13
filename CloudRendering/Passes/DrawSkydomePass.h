#pragma once
#include "Pass.h"

class DrawSkydomePass : public Pass {
private:    
    std::shared_ptr<MTL::RenderPipelineState> skydome_pso;
    std::shared_ptr<MTL::Function> vertex_shader;
    std::shared_ptr<MTL::Function> frag_shader;
    size_t vertex_count, index_count;
    
    std::shared_ptr<MTL::Buffer> vertices;
    std::shared_ptr<MTL::Buffer> indices;
    
    std::shared_ptr<MTL::Texture> dome_texture;
    
public:
    void initialize_persistent_resources(GlobalGPUResource& gpu) override;
    
    void encode_pass(std::shared_ptr<MTL::CommandBuffer> cmdbuffer,
                     std::shared_ptr<MTL::Texture> framebuffer_texture) override;
    
    void set_dome_texture(std::shared_ptr<MTL::Texture> tex);
};
