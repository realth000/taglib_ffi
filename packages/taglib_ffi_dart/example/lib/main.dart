import 'dart:async';

import 'package:file_picker/file_picker.dart';
import 'package:flutter/material.dart';
import 'package:flutter/services.dart';
import 'package:taglib_ffi_dart/taglib_ffi_dart.dart' as taglib;

Future<void> main() async {
  await taglib.initialize();
  runApp(const MyApp());
}

class MyApp extends StatefulWidget {
  const MyApp({super.key});

  @override
  State<MyApp> createState() => _MyAppState();
}

class _MyAppState extends State<MyApp> with SingleTickerProviderStateMixin {
  final formKey = GlobalKey<FormState>();
  final titleC = TextEditingController();
  final artistC = TextEditingController();
  final albumC = TextEditingController();
  final trackC = TextEditingController();
  final yearC = TextEditingController();
  final genreC = TextEditingController();
  final commentC = TextEditingController();
  final sampleRateC = TextEditingController();
  final bitrateC = TextEditingController();
  final channelsC = TextEditingController();
  final lengthC = TextEditingController();
  final lyricsC = TextEditingController();
  Uint8List? albumCoverData;

  late final TabController tabController;

  bool loadingFromDir = false;

  List<taglib.Metadata> allMetadata = [];

  @override
  void initState() {
    super.initState();
    tabController = TabController(length: 2, vsync: this);
  }

  @override
  void dispose() {
    tabController.dispose();
    super.dispose();
  }

  // Platform messages are asynchronous, so we initialize in an async method.
  Future<void> readMetadataFromFile() async {
    FilePickerResult? result = await FilePicker.platform.pickFiles();
    if (result == null) {
      return;
    }

    final filePath = result.files.single.path;
    if (filePath == null) {
      return;
    }

    // Get Metadata.
    taglib.Metadata? metaData;
    try {
      // metaData = await TagLib(filePath: filePath).readMetadata();
      metaData = await taglib.readMetadata(filePath);
    } on PlatformException {
      if (!mounted) return;
      ScaffoldMessenger.of(context).showSnackBar(
        SnackBar(
          content: Text('Failed to read metadata: $PlatformException'),
        ),
      );
      return;
    }

    setState(() {
      if (metaData == null) {
        return;
      }
      titleC.text = metaData.title ?? '';
      artistC.text = metaData.artist ?? '';
      albumC.text = metaData.album ?? '';
      trackC.text = metaData.track == 0 ? '' : metaData.track.toString();
      yearC.text = metaData.year == 0 ? '' : metaData.year.toString();
      genreC.text = metaData.genre ?? '';
      commentC.text = metaData.comment ?? '';
      sampleRateC.text =
          metaData.sampleRate == 0 ? '' : metaData.sampleRate.toString();
      bitrateC.text = metaData.bitrate == 0 ? '' : metaData.bitrate.toString();
      channelsC.text =
          metaData.channels == 0 ? '' : metaData.channels.toString();
      lengthC.text = metaData.length == 0 ? '' : metaData.length.toString();
      lyricsC.text = metaData.lyrics ?? '';
      albumCoverData = metaData.albumCover;
    });
  }

  Future<void> readMetadataFromDir() async {
    final result = await FilePicker.platform.getDirectoryPath();
    if (result == null) {
      debugPrint('dir path not selected');
      return;
    }

    final start = DateTime.now();
    debugPrint('scanning metadata in $result');
    setState(() {
      loadingFromDir = true;
    });

    final metadataList = await taglib.readMetadataFromDir(result);
    if (metadataList == null) {
      debugPrint('empty metadata list');
      return;
    }

    setState(() {
      allMetadata = metadataList;
      loadingFromDir = false;
    });
    debugPrint('metadata: length is ${allMetadata.length}');
    final end = DateTime.now();
    debugPrint('elapsed: ${end.difference(start)}');
  }

  Widget _buildFileTab(BuildContext context) {
    return
        // Single file default metadata page.
        Scrollbar(
      child: SingleChildScrollView(
        primary: true,
        child: Padding(
          padding: const EdgeInsets.all(15),
          child: Form(
            key: formKey,
            child: Column(
              mainAxisAlignment: MainAxisAlignment.spaceBetween,
              children: <Widget>[
                if (albumCoverData != null) Image.memory(albumCoverData!),
                TextFormField(
                  autofocus: false,
                  controller: titleC,
                  decoration: const InputDecoration(
                    labelText: 'Title',
                  ),
                ),
                TextFormField(
                  autofocus: false,
                  controller: artistC,
                  decoration: const InputDecoration(
                    labelText: 'Artist',
                  ),
                ),
                TextFormField(
                  autofocus: false,
                  controller: albumC,
                  decoration: const InputDecoration(
                    labelText: 'Album',
                  ),
                ),
                TextFormField(
                  autofocus: false,
                  controller: trackC,
                  decoration: const InputDecoration(
                    labelText: 'Track',
                  ),
                ),
                TextFormField(
                  autofocus: false,
                  controller: yearC,
                  decoration: const InputDecoration(
                    labelText: 'Year',
                  ),
                ),
                TextFormField(
                  autofocus: false,
                  controller: genreC,
                  decoration: const InputDecoration(
                    labelText: 'Genre',
                  ),
                ),
                TextFormField(
                  autofocus: false,
                  controller: commentC,
                  decoration: const InputDecoration(
                    labelText: 'Comment',
                  ),
                  minLines: 1,
                  maxLines: 10,
                ),
                TextFormField(
                  autofocus: false,
                  controller: sampleRateC,
                  decoration: const InputDecoration(
                    labelText: 'Sample Rate (kHz)',
                  ),
                ),
                TextFormField(
                  autofocus: false,
                  controller: bitrateC,
                  decoration: const InputDecoration(
                    labelText: 'Bitrate (kbps)',
                  ),
                ),
                TextFormField(
                  autofocus: false,
                  controller: channelsC,
                  decoration: const InputDecoration(
                    labelText: 'Channels',
                  ),
                ),
                TextFormField(
                  autofocus: false,
                  controller: lengthC,
                  decoration: const InputDecoration(
                    labelText: 'Length (Seconds)',
                  ),
                ),
                TextFormField(
                  autofocus: false,
                  controller: lyricsC,
                  decoration: const InputDecoration(
                    labelText: 'Lyrics',
                  ),
                  minLines: 1,
                  maxLines: 20,
                )
              ],
            ),
          ),
        ),
      ),
    );
  }

  Widget _buildDirTab(BuildContext context) {
    if (loadingFromDir) {
      return const Center(child: CircularProgressIndicator());
    }
    return ListView.builder(
      itemCount: allMetadata.length,
      itemBuilder: (context, index) {
        final item = allMetadata[index];
        return ListTile(
          leading:
              item.albumCover != null ? Image.memory(item.albumCover!) : null,
          title: Text(item.title ?? ''),
          subtitle: Text('${item.artist ?? ""} : ${item.album ?? ""}'),
          trailing: Text('$index'),
        );
      },
    );
  }

  @override
  Widget build(BuildContext context) {
    return MaterialApp(
      home: Scaffold(
        appBar: AppBar(
          title: const Text('taglib_ffi_dart Example App'),
          bottom: TabBar(
            controller: tabController,
            tabs: const [
              Tab(child: Text('File')),
              Tab(child: Text('Dir')),
            ],
          ),
        ),
        body: TabBarView(
          controller: tabController,
          children: [
            _buildFileTab(context),
            _buildDirTab(context),
          ],
        ),
        floatingActionButton: FloatingActionButton(
          child: const Icon(Icons.add),
          onPressed: () async {
            if (tabController.index == 0) {
              await readMetadataFromFile();
            } else if (tabController.index == 1) {
              await readMetadataFromDir();
            } else {
              throw UnimplementedError();
            }
          },
        ),
      ),
    );
  }
}
