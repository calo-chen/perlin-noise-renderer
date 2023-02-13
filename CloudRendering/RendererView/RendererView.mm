//
//  RendererView.m
//  CloudRendering
//
//  Created by Zeyu Chen on 2022-03-19.
//

#import "RendererView.h"
#import <QuartzCore/QuartzCore.h>
#import <Metal/Metal.h>
#include <memory>
#include <Rendering/RenderLoopManager.h>
#include <Passes/GeneratePerlinNoisePass.h>
#include <Passes/DrawSkydomePass.h>

@implementation RendererView
{
    CAMetalLayer* metalLayer;
    std::shared_ptr<GlobalGPUResource> gpu;
    std::shared_ptr<RenderLoopManager> manager;

    std::shared_ptr<GeneratePerlinNoisePass> perlin;
    std::shared_ptr<DrawSkydomePass> skydome;
}

//- (id) init {
//    self = [super init];
//
//    metalLayer = [[CAMetalLayer alloc] init];
//    renderer = std::make_shared<Renderer>();
//    [metalLayer setDevice:(__bridge id<MTLDevice>) renderer->get_device()];
//    renderer->set_metal_layer((__bridge CA::MetalLayer*) metalLayer);
//
//    self.wantsLayer = YES;
//    self.layer = metalLayer;
//    return self;
//}
//
//- (void) beginRendering {
//    renderer->start_render_loop();
//}

- (id)init {
    self = [super init];

    metalLayer = [[CAMetalLayer alloc] init];

    gpu = std::make_shared<GlobalGPUResource>();
    gpu->use_shader("generate_cloud_density_map");

    metalLayer.device = (__bridge id<MTLDevice>) gpu->get_device().get();

    manager = std::make_shared<RenderLoopManager>(*gpu);
    manager->set_metal_layer((__bridge CA::MetalLayer*) metalLayer);

    perlin = std::make_shared<GeneratePerlinNoisePass>();
    perlin->set_dimensions(2048, 2048);

    skydome = std::make_shared<DrawSkydomePass>();

    manager->add_render_pass(perlin);
    manager->add_render_pass(skydome);

    skydome->set_dome_texture(perlin->get_cloud_density_map());

    self.wantsLayer = YES;
    self.layer = metalLayer;
    return self;
}

- (void)beginRendering {
    manager->start_render_loop();
}

@end
