# taglib_ffi

Use [TagLib](https://taglib.org/) to read audio metadata tags in flutter.

## WIP

taglib_ffi is currently work in progress.

* Only support reading metadata.
* Performance is not good as expected, maybe extreamly slow with large mount of audios.
* API may change in future, but it should be easy to migrate.

## Usage

``` dart

import 'package:taglib_ffi/taglib_ffi.dart';

final filePath = 'path/to/audio/file';

final metadata = await TagLib(filePath: filePath).readMetadata();

```

where `metadata` is:

``` dart

class Metadata {
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

```

## Features

* [x](Read metadata)
* [ ](Write metadata)
* [ ](Support multiple album covers with named position)
* [ ](Better performance)
* [ ](All other fields that taglib can do)

~~What if we have a cross platform foobar2000...~~

## Build From Source

* For Android, bundling `*.so` can by run `./scripts/build_android.sh`.
  * After that, `libtag.so` and `libtaglib_ffi.so` are automatically bundled with app.
  * Finally run `flutter build apk`.
  * To build `*.so`, NDK and `${ANDROID_HOME}` variable are required.
* For Windows and Linux, just run `flutter build windows` or `flutter build linux`.
