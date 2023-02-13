
#include "DrawSkydomePass.h"
#include <Util/Util.hpp>
#include <string>
#include <Util/ObjLoader.hpp>

void DrawSkydomePass::initialize_persistent_resources(GlobalGPUResource& gpu) {
    std::string dome_path = Util::get_full_path("Assets/hemisphere.obj");
    ObjLoader hemisphere{ dome_path };
    vertex_count = hemisphere.get_vertices_count();
    index_count = hemisphere.get_indices_count();
    
    vertices = gpu.create_buffer(hemisphere.get_vertices_data(),
                                 hemisphere.get_vertices_data_size(),
                                 MTL::StorageModeManaged);
    
    indices = gpu.create_buffer(hemisphere.get_indices_data(),
                                hemisphere.get_indices_data_size(),
                                MTL::StorageModeManaged);
    
    vertex_shader = gpu.use_shader("transform");
    frag_shader = gpu.use_shader("draw_skydome");
    skydome_pso = gpu.make_render_pso([&] (MTL::RenderPipelineDescriptor* desc) {
        desc->setVertexFunction(vertex_shader.get());
        desc->setFragmentFunction(frag_shader.get());
        desc->colorAttachments()->object(0)->setPixelFormat(MTL::PixelFormatBGRA8Unorm);
        desc->setSampleCount(1);
    });
}


void DrawSkydomePass::encode_pass(std::shared_ptr<MTL::CommandBuffer> cmdbuffer,
                                  std::shared_ptr<MTL::Texture> framebuffer_texture) {
    auto rp_desc = Util::new_scoped<MTL::RenderPassDescriptor>();
    auto x = framebuffer_texture->width(), y = framebuffer_texture->height();
    rp_desc->setRenderTargetWidth(x);
    rp_desc->setRenderTargetHeight(y);
    rp_desc->colorAttachments()->object(0)->setTexture(framebuffer_texture.get());
    rp_desc->colorAttachments()->object(0)->setLoadAction(MTL::LoadActionClear);
    rp_desc->colorAttachments()->object(0)->setClearColor(MTL::ClearColor::Make(0, 0, 0, 0));
    
    auto encoder = cmdbuffer->renderCommandEncoder(rp_desc.get());
    
    encoder->setRenderPipelineState(skydome_pso.get());
    encoder->setVertexBuffer(vertices.get(), 0, 0);
    encoder->setFrontFacingWinding(MTL::WindingClockwise);
    
    simd::float4x4 lookat = Math::look_at({ 0.f, 0.f, 0.0f },
                                          { 0, cos(Math::radian(45)), sin(Math::radian(45)) },
                                          { 0, 1, 0 });
    simd::float4x4 view = lookat * Math::scale(100.f);
    
//    simd::float4x4 view = Math::scale(5);
    
    encoder->setVertexBytes(&view, sizeof(simd::float4x4), 1);
    
    simd::float4x4 view_t_i = simd::transpose(simd::inverse(view));
    encoder->setVertexBytes(&view_t_i, sizeof(simd::float4x4), 2);
    
    simd::float4x4 proj = Math::perspective(Math::radian(100.f), x / y, 0.0f, 100.f);
//    simd::float4x4 proj = simd::float4x4(1.0f);
    
    encoder->setVertexBytes(&proj, sizeof(simd::float4x4), 3);
    
    encoder->setFragmentTexture(dome_texture.get(), 0);
    
    encoder->setFragmentTexture(dome_texture.get(), 0);
    
    encoder->drawIndexedPrimitives(MTL::PrimitiveTypeTriangle,
                                   index_count,
                                   MTL::IndexTypeUInt32,
                                   indices.get(), 0);
    encoder->endEncoding();
}


void DrawSkydomePass::set_dome_texture(std::shared_ptr<MTL::Texture> tex) {
    dome_texture = tex;
}
