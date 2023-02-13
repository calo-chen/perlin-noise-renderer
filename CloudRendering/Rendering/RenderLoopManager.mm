//
//  RenderLoopManager.cpp
//  CloudRendering
//
//  Created by Zeyu Chen on 2022-10-06.
//

#include "RenderLoopManager.h"
#import <QuartzCore/QuartzCore.h>
#include <QuartzCore/QuartzCore.hpp>


CA::MetalDrawable* RenderLoopManager::retrieve_next_drawable() {
    __block id<CAMetalDrawable> next_drawable;
    dispatch_sync(dispatch_get_main_queue(), ^{
        next_drawable = [(__bridge CAMetalLayer*)metal_layer nextDrawable];
    });
    auto ret = (__bridge CA::MetalDrawable*)next_drawable;
    ret->retain();
    return ret;
}
