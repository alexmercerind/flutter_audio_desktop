# flutter_audio_desktop

### A simple yet functional :notes: audio library for Flutter Desktop.

Right now, as it is just a start, it supports MP3 playback. I'll try to increase the domain of support formats with time & plan is to provide metadata of a track aswell.


```bash
git clone http://github.com/alexmercerind/flutter_audio_desktop.git --depth=1

cd flutter_audio_desktop

flutter pub get

cd example

flutter run
```

Feel free to open issue anytime.


## :triangular_ruler: Usage

**For usage in your Flutter Desktop app, checkout [this](https://github.com/alexmercerind/flutter_audio_desktop/blob/master/lib/src/main.dart) simple implementation.**

```dart
// Start AudioPlayer.
var audioPlayer = new AudioPlayer();

// Load audio file.
audioPlayer.load('/home/alexmercerind/music.mp3');

// Start playing loaded audio file.
audioPlayer.play();
print('Duration Of Track: ${audioPlayer.getDuration()}');

// Change playback volume.
audioPlayer.setVolume(0.5);
print('Changed volume to 50%.');

// Change playback position.
audioPlayer.setPosition(Duration(seconds: 10));
print('Position of playback after skipping 60 seconds: ${audioPlayer.getPosition()}');

Timer(Duration(seconds: 10), () {

    // Pause the playback.
    audioPlayer.pause();
    print('Playback of audio stopped after 10 seconds.');
}
```


## :heart: Like the library?

Feel free to use in your Flutter Desktop app. Consider :star: starring the repository if you want to show YOUR SUPPORT to the development & appreciate the effort.

![flutter_audio_desktop](https://github.com/alexmercerind/flutter_audio_desktop/blob/master/screenshot.png)

## :heavy_check_mark: Progress

The library is only supported on x64 systems right now.

|Platform|Status    |Remark     |
|--------|----------|-----------|
|Linux   |Working   |           |
|Windows |Not Tested|Coming Soon|
|MacOS   |Not Tested|           |

## :wrench: How It Works ?

One word, C++. I had bit of experience with C++ & it simply uses [dart::ffi](https://dart.dev/guides/libraries/c-interop) for accessing Native C++ for playing audio. It uses the native code to use [miniaudio](https://github.com/mackron/miniaudio) from [David Reid](https://github.com/mackron). 

I wrote [this](https://github.com/alexmercerind/flutter_audio_desktop/blob/master/lib/src/AudioPlayer.hpp) simple wrapper around his library to get this working.

There is not any audio playback library for Flutter Desktop at the moment, so I decided to write one myself.

###### :love_letter: Thanks to [David Reid](https://github.com/mackron).
