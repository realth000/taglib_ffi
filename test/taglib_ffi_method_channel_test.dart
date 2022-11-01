import 'package:flutter/services.dart';
import 'package:flutter_test/flutter_test.dart';
import 'package:taglib_ffi/taglib_ffi_method_channel.dart';

void main() {
  MethodChannelTaglibFfi platform = MethodChannelTaglibFfi();
  const MethodChannel channel = MethodChannel('taglib_ffi');

  TestWidgetsFlutterBinding.ensureInitialized();

  setUp(() {
    channel.setMockMethodCallHandler((MethodCall methodCall) async {
      return '42';
    });
  });

  tearDown(() {
    channel.setMockMethodCallHandler(null);
  });

  test('getPlatformVersion', () async {
    expect(await platform.getPlatformVersion(), '42');
  });
}
