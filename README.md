# taglib_ffi

Use [TagLib](https://taglib.org/) to manage audio metadata tags in dart/flutter.

## WIP

taglib_ffi is currently work in progress. API may change in the future.

## Usage

``` dart

import 'package:taglib_ffi/taglib_ffi.dart' as taglib;

final filePath = 'path/to/audio/file';

await taglib.initialize();

final metadata = await taglib.readMetadata(filePath);

```

**For Android platform,**

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

### Format support

| Format  | Tag Type       | Read | Write |
|---------|----------------|------|-------|
| mp3     | Id3v2          |  ✔️  |       |
| flac    | Vorbis comment |      |       |
| ape     | APEv2          |      |       |
| wma     | ASF            |      |       |
| dsd/dsf | Id3v2          |      |       |
| wav     | RIFF           |      |       |

### Tag support

* [x] Title
* [x] Artist
* [x] Album
* [x] Album artist
* [x] Track number
* [x] Album track number
* [x] Year
* [x] Genre
* [x] Comment
* [x] Sample rate
* [x] Bitrate
* [x] Channels
* [x] Duration
* [x] Lyrics
* [x] Album cover image
  * [ ] Multiple images with type
* [ ] Custom tags

### Deploy

* [x] Auto bundle on Windows/Linux.
  * Add taglib_ffi to `pubspec.yaml` then `*.so`/`*.dll`s are automatically bundled with your app.
* [x] Manually build and bundle on Android.
  * See [Build from source](#Build-from-source).
* [ ] Auto bundle on Android.

## Build from source

* For Android, bundle `*.so`s by run `./scripts/build_android.sh`.
  * After that, `libtag.so` and `libtaglib_ffi.so` are automatically bundled with app.
  * Finally, run `flutter build apk`.
  * To build `*.so`, Android NDK and `${ANDROID_HOME}` variable are required.
* For Windows and Linux, just run `flutter build windows` or `flutter build linux`.
* For Linux, when failed to find header files, export the variable:
  * `export CPATH="$(clang -v 2>&1 | grep "Selected GCC installation" | rev | cut -d' ' -f1 | rev)/include"`
  * See [dart-lang issue](https://github.com/dart-lang/native/issues/338#issuecomment-1813390726)
