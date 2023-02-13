//
//  DrawTextureToFramebufferPass.cpp
//  CloudRendering
//
//  Created by Zeyu Chen on 2022-10-06.
//

#include "DrawTextureToFramebufferPass.h"
#include <Util/Util.hpp>

void DrawTextureToFramebufferPass::initialize_persistent_resources(GlobalGPUResource& gpu) {
    vertex_shader = gpu.use_shader("vertex_passthrough");
    frag_shader   = gpu.use_shader("texture_passthrough");
    
    // create PSO
    pso = gpu.make_render_pso([&] (MTL::RenderPipelineDescriptor* desc) {
        desc->setVertexFunction(vertex_shader.get());
        desc->setFragmentFunction(frag_shader.get());
        desc->colorAttachments()->object(0)->setPixelFormat(MTL::PixelFormatBGRA8Unorm);
        desc->setSampleCount(1);
    });
    
    vertices_buffer = gpu.create_buffer(vertices_data.data(),
                                        sizeof(simd::float3) * vertices_data.size(),
                                        MTL::StorageModeManaged);
}


void DrawTextureToFramebufferPass::use_greyscale(bool p) {
    is_greyscale = p;
}


void DrawTextureToFramebufferPass::set_render_target_texture(std::shared_ptr<MTL::Texture> tex) {
    render_target_texture = tex;
}


void DrawTextureToFramebufferPass::set_texture(std::shared_ptr<MTL::Texture> _texture) {
    texture = _texture;
}


void DrawTextureToFramebufferPass::encode_pass(std::shared_ptr<MTL::CommandBuffer> cmd_buffer,
                                               std::shared_ptr<MTL::Texture> framebuffer_texture) {
    simd::uint2 viewport_size = { (uint32_t)render_target_texture->width(), (uint32_t)render_target_texture->height() };
    
    auto render_pass_desc = Util::new_scoped<MTL::RenderPassDescriptor>();
    render_pass_desc->setRenderTargetWidth(viewport_size.x);
    render_pass_desc->setRenderTargetHeight(viewport_size.y);
    render_pass_desc->colorAttachments()->object(0)->setTexture(render_target_texture.get());
    render_pass_desc->colorAttachments()->object(0)->setLoadAction(MTL::LoadActionClear);
    render_pass_desc->colorAttachments()->object(0)->setClearColor(MTL::ClearColor(0, 0, 0, 0));
    render_pass_desc->setDefaultRasterSampleCount(1);
    
    auto encoder = Util::scoped(cmd_buffer->renderCommandEncoder(render_pass_desc.get()));
    encoder->setRenderPipelineState(pso.get());
    encoder->setVertexBuffer(vertices_buffer.get(), 0, 0);
    encoder->setFragmentTexture(texture.get(), 0);
    encoder->setFragmentBytes(&viewport_size, sizeof(viewport_size), 0);
    encoder->setFragmentBytes(&is_greyscale, sizeof(bool), 1);
    encoder->drawPrimitives(MTL::PrimitiveTypeTriangle, 0, 6, 1);
    encoder->endEncoding();
}
