import 'dart:ffi';

import 'package:ffi/ffi.dart';

import 'taglib_bindings.dart';

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
  });

  final String? title;
  final String? artist;
  final String? album;
  final int? track;
  final int? year;
  final String? genre;
  final String? comment;
  final int? sampleRate;
  final int? bitrate;
  final int? channels;
  final int? length;
}

class TagLib {
  /// Read [Metadata] from file.
  static Future<Metadata?> readMetadata({required String filePath}) async {
    if (filePath.isEmpty) {
      return null;
    }
    final tagLib = NativeLibrary(DynamicLibrary.open('libtag_c.so'));
    final tagFileName = filePath.toNativeUtf8();
    final tagFile = tagLib.taglib_file_new(tagFileName.cast<Char>());
    final tagFileTag = tagLib.taglib_file_tag(tagFile);

    // Read common properties.
    // Read Title.
    final tagFileTagTitle = tagLib.taglib_tag_title(tagFileTag);
    // Read Artist.
    final tagFileTagArtist = tagLib.taglib_tag_artist(tagFileTag);
    // Read AlbumTitle.
    final tagFileTagAlbum = tagLib.taglib_tag_album(tagFileTag);
    // Read Track.
    final tagFileTagTrack = tagLib.taglib_tag_track(tagFileTag);
    // Read Year.
    final tagFileTagYear = tagLib.taglib_tag_year(tagFileTag);
    // Read Genre.
    final tagFileTagGenre = tagLib.taglib_tag_genre(tagFileTag);
    // Read Comment
    final tagFileTagComment = tagLib.taglib_tag_comment(tagFileTag);

    // Read audio properties.
    final tagProp = tagLib.taglib_file_audioproperties(tagFile);
    // Read SampleRate;
    final tagPropSampleRate = tagLib.taglib_audioproperties_samplerate(tagProp);
    // Read Bitrate.
    final tagPropBitrate = tagLib.taglib_audioproperties_bitrate(tagProp);
    // Read Channels.
    final tagPropChannels = tagLib.taglib_audioproperties_channels(tagProp);
    // Read Length.
    final tagPropLength = tagLib.taglib_audioproperties_length(tagProp);
    return Metadata(
      title: tagFileTagTitle.cast<Utf8>().toDartString(),
      artist: tagFileTagArtist.cast<Utf8>().toDartString(),
      album: tagFileTagAlbum.cast<Utf8>().toDartString(),
      track: tagFileTagTrack,
      year: tagFileTagYear,
      genre: tagFileTagGenre.cast<Utf8>().toDartString(),
      comment: tagFileTagComment.cast<Utf8>().toDartString(),
      sampleRate: tagPropSampleRate,
      bitrate: tagPropBitrate,
      channels: tagPropChannels,
      length: tagPropLength,
    );
  }
}
