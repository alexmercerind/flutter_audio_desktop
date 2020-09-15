## 0.0.4

**Final Improvements**

- Now plugin uses MethodChannel instead of dart:ffi for calling native methods.
- Any additional setup is not required anymore.

## 0.0.3

**First Public Release**

- Added docstrings.
- Improved dart usage.
- Fixed wrong sample rate.
- AudioPlayer::load now checks for file existence.


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
