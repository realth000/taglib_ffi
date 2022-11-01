#ifndef FLUTTER_PLUGIN_TAGLIB_FFI_PLUGIN_H_
#define FLUTTER_PLUGIN_TAGLIB_FFI_PLUGIN_H_

#include <flutter/method_channel.h>
#include <flutter/plugin_registrar_windows.h>

#include <memory>

namespace taglib_ffi {

class TaglibFfiPlugin : public flutter::Plugin {
 public:
  static void RegisterWithRegistrar(flutter::PluginRegistrarWindows *registrar);

  TaglibFfiPlugin();

  virtual ~TaglibFfiPlugin();

  // Disallow copy and assign.
  TaglibFfiPlugin(const TaglibFfiPlugin&) = delete;
  TaglibFfiPlugin& operator=(const TaglibFfiPlugin&) = delete;

 private:
  // Called when a method is called on this plugin's channel from Dart.
  void HandleMethodCall(
      const flutter::MethodCall<flutter::EncodableValue> &method_call,
      std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result);
};

}  // namespace taglib_ffi

#endif  // FLUTTER_PLUGIN_TAGLIB_FFI_PLUGIN_H_
