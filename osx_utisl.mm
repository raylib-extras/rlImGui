#include "osx_utils.h"
#include <Cocoa/Cocoa.h>

float getBackingScaleFactor() {
    float scaleFactor = 1.0;
    
    @autoreleasepool {
        NSArray<NSScreen *> *screens = [NSScreen screens];
        if ([screens count] > 0) {
            NSScreen *screen = [screens objectAtIndex:0];
            scaleFactor = [screen backingScaleFactor];
        }
    }
    
    return scaleFactor;
}

