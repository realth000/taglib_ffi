#include "include/taglib_ffi/taglib_ffi_plugin_c_api.h"

#include <flutter/plugin_registrar_windows.h>

#include "taglib_ffi_plugin.h"

void TaglibFfiPluginCApiRegisterWithRegistrar(
    FlutterDesktopPluginRegistrarRef registrar) {
  taglib_ffi::TaglibFfiPlugin::RegisterWithRegistrar(
      flutter::PluginRegistrarManager::GetInstance()
          ->GetRegistrar<flutter::PluginRegistrarWindows>(registrar));
}
