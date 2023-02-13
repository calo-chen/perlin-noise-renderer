#pragma once

#include <memory>
#include <optional>
#include <functional>

#include <Metal/Metal.hpp>


class GlobalGPUResource {
private:
    MTL::Device* device;
    MTL::Library* shader_library;
    MTL::CommandQueue* cmd_queue;
    
public:
    explicit GlobalGPUResource();
    
    /**
     Retrieve Metal device object
     */
    std::shared_ptr<MTL::Device> get_device();
    
    /**
     Get command queue
     */
    std::shared_ptr<MTL::CommandQueue> get_command_queue();
    
    /**
     Get Metal shader object
     */
    std::shared_ptr<MTL::Function> use_shader(char const* name);
    

    /**
     Create texture object
     */
    std::shared_ptr<MTL::Texture> make_texture(std::function<void(MTL::TextureDescriptor*)> f);
    
    
    /**
     Upload buffer data
     */
    std::shared_ptr<MTL::Buffer> create_buffer(void* data, size_t size, MTL::ResourceOptions options);
    
    
    /**
     Create compute pipeline state object
     */
    std::shared_ptr<MTL::ComputePipelineState> make_compute_pso(std::shared_ptr<MTL::Function> compute_shader);
    
    
    /**
     Create render pipeline state object
     */
    std::shared_ptr<MTL::RenderPipelineState> make_render_pso(std::function<void(MTL::RenderPipelineDescriptor*)> f);
    
    
    /**
     Reduce reference counts
     */
    ~GlobalGPUResource() {
        cmd_queue->release();
        shader_library->release();
        device->release();
    }
};
