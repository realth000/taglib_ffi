import 'dart:ffi';

class TagLib {
  static Future<String> getTitle(String path) async {
    final DynamicLibrary tagLibNative = DynamicLibrary.open('libtag_c.so');
    tagLibNative
        .lookup<NativeFunction<Struct Function(Pointer<SignedChar> name)>>(
            'taglib_file_new');
    return 'aa';
  }
}
