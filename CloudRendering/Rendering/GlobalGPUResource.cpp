
#define NS_PRIVATE_IMPLEMENTATION
#define CA_PRIVATE_IMPLEMENTATION
#define MTL_PRIVATE_IMPLEMENTATION
#include <Foundation/Foundation.hpp>
#include <Metal/Metal.hpp>
#include <QuartzCore/QuartzCore.hpp>

#include "GlobalGPUResource.h"
#include <Util/Util.hpp>

GlobalGPUResource::GlobalGPUResource() {
    device = MTL::CreateSystemDefaultDevice();
    shader_library = device->newDefaultLibrary();
    cmd_queue = device->newCommandQueue();
}

std::shared_ptr<MTL::Device> GlobalGPUResource::get_device() {
    device->retain();
    return Util::rc(device);
}


std::shared_ptr<MTL::CommandQueue> GlobalGPUResource::get_command_queue() {
    cmd_queue->retain();
    return Util::rc(cmd_queue);
}


std::shared_ptr<MTL::Function> GlobalGPUResource::use_shader(char const* name) {
    auto name_nsstring = Util::ns_str(name);
    auto fn = shader_library->newFunction(name_nsstring);
//    name_nsstring->release();
    if (fn == nullptr)
        throw std::runtime_error("failed to load shader");
    else
        return Util::rc(fn);
}


std::shared_ptr<MTL::Texture> GlobalGPUResource::make_texture(std::function<void(MTL::TextureDescriptor*)> f) {
    auto desc = Util::scoped(MTL::TextureDescriptor::alloc()->init());
    f(desc.get());
    auto ret = Util::rc(device->newTexture(desc.get()));
    return ret;
}


std::shared_ptr<MTL::Buffer> GlobalGPUResource::create_buffer(void* data, size_t size, MTL::ResourceOptions options) {
    return Util::rc(device->newBuffer(data, size, options));
}


std::shared_ptr<MTL::ComputePipelineState> GlobalGPUResource::make_compute_pso(std::shared_ptr<MTL::Function> compute_shader) {
    NS::Error* err;
    auto pso = device->newComputePipelineState(compute_shader.get(), &err);
    if (err != nullptr)
        throw std::runtime_error(Util::c_str(err->description()));
    else
        return Util::rc(pso);
}


std::shared_ptr<MTL::RenderPipelineState> GlobalGPUResource::make_render_pso(std::function<void(MTL::RenderPipelineDescriptor*)> f) {
    NS::Error* err;
    auto desc = Util::scoped(MTL::RenderPipelineDescriptor::alloc()->init());
    f(desc.get());
    auto pso = device->newRenderPipelineState(desc.get(), &err);
    if (err != nullptr)
        throw std::runtime_error(Util::c_str(err->description()));
    else
        return Util::rc(pso);
}
