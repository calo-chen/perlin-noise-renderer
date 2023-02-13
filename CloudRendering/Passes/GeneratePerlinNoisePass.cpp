
#include "GeneratePerlinNoisePass.h"
#include <Util/Util.hpp>
#include <random>

void GeneratePerlinNoisePass::initialize_density_map_generation(GlobalGPUResource& gpu) {
    density_generation_shader = gpu.use_shader("generate_cloud_density_map");
    density_generation_pso = gpu.make_compute_pso(density_generation_shader);
    
    // load GPU buffer
    std::uniform_real_distribution<float> dist { 0.f, 2 * Math::pi };
    std::default_random_engine rng;
    std::vector<float> p;
    
    for (int i = 0; i < 128; i += 1)
        p.push_back(dist(rng));
    
    permutations_buffer = gpu.create_buffer(p.data(), p.size() * sizeof(float), MTL::StorageModeManaged);
    cloud_density_map = gpu.make_texture([&] (MTL::TextureDescriptor* desc) {
        desc->setWidth(width);
        desc->setHeight(height);
        desc->setTextureType(MTL::TextureType2D);
        desc->setUsage(MTL::TextureUsageShaderRead | MTL::TextureUsageShaderWrite);
        desc->setPixelFormat(MTL::PixelFormatR32Float);
    });
}


void GeneratePerlinNoisePass::initialize_normal_map_generation(GlobalGPUResource& gpu) {
    normal_generation_shader = gpu.use_shader("generate_normal_map");
    normal_generation_pso = gpu.make_compute_pso(normal_generation_shader);
    cloud_normal_map = gpu.make_texture([&] (MTL::TextureDescriptor* desc) {
        desc->setWidth(width);
        desc->setHeight(height);
        desc->setTextureType(MTL::TextureType2D);
        desc->setUsage(MTL::TextureUsageShaderRead | MTL::TextureUsageShaderWrite);
        desc->setPixelFormat(MTL::PixelFormatRGBA8Snorm);
    });
}


void GeneratePerlinNoisePass::initialize_persistent_resources(GlobalGPUResource &gpu) {
    initialize_density_map_generation(gpu);
    initialize_normal_map_generation(gpu);
}


void GeneratePerlinNoisePass::encode_pass(std::shared_ptr<MTL::CommandBuffer> cmdbuffer,
                                          std::shared_ptr<MTL::Texture> framebuffer_texture) {
    auto encoder = Util::scoped(cmdbuffer->computeCommandEncoder());
    auto dispatch_size = MTL::Size::Make(width, height, 1);
    simd::uint2 dimension = { width, height };
    
    /// Dispatch `generate_cloud_density_map`
    {
        encoder->setComputePipelineState(density_generation_pso.get());
        // texture dimension
        encoder->setBytes(&dimension, sizeof(simd::uint2), 0);
        // permutation array
        encoder->setBuffer(permutations_buffer.get(), 0, 1);
        // output texture
        encoder->setTexture(cloud_density_map.get(), 0);
        
        auto threadgroup_size = MTL::Size::Make(density_generation_pso->threadExecutionWidth(),
                                                density_generation_pso->threadExecutionWidth(), 1);
        encoder->dispatchThreads(dispatch_size, threadgroup_size);
    }
    
    /// Dispatch `generate_normal_map`
    {
        encoder->setComputePipelineState(normal_generation_pso.get());
        encoder->setBytes(&dimension, sizeof(simd::uint2), 0);
        encoder->setTexture(cloud_density_map.get(), 0);
        encoder->setTexture(cloud_normal_map.get(), 1);
        auto threadgroup_size = MTL::Size::Make(normal_generation_pso->threadExecutionWidth(),
                                                normal_generation_pso->threadExecutionWidth(), 1);
        encoder->dispatchThreads(dispatch_size, threadgroup_size);
    }
    
    encoder->endEncoding();
}


void GeneratePerlinNoisePass::set_dimensions(uint32_t w, uint32_t h) {
    width = w;
    height = h;
}


std::shared_ptr<MTL::Texture> GeneratePerlinNoisePass::get_cloud_density_map() {
    return this->cloud_density_map;
}


std::shared_ptr<MTL::Texture> GeneratePerlinNoisePass::get_cloud_normal_map() {
    return this->cloud_normal_map;
}
