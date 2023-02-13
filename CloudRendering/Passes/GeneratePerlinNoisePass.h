#pragma once
#include "Pass.h"

class GeneratePerlinNoisePass : public Pass {
private:
    uint32_t width = 0, height = 0;
    
    std::shared_ptr<MTL::ComputePipelineState> density_generation_pso;
    std::shared_ptr<MTL::Function> density_generation_shader;
    std::shared_ptr<MTL::Buffer> permutations_buffer;
    std::shared_ptr<MTL::Texture> cloud_density_map;
    
    std::shared_ptr<MTL::ComputePipelineState> normal_generation_pso;
    std::shared_ptr<MTL::Function> normal_generation_shader;
    std::shared_ptr<MTL::Texture> cloud_normal_map;
    
    
    void initialize_density_map_generation(GlobalGPUResource& gpu);
    
    void initialize_normal_map_generation(GlobalGPUResource& gpu);
    
public:
    void initialize_persistent_resources(GlobalGPUResource &gpu) override;
    
    
    void encode_pass(std::shared_ptr<MTL::CommandBuffer> cmdbuffer,
                     std::shared_ptr<MTL::Texture> framebuffer_texture) override;
    
    void set_dimensions(uint32_t w, uint32_t h);
    
    std::shared_ptr<MTL::Texture> get_cloud_density_map();
    
    std::shared_ptr<MTL::Texture> get_cloud_normal_map();

};
