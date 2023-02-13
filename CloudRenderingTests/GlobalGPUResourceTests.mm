
#import <XCTest/XCTest.h>
#include "GlobalGPUResource.h"

@interface GlobalGPUResourceTests : XCTestCase

@end

@implementation GlobalGPUResourceTests
{
    GlobalGPUResource* gpu;
}


- (void)setUp {
    gpu = new GlobalGPUResource();
}

- (void)tearDown {
    delete gpu;
}

- (void)testGetDevice {
    auto device = gpu->get_device();
    XCTAssert(device != nullptr);
}

- (void)testGetShader1 {
    auto s = gpu->use_shader("hello");
    XCTAssert(s != nullptr);
}

- (void)testGetShader2 {
    try {
        auto s = gpu->use_shader("vertex_passthrough");
    } catch (std::runtime_error const& e) {
        XCTAssert(true);
    }
}

- (void)testCreateBuffer1 {
    int data[4] = { 1, 2, 3, 4 };
    auto buf = gpu->create_buffer(data, 4 * sizeof(int), MTL::StorageModeManaged);
    XCTAssert(buf->allocatedSize() == 4 * sizeof(int));
}


@end
