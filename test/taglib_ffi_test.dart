import 'package:flutter_test/flutter_test.dart';
import 'package:taglib_ffi/taglib_ffi.dart';
import 'package:taglib_ffi/taglib_ffi_platform_interface.dart';
import 'package:taglib_ffi/taglib_ffi_method_channel.dart';
import 'package:plugin_platform_interface/plugin_platform_interface.dart';

class MockTaglibFfiPlatform
    with MockPlatformInterfaceMixin
    implements TaglibFfiPlatform {

  @override
  Future<String?> getPlatformVersion() => Future.value('42');
}

void main() {
  final TaglibFfiPlatform initialPlatform = TaglibFfiPlatform.instance;

  test('$MethodChannelTaglibFfi is the default instance', () {
    expect(initialPlatform, isInstanceOf<MethodChannelTaglibFfi>());
  });

  test('getPlatformVersion', () async {
    TaglibFfi taglibFfiPlugin = TaglibFfi();
    MockTaglibFfiPlatform fakePlatform = MockTaglibFfiPlatform();
    TaglibFfiPlatform.instance = fakePlatform;

    expect(await taglibFfiPlugin.getPlatformVersion(), '42');
  });
}
