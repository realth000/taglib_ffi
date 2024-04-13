#ifndef FLUTTER_PLUGIN_TAGLIB_FFI_DART_LIBS_PLUGIN_H_
#define FLUTTER_PLUGIN_TAGLIB_FFI_DART_LIBS_PLUGIN_H_

#include <flutter/method_channel.h>
#include <flutter/plugin_registrar_windows.h>

#include <memory>

namespace taglib_ffi_dart_libs {

class TaglibFfiDartLibsPlugin : public flutter::Plugin {
 public:
  static void RegisterWithRegistrar(flutter::PluginRegistrarWindows *registrar);

  TaglibFfiDartLibsPlugin();

  virtual ~TaglibFfiDartLibsPlugin();

  // Disallow copy and assign.
  TaglibFfiDartLibsPlugin(const TaglibFfiDartLibsPlugin&) = delete;
  TaglibFfiDartLibsPlugin& operator=(const TaglibFfiDartLibsPlugin&) = delete;

  // Called when a method is called on this plugin's channel from Dart.
  void HandleMethodCall(
      const flutter::MethodCall<flutter::EncodableValue> &method_call,
      std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result);
};

}  // namespace taglib_ffi_dart_libs

#endif  // FLUTTER_PLUGIN_TAGLIB_FFI_DART_LIBS_PLUGIN_H_
