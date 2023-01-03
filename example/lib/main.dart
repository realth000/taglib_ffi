import 'dart:async';

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

  @override
  void initState() {
    super.initState();
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
    late final Metadata? metaData;
    try {
      // metaData = await TagLib(filePath: filePath).readMetadata();
      metaData = await TagLib(filePath: filePath).readMetadataEx();
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

  @override
  Widget build(BuildContext context) {
    return MaterialApp(
      home: Scaffold(
        appBar: AppBar(
          title: const Text('taglib_ffi Example App'),
        ),
        body: Scrollbar(
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
        ),
        floatingActionButton: FloatingActionButton(
          child: const Icon(Icons.add),
          onPressed: () async {
            await readMetadataFromFile();
          },
        ),
      ),
    );
  }
}
