#if defined(DM_PLATFORM_ANDROID)

#include <dmsdk/dlib/android.h>
#include "imagepicker_private.h"

namespace ext_imagepicker {

struct ImagePicker
{
	jobject            m_Instance;
	jmethodID          m_Show;
};

static ImagePicker g_ImagePicker;

void Initialize()
{
	dmAndroid::ThreadAttacher threadAttacher;
	JNIEnv* env = threadAttacher.GetEnv();

	jclass cls = dmAndroid::LoadClass(env, "com.defold.imagepicker.ImagePickerJNI");

	jmethodID constructor = env->GetMethodID(cls, "<init>", "(Landroid/app/Activity;)V");
	g_ImagePicker.m_Instance = env->NewGlobalRef(env->NewObject(cls, constructor, threadAttacher.GetActivity()->clazz));

	g_ImagePicker.m_Show = env->GetMethodID(cls, "show", "()V");
}

const char* CallStringMethod(jobject instance, jmethodID method)
{
	dmAndroid::ThreadAttacher threadAttacher;
	JNIEnv* env = threadAttacher.GetEnv();

	jstring return_value = (jstring)env->CallObjectMethod(instance, method);
	return env->GetStringUTFChars(return_value, 0);
}

void Show()
{
	dmAndroid::ThreadAttacher threadAttacher;
	JNIEnv* env = threadAttacher.GetEnv();
	env->CallVoidMethod(g_ImagePicker.m_Instance, g_ImagePicker.m_Show);
}

} // namespace

#endif // DM_PLATFORM_ANDROID