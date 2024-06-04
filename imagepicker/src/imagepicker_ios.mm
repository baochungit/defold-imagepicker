#if defined(DM_PLATFORM_IOS)

#import <Foundation/Foundation.h>
#import <UIKit/UIKit.h>
#include "imagepicker_private.h"

namespace ext_imagepicker {

void Initialize()
{
}

const char* GetTimezone()
{
	return [[[NSTimeZone localTimeZone] name] UTF8String];
}

} // namespace

#endif // DM_PLATFORM_IOS