#pragma once

#include <Passes/Pass.h>
#include <QuartzCore/QuartzCore.hpp>
#include "GlobalGPUResource.h"

#include <vector>
#include <thread>

class RenderLoopManager {
private:
    GlobalGPUResource& gpu;
    std::vector<std::shared_ptr<Pass>> passes;
    CA::MetalLayer* metal_layer;
    std::function<void(std::shared_ptr<MTL::Texture>)> per_frame_setup;
    
    CA::MetalDrawable* retrieve_next_drawable();
    std::thread rendering_thread;
    
    void render_loop();
    
public:
    explicit RenderLoopManager(GlobalGPUResource& _gpu)
        : gpu(_gpu) {}
    
    void add_render_pass(std::shared_ptr<Pass> pass);
    
    void set_metal_layer(CA::MetalLayer* metal_layer);
    
    void start_render_loop();
};
