#define LIB_NAME "ImagePicker"
#define MODULE_NAME "imagepicker"

#include <dmsdk/sdk.h>
#include "imagepicker_private.h"

#if defined(DM_PLATFORM_ANDROID) || defined(DM_PLATFORM_IOS)

namespace ext_imagepicker {

static int ImagePicker_Show(lua_State* L)
{
	DM_LUA_STACK_CHECK(L, 0);
	Show();
	return 0;
}

// Functions exposed to Lua
static const luaL_reg Module_methods[] =
{
	{"show", ImagePicker_Show},
	{0, 0}
};

static void LuaInit(lua_State* L)
{
	int top = lua_gettop(L);

	// Register lua names
	luaL_register(L, MODULE_NAME, Module_methods);

	lua_pop(L, 1);
	assert(top == lua_gettop(L));
}

} // namespace

dmExtension::Result AppInitializeImagePicker(dmExtension::AppParams* params)
{
	return dmExtension::RESULT_OK;
}

dmExtension::Result InitializeImagePicker(dmExtension::Params* params)
{
	// Init Lua
	ext_imagepicker::LuaInit(params->m_L);
	ext_imagepicker::Initialize();
	printf("Registered %s Extension\n", MODULE_NAME);
	return dmExtension::RESULT_OK;
}

dmExtension::Result AppFinalizeImagePicker(dmExtension::AppParams* params)
{
	return dmExtension::RESULT_OK;
}

dmExtension::Result FinalizeImagePicker(dmExtension::Params* params)
{
	return dmExtension::RESULT_OK;
}

DM_DECLARE_EXTENSION(ImagePicker, LIB_NAME, AppInitializeImagePicker, AppFinalizeImagePicker, InitializeImagePicker, 0, 0, FinalizeImagePicker)

#else

static  dmExtension::Result InitializeImagePicker(dmExtension::Params* params)
{
	dmLogInfo("Registered extension ImagePicker (null)");
	return dmExtension::RESULT_OK;
}

static dmExtension::Result FinalizeImagePicker(dmExtension::Params* params)
{
	return dmExtension::RESULT_OK;
}

DM_DECLARE_EXTENSION(ImagePicker, LIB_NAME, 0, 0, InitializeImagePicker, 0, 0, FinalizeImagePicker)

#endif