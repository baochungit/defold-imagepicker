#if defined(DM_PLATFORM_ANDROID)

#include <dmsdk/sdk.h>
#include <dmsdk/dlib/android.h>
#include "imagepicker_private.h"

namespace ext_imagepicker {

struct ImagePicker
{
	jobject                     m_Instance;
	jmethodID                   m_Show;
	dmScript::LuaCallbackInfo*  m_Listener;
};

static ImagePicker g_ImagePicker;

static char* CopyString(JNIEnv* env, jstring s)
{
	const char* javastring = env->GetStringUTFChars(s, 0);
	char* copy = strdup(javastring);
	env->ReleaseStringUTFChars(s, javastring);
	return copy;
}

extern "C" {
	JNIEXPORT void JNICALL Java_com_defold_imagepicker_ImagePickerJNI_onDone(JNIEnv* env, jobject, jstring text)
	{
		if (g_ImagePicker.m_Listener != 0)
		{
			lua_State* L = dmScript::GetCallbackLuaContext(g_ImagePicker.m_Listener);
			DM_LUA_STACK_CHECK(L, 0);

			if (dmScript::SetupCallback(g_ImagePicker.m_Listener))
			{
				lua_pushstring(L, CopyString(env, text));

				dmScript::PCall(L, 2, 0);
				dmScript::TeardownCallback(g_ImagePicker.m_Listener);
				dmScript::DestroyCallback(g_ImagePicker.m_Listener);
				g_ImagePicker.m_Listener = 0;
			}
		}
	}
}

void Initialize()
{
	dmAndroid::ThreadAttacher threadAttacher;
	JNIEnv* env = threadAttacher.GetEnv();

	jclass cls = dmAndroid::LoadClass(env, "com.defold.imagepicker.ImagePickerJNI");

	jmethodID constructor = env->GetMethodID(cls, "<init>", "(Landroid/app/Activity;)V");
	g_ImagePicker.m_Instance = env->NewGlobalRef(env->NewObject(cls, constructor, threadAttacher.GetActivity()->clazz));

	g_ImagePicker.m_Show = env->GetMethodID(cls, "show", "()V");
}

void Show(dmScript::LuaCallbackInfo* callback)
{
	dmAndroid::ThreadAttacher threadAttacher;
	JNIEnv* env = threadAttacher.GetEnv();
	g_ImagePicker.m_Listener = callback;
	env->CallVoidMethod(g_ImagePicker.m_Instance, g_ImagePicker.m_Show);
}

} // namespace

#endif // DM_PLATFORM_ANDROID