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
# Export env, NDK is required.
export ANDROID_HOME=/path/to/Android/SDK

# Build *.so and save somewhere.
./scripts/build_android.sh /path/to/save/built/libs
```

After running `build_android.sh`, all `*.so` are saved in:

```
/your_save_directory
├── arm64-v8a
│   ├── libtaglib_ffi.so
│   └── libtag.so
├── armeabi-v7a
│   ├── libtaglib_ffi.so
│   └── libtag.so
├── x86
│   ├── libtaglib_ffi.so
│   └── libtag.so
└── x86_64
    ├── libtaglib_ffi.so
    └── libtag.so
```

Copy the save directory to the app's `android/src/main/jniLibs` directory.

### Linux

```
# After add to pubspec.yaml
flutter build linux
```

### Windows

```
# After add to pubspec.yaml
flutter build windows
```

* For Windows and Linux, just run `flutter build windows` or `flutter build linux`.
* For Windows
* For Linux, when failed to find header files, export the variable:
