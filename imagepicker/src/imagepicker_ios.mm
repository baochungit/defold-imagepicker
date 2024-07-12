#if defined(DM_PLATFORM_IOS)

#import <Foundation/Foundation.h>
#import <UIKit/UIKit.h>
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


@interface CImagePickerHandler : NSObject<UINavigationControllerDelegate, UIImagePickerControllerDelegate>

- (void)show;

@end

@implementation CImagePickerHandler

- (void)show
{
	UIImagePickerController * picker = [[UIImagePickerController alloc] init];
	picker.delegate = self;
	picker.sourceType = UIImagePickerControllerSourceTypePhotoLibrary;
	picker.allowsEditing = true;
	[picker setModalPresentationStyle:UIModalPresentationOverFullScreen];

	UIViewController* controller = [[[UIApplication sharedApplication] keyWindow] rootViewController];
	[controller presentViewController:picker animated:YES completion:nil];
}

- (void)imagePickerController:(UIImagePickerController *)picker didFinishPickingImage:(UIImage *)editedImage editingInfo:(NSDictionary *)editingInfo
{
	NSString* fileName = [[[NSUUID UUID] UUIDString] stringByAppendingString:@".jpg"];
	NSArray* paths = NSSearchPathForDirectoriesInDomains(NSDocumentDirectory, NSUserDomainMask, YES);
	NSString* filePath = [[paths objectAtIndex:0] stringByAppendingPathComponent:fileName];
	[UIImageJPEGRepresentation(editedImage, 0.5) writeToFile:filePath atomically:YES];
	NSLog(@"Image path: %@", filePath);

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

	[picker dismissViewControllerAnimated:YES completion:nil];
}

- (void)imagePickerControllerDidCancel:(UIImagePickerController *)picker{
	NSLog(@"Cancelled");
	[picker dismissViewControllerAnimated:YES completion:nil];
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