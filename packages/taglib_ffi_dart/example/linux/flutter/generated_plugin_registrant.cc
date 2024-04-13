//
//  Generated file. Do not edit.
//

// clang-format off

#include "generated_plugin_registrant.h"

#include <taglib_ffi_dart_libs/taglib_ffi_dart_libs_plugin.h>

void fl_register_plugins(FlPluginRegistry* registry) {
  g_autoptr(FlPluginRegistrar) taglib_ffi_dart_libs_registrar =
      fl_plugin_registry_get_registrar_for_plugin(registry, "TaglibFfiDartLibsPlugin");
  taglib_ffi_dart_libs_plugin_register_with_registrar(taglib_ffi_dart_libs_registrar);
}
