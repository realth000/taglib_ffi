import 'dart:async';
import 'dart:ffi';

import 'package:ffi/ffi.dart';
import 'package:file_picker/file_picker.dart';
import 'package:flutter/material.dart';
import 'package:flutter/services.dart';
import 'package:taglib_ffi/taglib_ffi.dart';

void main() {
  runApp(const MyApp());
}

class MyApp extends StatefulWidget {
  const MyApp({super.key});

  @override
  State<MyApp> createState() => _MyAppState();
}

class _MyAppState extends State<MyApp> {
  String _platformVersion = 'Unknown';
//  final _taglibFfiPlugin = TaglibFfi();

  @override
  void initState() {
    super.initState();
    // initPlatformState();
  }

  // Platform messages are asynchronous, so we initialize in an async method.
  Future<void> initPlatformState() async {
    String platformVersion;

    FilePickerResult? result = await FilePicker.platform.pickFiles();
    if (result == null) {
      return;
    }

    final filePath = result.files.single.path;

    // Platform messages may fail, so we use a try/catch PlatformException.
    // We also handle the message potentially returning null.
    try {
      final tagLib = NativeLibrary(DynamicLibrary.open('libtag_c.so'));
      final tagFileName = filePath!.toNativeUtf8();
      final tagFile = tagLib.taglib_file_new(tagFileName.cast<Char>());
      final tagFileTag = tagLib.taglib_file_tag(tagFile);
      final tagFileTagTitle = tagLib.taglib_tag_title(tagFileTag);
      platformVersion = tagFileTagTitle.cast<Utf8>().toDartString();
    } on PlatformException {
      platformVersion = 'Failed to get platform version.';
    }

    // If the widget was removed from the tree while the asynchronous platform
    // message was in flight, we want to discard the reply rather than calling
    // setState to update our non-existent appearance.
    if (!mounted) return;

    setState(() {
      _platformVersion = platformVersion;
    });
  }

  @override
  Widget build(BuildContext context) {
    return MaterialApp(
      home: Scaffold(
        appBar: AppBar(
          title: const Text('Plugin example app'),
        ),
        body: Center(
          child: Text('Running on: $_platformVersion\n'),
        ),
        floatingActionButton: FloatingActionButton(
          child: const Icon(Icons.add),
          onPressed: () async {
            await initPlatformState();
          },
        ),
      ),
    );
  }
}
