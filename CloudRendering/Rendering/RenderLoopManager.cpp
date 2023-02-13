
#include "RenderLoopManager.h"
#include <Util/Util.hpp>

void RenderLoopManager::render_loop() {
    while (true) {
        auto drawable = Util::scoped(retrieve_next_drawable());
        auto cmd_buffer = Util::rc(gpu.get_command_queue()->commandBuffer());
        
        std::shared_ptr<MTL::Texture> framebuffer_texture = Util::rc(drawable->texture());
//        per_frame_setup(framebuffer_texture);
        
        for (std::shared_ptr<Pass> pass : passes) {
            pass->encode_pass(cmd_buffer, framebuffer_texture);
        }
        cmd_buffer->presentDrawable(drawable.get());
        cmd_buffer->commit();
        cmd_buffer->waitUntilCompleted();
        
        std::this_thread::sleep_for(std::chrono::milliseconds(16));
    }
}


void RenderLoopManager::set_metal_layer(CA::MetalLayer* layer) {
    metal_layer = layer;
}


void RenderLoopManager::add_render_pass(std::shared_ptr<Pass> pass) {
    passes.push_back(pass);
    pass->initialize_persistent_resources(gpu);
}


void RenderLoopManager::start_render_loop() {
    rendering_thread = std::thread(&RenderLoopManager::render_loop, this);
}
