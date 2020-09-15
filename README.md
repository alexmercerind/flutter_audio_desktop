# [flutter_audio_desktop](https://github.com/alexmercerind/flutter_audio_desktop)

#### A simple yet functional :notes: audio library for Flutter Desktop.

Right now, as it is just a start, it supports MP3 playback. I'll try to increase the domain of support formats with time & plan is to provide metadata of a track aswell.

Feel free to open issue anytime.


## :arrow_down: Install

Mention in your pubspec.yaml requirements:

```yaml
dependencies:
  ...
  flutter_audio_desktop: ^0.0.5
```


## :triangular_ruler: Usage

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
print('Position of playback after skipping 10 seconds: ${audioPlayer.getPosition()}');

Timer(Duration(seconds: 10), () {

    // Pause the playback.
    audioPlayer.pause();
    print('Playback of audio stopped after 10 seconds.');
}
```

You can see [this](https://github.com/alexmercerind/flutter_audio_desktop/blob/master/example/lib/main.dart) simple example app, if you are confused with the usage.


## :heart: Like the library?

Feel free to use in your Flutter Desktop app. Consider :star: starring the repository if you want to show YOUR SUPPORT to the development & appreciate the effort.

## :heavy_check_mark: Progress

The library is only supported on Linux right now.

|Platform|Status     |
|--------|-----------|
|Linux   |Working    |
|Windows |Not Working|
|MacOS   |Not Working|

## :wrench: How It Works ?

One word, C++. It calls native methods using dart's MethodChannel. It uses [miniaudio](https://github.com/mackron/miniaudio) from [David Reid](https://github.com/mackron). 

I wrote [this](https://github.com/alexmercerind/flutter_audio_desktop/blob/master/linux/include/com.alexmercerind/AudioPlayer.hpp) simple wrapper in C++ around his library to get this working.

There is not any audio playback library for Flutter Desktop at the moment, so I decided to write one myself.

###### :love_letter: Thanks to [David Reid](https://github.com/mackron).
