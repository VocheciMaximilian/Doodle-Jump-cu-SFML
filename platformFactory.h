#ifndef PLATFORMFACTORY_H
#define PLATFORMFACTORY_H

#include "Platform.h"
#include "BreakablePlatform.h"
#include "ColorChangingPlatform.h"

enum class PlatformType {
    Normal,
    Breakable,
    ColorChanging
};

class PlatformFactory {
public:
    static Platform* createPlatform(PlatformType type, float x, float y) {
        switch (type) {
            case PlatformType::Normal:
                return new Platform(x, y);

            case PlatformType::Breakable:
                // Using durability of 2 for demonstration
                    return new BreakablePlatform<int>(x, y, 2);

            case PlatformType::ColorChanging:
                return new ColorChangingPlatform(x, y);
        }
        return nullptr;
    }
};

#endif

