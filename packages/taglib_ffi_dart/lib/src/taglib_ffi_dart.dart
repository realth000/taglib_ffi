import 'dart:async';
import 'dart:ffi';
import 'dart:io';
import 'dart:typed_data';

import 'package:ffi/ffi.dart';
import 'package:isolate_pool_2/isolate_pool_2.dart';

import 'taglib_ffi_bindings_generated.dart';

/// Audio metadata data class.
///
/// Holding all metadata types and values.
class Metadata {
  /// Constructor.
  Metadata({
    required this.filePath,
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
    required this.lengthInSeconds,
    required this.lengthInMilliseconds,
    this.albumArtist,
    this.albumTotalTrack,
    this.lyrics,
    this.albumCover,
  });

  final String filePath;
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
  final int? lengthInSeconds;
  final int? lengthInMilliseconds;
  final String? lyrics;
  final Uint8List? albumCover;
}

late final _TagLib _taglib;

var _initialized = false;

/// Initialize taglib_ffi_dart package.
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
  _ReadMetadataJob(this.filePath, this.readImage);

  final String filePath;

  final bool readImage;

  @override
  Future<Metadata?> job() async {
    if (filePath.isEmpty) {
      return null;
    }
    return _readMetadata(filePath, readImage);
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

  Future<Metadata?> readMetadata(
    String filePath, {
    bool readImage = true,
  }) async {
    return pool.scheduleJob<Metadata>(_ReadMetadataJob(filePath, readImage));
  }

  Future<List<Metadata>?> readMetadataFromDir(
    String dirPath, {
    bool readImage = true,
  }) async {
    final dir = Directory(dirPath);
    if (!dir.existsSync()) {
      return [];
    }

    final tasks = await dir
        .list(recursive: true)
        .where((e) => e.path.endsWith('.mp3'))
        .map((e) async =>
            pool.scheduleJob<Metadata>(_ReadMetadataJob(e.path, readImage)))
        .toList();
    final data = Future.wait(tasks);

    return data;
  }

  Stream<Future<Metadata>> readMetadataStreamFromDir(
    String dirPath,
    bool readImage,
  ) async* {
    final dir = Directory(dirPath);
    if (!dir.existsSync()) {
      return;
    }
    yield* dir.list(recursive: true).where((e) => e.path.endsWith('.mp3')).map(
          (e) async => pool.scheduleJob<Metadata>(
            _ReadMetadataJob(
              e.path,
              readImage,
            ),
          ),
        );
  }
}

/// Read auto metadata from given [filePath].
///
/// Return null if file not found or failed to load data.
Future<Metadata?> readMetadata(String filePath) async {
  return _taglib.readMetadata(filePath);
}

Future<List<Metadata>?> readMetadataFromDir(String dirPath) async {
  return _taglib.readMetadataFromDir(dirPath);
}

Stream<Future<Metadata>> readMetadataStreamFromDir(
  String dirPath, {
  bool readImage = true,
}) async* {
  yield* _taglib.readMetadataStreamFromDir(dirPath, readImage);
}

Future<Metadata?> _readMetadata(String filePath, bool readImage) async {
  Pointer<ID3v2Tag> originalTag = nullptr;
  late final TaglibFfiBindings meipuru;
  try {
    meipuru = TaglibFfiBindings(
      Platform.isWindows
          ? DynamicLibrary.open('libtag_ffi.dll')
          : DynamicLibrary.open('libtag_ffi.so'),
    );
    final Pointer<Char> tagFileName = filePath.toNativeUtf8().cast();
    originalTag = meipuru.readID3v2Tag(tagFileName, readImage);
    if (originalTag == nullptr) {
      print('FFI returned nullptr in meipuru.MeipuruReadID3v2Tag');
      return null;
    }

    final id3v2Tag = originalTag.ref;
    final metaData = Metadata(
      filePath: filePath,
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
      lengthInSeconds: id3v2Tag.lengthInSeconds,
      lengthInMilliseconds: id3v2Tag.lengthInMilliSeconds,
      albumArtist: id3v2Tag.albumArtist.cast<Utf8>().toDartString(),
      albumTotalTrack: id3v2Tag.albumTotalTrack,
      lyrics: null,
      albumCover: id3v2Tag.albumCoverLength > 0 &&
              id3v2Tag.albumCover != nullptr
          // As mentioned in `asTypedList`'s comment, the created `Uint8List`
          // is only a view of the background data. Caller need to ensure the
          // data behind is available when using the converted `Uint8List`
          // instance.
          //
          // But we want to free the c-side memory immediately after finish
          // reading the data.
          // So copy a list here to let us free the backend data asap.
          ? Uint8List.fromList(
              id3v2Tag.albumCover.asTypedList(id3v2Tag.albumCoverLength))
          : null,
    );
    meipuru.freeID3v2Tag(originalTag);
    return metaData;
  } catch (e) {
    if (originalTag != nullptr) {
      meipuru.freeID3v2Tag(originalTag);
    }
    print('Error in readMetadataEx: $e');
    return null;
  }
}
