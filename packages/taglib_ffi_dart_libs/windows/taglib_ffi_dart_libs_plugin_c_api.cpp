#include "include/taglib_ffi_dart_libs/taglib_ffi_dart_libs_plugin_c_api.h"

#include <flutter/plugin_registrar_windows.h>

#include "taglib_ffi_dart_libs_plugin.h"

void TaglibFfiDartLibsPluginCApiRegisterWithRegistrar(
    FlutterDesktopPluginRegistrarRef registrar) {
  taglib_ffi_dart_libs::TaglibFfiDartLibsPlugin::RegisterWithRegistrar(
      flutter::PluginRegistrarManager::GetInstance()
          ->GetRegistrar<flutter::PluginRegistrarWindows>(registrar));
}
