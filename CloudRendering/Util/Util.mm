//
//  Util.cpp
//  CloudRendering
//
//  Created by Zeyu Chen on 2022-10-06.
//

#import <AppKit/AppKit.h>
#include <string>

namespace Util {
    std::string get_full_path(std::string const& path) {
        NSString* relative_path = [[NSString alloc] initWithUTF8String:path.c_str()];
        NSString* resource_path = [[NSBundle mainBundle] resourcePath];
        NSString* fullPath = [[resource_path stringByAppendingString:@"/"] stringByAppendingString:relative_path];
        return std::string([fullPath cStringUsingEncoding:NSUTF8StringEncoding]);
    }

}


