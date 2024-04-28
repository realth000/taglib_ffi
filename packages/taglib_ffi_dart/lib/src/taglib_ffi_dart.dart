import 'dart:async';
import 'dart:ffi';
import 'dart:io';
import 'dart:isolate';
import 'dart:typed_data';

import 'package:ffi/ffi.dart';
import 'package:isolate_pool_2/isolate_pool_2.dart';

import 'taglib_ffi_bindings_generated.dart';

/// Audio metada data class.
///
/// Holding all metadata types and values.
class Metadata {
  /// Constructor.
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

late final _TagLib _taglib;

var _initialized = false;

/// Initialize talibg_ffi_dart package.
Future<void> initialize({int? isolateCount}) async {
  if (_initialized) {
    return;
  }
  _taglib = _TagLib(isolateCount);
  await _taglib.init();
  _initialized = true;
}

void checkInitialization() {
  if (!_initialized) {
    throw Exception(
        'TagLibFfi: Need to call "TagLib.initialize()" before calling TagLibFfi APIs');
  }
}

class _ReadMetadataJob extends PooledJob<Metadata?> {
  _ReadMetadataJob(this.filePath);

  final String filePath;

  @override
  Future<Metadata?> job() async {
    if (filePath.isEmpty) {
      return null;
    }
    final p = ReceivePort();
    Isolate.spawn(_readMetadata, <dynamic>[p.sendPort, filePath]);
    return await p.first;
  }
}

class _TagLib {
  _TagLib([int? isolateCount]) {
    this.isolateCount = isolateCount ?? 1;
    pool = IsolatePool(this.isolateCount);
  }

  Future<void> init() async {
    await pool.start();
  }

  late final int isolateCount;
  late final IsolatePool pool;

  Future<Metadata?> readMetadata(String filePath) async {
    return pool.scheduleJob(_ReadMetadataJob(filePath));
  }
}

/// Read auto metadata from given [filePath].
///
/// Return null if file not found or failed to load data.
Future<Metadata?> readMetadata(String filePath) async {
  return _taglib.readMetadata(filePath);
}

Future<dynamic> _readMetadata(List<dynamic> params) async {
  final p = params[0] as SendPort;
  final filePath = params[1] as String;
  Pointer<ID3v2Tag> originalTag = nullptr;
  late final TaglibFfiBindings meipuru;
  try {
    meipuru = TaglibFfiBindings(
      Platform.isWindows
          ? DynamicLibrary.open('taglib_ffi.dll')
          : DynamicLibrary.open('libtaglib_ffi.so'),
    );
    final Pointer<Char> tagFileName = filePath.toNativeUtf8().cast();
    originalTag = meipuru.readID3v2Tag(tagFileName);
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
