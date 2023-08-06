import 'dart:ffi';
import 'dart:io';
import 'dart:isolate';

import 'package:ffi/ffi.dart';
import 'package:flutter/services.dart';

import 'taglib_ffi_bindings_generated.dart';

class Metadata {
  Metadata({
    required this.title,
    required this.artist,
    required this.album,
    required this.track,
    required this.year,
    required this.genre,
    required this.comment,
    required this.sampleRate,
    required this.bitrate,
    required this.channels,
    required this.length,
    this.albumArtist,
    this.albumTotalTrack,
    this.lyrics,
    this.albumCover,
  });

  final String? title;
  final String? artist;
  final String? album;
  final String? albumArtist;
  final int? track;
  final int? albumTotalTrack;
  final int? year;
  final String? genre;
  final String? comment;
  final int? sampleRate;
  final int? bitrate;
  final int? channels;
  final int? length;
  final String? lyrics;
  final Uint8List? albumCover;
}

class TagLib {
  TagLib({required this.filePath});

  final String filePath;

  Future<Metadata?> readMetadata() async {
    if (filePath.isEmpty) {
      return null;
    }
    final p = ReceivePort();
    await Isolate.spawn(_readMetadata, p.sendPort);
    return await p.first as Metadata?;
  }

  Future<dynamic> _readMetadata(SendPort p) async {
    Pointer<ID3v2Tag> originalTag = nullptr;
    late final TaglibFfiBindings meipuru;
    try {
      meipuru = TaglibFfiBindings(
        Platform.isWindows
            ? DynamicLibrary.open('taglib_ffi.dll')
            : DynamicLibrary.open('libtaglib_ffi.so'),
      );
      if (Platform.isWindows) {
        // Use wchar_t as parameter on Windows.
        // FIXME: Fix multiplatform compatibility.
        // final Pointer<WChar> tagFileName = filePath.toNativeUtf16().cast();
        // originalTag = meipuru.MeipuruReadID3v2TagW(tagFileName);
        // try {
        //   final locale = await Get.find<LocaleService>().getLocale();
        //   print('AAAA LOCALE: ${locale}');
        //   if (locale.languageCode == 'zh' && locale.countryCode == 'CN') {
        //     //if (locale != null && locale == 'zh_CN') {
        //     // tagFileName = filePath.toNativeGbk().cast();
        //     tagFileName = filePath.toNativeUtf8().cast();
        //   } else {
        //     tagFileName = filePath.toNativeUtf8().cast();
        //   }
        // } catch (e) {
        //   print('Error in _readMetadataEx getting tagFileName: $e');
        //   tagFileName = filePath.toNativeUtf8().cast();
        // }
      } else {
        final Pointer<Char> tagFileName = filePath.toNativeUtf8().cast();
        originalTag = meipuru.readID3v2Tag(tagFileName);
      }
      if (originalTag.address == nullptr.address) {
        print('FFI returned nullptr in meipuru.MeipuruReadID3v2Tag');
        return Isolate.exit(p);
      }

      final id3v2Tag = originalTag.cast<ID3v2Tag>().ref;
      final metaData = Metadata(
        title: id3v2Tag.title.cast<Utf8>().toDartString(),
        artist: id3v2Tag.artist.cast<Utf8>().toDartString(),
        album: id3v2Tag.albumTitle.cast<Utf8>().toDartString(),
        track: id3v2Tag.track,
        year: id3v2Tag.year,
        genre: id3v2Tag.genre.cast<Utf8>().toDartString(),
        comment: id3v2Tag.comment.cast<Utf8>().toDartString(),
        sampleRate: id3v2Tag.sampleRate,
        bitrate: id3v2Tag.bitRate,
        channels: id3v2Tag.channels,
        length: id3v2Tag.length,
        albumArtist: id3v2Tag.albumArtist.cast<Utf8>().toDartString(),
        albumTotalTrack: id3v2Tag.albumTotalTrack,
        lyrics: null,
        albumCover: id3v2Tag.albumCoverLength > 0
            ? id3v2Tag.albumCover
                .cast<Uint8>()
                .asTypedList(id3v2Tag.albumCoverLength)
            : null,
      );
      /*
      id3v2Tag.lyricsLength > 0
            ? id3v2Tag.lyrics
                .cast<Utf8>()
                .toDartString(length: id3v2Tag.lyricsLength)
            : null

       id3v2Tag.albumCoverLength > 0
            ? id3v2Tag.albumCover
                .cast<Uint8>()
                .asTypedList(id3v2Tag.albumCoverLength)
            : null
       */
      meipuru.freeID3v2Tag(originalTag);
      return Isolate.exit(p, metaData);
    } catch (e) {
      if (originalTag != nullptr) {
        meipuru.freeID3v2Tag(originalTag);
      }
      print('Error in readMetadataEx: $e');
      return Isolate.exit(p, null);
    }
  }
}
