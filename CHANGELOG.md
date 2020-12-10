## 0.0.8

**Multiple player instances, wave & noise methods**

- Now multiple AudioPlayer instances can be made, by providing optional id parameter to the constructor.
- Added methods for playing waves.
- Added methods for playing noise.

## 0.0.7

**Initial Playback Device Changing Support**

- Added setDevice method to AudioPlayer class.

## 0.0.6

**Microsoft Windows Support**

- Plugin is now capable of playing audio files on Windows.

## 0.0.5

**A Little Fix**

- pub package now has miniaudio in it.

## 0.0.4

**Final Improvements**

- Now plugin uses MethodChannel instead of dart:ffi for calling native methods.
- Any additional setup is not required anymore.

## 0.0.3

**First Public Release**

- Added docstrings.
- Improved dart usage.
- Fixed wrong sample rate.
- Now Dart code is asynchronous.


## 0.0.2

**Now Fully Open Source**

Changed native code to use [miniaudio](https://github.com/mackron/miniaudio)


## 0.0.1

**Initial Release**

Supports audio playback on Linux.

Added mandatory audio playback functions like:
- Loading audio file
- Playing
- Pausing
- Getting duration of an audio file.
- Seeking
- Changing volume
