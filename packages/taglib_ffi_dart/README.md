# taglib_ffi

Use [TagLib](https://taglib.org/) to manage audio metadata tags in dart/flutter.

## WIP

taglib_ffi is currently work in progress. API may change in the future.

## Usage

``` dart

import 'package:taglib_ffi_dart/taglib_ffi_dart.dart' as taglib;

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

### Platform support

* [x] Android
* [ ] iOS
* [x] Linux
* [ ] macOS
* [ ] Windows (coming soon)

> I don't have Apple device, feel free to PR if you would like to help for this!

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

### Install

Add `taglib_ffi_dart` and `taglib_ffi_dart_libs` to `pubspec.yaml` as dependencies.

Use packages hosted on pub:

```yaml
dependencies:
  taglib_ffi_dart: current_version
  taglib_ffi_dart_libs: current_version
```

## Build from source

### All

All platforms should generate the binding dart code first.
```bash
dart pub get
dart run ffigen --config ffigen.yaml
```

* Set llvm directory in `ffigen.yaml` if llvm is not found on Windows.
  ```yaml
  # For Windows:
  # llvm-path:
  #   - 'D:\PATH\TO\LLVM'
  ```
* Run the following command if system header is not found on Linux.
  * `export CPATH="$(clang -v 2>&1 | grep "Selected GCC installation" | rev | cut -d' ' -f1 | rev)/include"`
  * See [dart-lang issue](https://github.com/dart-lang/native/issues/338#issuecomment-1813390726)

### Android

```bash
# Export env, SDK and NDK are required.
export ANDROID_HOME=/path/to/Android/SDK

cd packages

# Build and save libs
./taglib_ffi_dart/scripts/build_android.sh ./taglib_ffi_dart_libs/android/src/main/jniLibs
```

After that, libs are automatically bundled with app.

### Linux

```bash
./taglib_ffi_dart/scripts/build_linux.sh ./taglib_ffi_dart_libs/linux
```

After that, libs are automatically bundled with app.

### Windows

TODO
