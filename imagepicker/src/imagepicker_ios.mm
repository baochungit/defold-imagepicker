#if defined(DM_PLATFORM_IOS)

#import <Foundation/Foundation.h>
#import <UIKit/UIKit.h>
#import <JQImagePicker.h>
#include <dmsdk/sdk.h>
#include "imagepicker_private.h"


struct ImagePicker
{
	dmScript::LuaCallbackInfo*  m_Listener;
};

static ImagePicker g_ImagePicker;

static char* CopyString(NSString* s)
{
	const char* osstring = [s UTF8String];
	char* copy = strdup(osstring);
	return copy;
}


@interface CImagePickerHandler : NSObject<JQImagePickerDelegate>

UIViewController viewController;

- (void)show;

@end

@implementation CImagePickerHandler

- (void)show
{
	self.viewController = [[UIViewController alloc] init];

	JQImagePicker *imagePicker = [JQImagePicker sharedInstance];
	imagePicker.delegate = self;
	[imagePicker showOriginalImagePickerWithType:ImagePickerPhoto InViewController:viewController];
}

#pragma mark - JQImagePicker Delegate

- (void)imagePickerDidCancel:(JQImagePicker *)imagePicker
{
	NSLog(@"Cancelled");
	[viewController release];
}

- (void)imagePicker:(JQImagePicker *)imagePicker didFinished:(UIImage *)editedImage
{
	// NSLog(@"%@",editedImage);
	NSString* fileName = [[NSUUID UUID] UUIDString];
	NSArray* paths = NSSearchPathForDirectoriesInDomains(NSDocumentDirectory, NSUserDomainMask, YES);
	NSString* filePath = [[paths objectAtIndex:0] stringByAppendingPathComponent:@fileName];
	[UIImageJPEGRepresentation(editedImage, 1.0) writeToFile:filePath atomically:YES];
	NSLog(@"%@",filePath);
	[viewController release];

	if (g_ImagePicker.m_Listener != 0)
	{
		lua_State* L = dmScript::GetCallbackLuaContext(g_ImagePicker.m_Listener);
		DM_LUA_STACK_CHECK(L, 0);

		if (dmScript::SetupCallback(g_ImagePicker.m_Listener))
		{
			lua_pushstring(L, CopyString(filePath));

			dmScript::PCall(L, 2, 0);
			dmScript::TeardownCallback(g_ImagePicker.m_Listener);
			dmScript::DestroyCallback(g_ImagePicker.m_Listener);
			g_ImagePicker.m_Listener = 0;
		}
	}
}

@end


namespace ext_imagepicker {

void Initialize()
{
}

void Show(dmScript::LuaCallbackInfo* callback)
{
	g_ImagePicker.m_Listener = callback;
	CImagePickerHandler* imagePicker = [[CImagePickerHandler alloc] init];
	[imagePicker show];
}

} // namespace

#endif // DM_PLATFORM_IOS