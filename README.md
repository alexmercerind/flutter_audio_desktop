# [flutter_audio_desktop](https://github.com/alexmercerind/flutter_audio_desktop)

#### A :musical_note: audio playback library for Flutter Desktop.

Feel free to open issue anytime.


## :arrow_down: Installing

Mention in your pubspec.yaml requirements:

```yaml
dependencies:
  ...
  flutter_audio_desktop: ^0.0.9
```
Fetch the dependencies by following command:
```
flutter pub get
```

## :triangular_ruler: Using

```dart
// Start AudioPlayer and provide int for id.
var audioPlayer = new AudioPlayer(id: 0);

// Load audio file
audioPlayer.load('/home/alexmercerind/music.mp3');

// Start playing loaded audio file.
audioPlayer.play();

// Get audio duration.
print('Duration Of Track: ${audioPlayer.getDuration()}');

// Change playback volume.
audioPlayer.setVolume(0.5);
print('Changed volume to 50%.');

// Change playback position.
audioPlayer.setPosition(Duration(seconds: 10));

// Get playback position.
print('Position of playback after skipping 10 seconds: ${audioPlayer.getPosition()}');

Timer(Duration(seconds: 10), () {
    // Pause the playback.
    audioPlayer.pause();
    print('Playback of audio stopped after 10 seconds.');
}
```

#### Linux

![](https://github.com/alexmercerind/flutter_audio_desktop/blob/assets/linux.png?raw=true)

You can see [this](https://github.com/alexmercerind/flutter_audio_desktop/blob/master/example/lib/main.dart) simple example app, if you are confused with the usage.


## :page_facing_up: License

MIT

## :heavy_check_mark: Progress

The library is supported on Microsoft Windows & Linux.

|Platform            |Status     |
|--------------------|-----------|
|Linux               |Working    |
|Microsoft Windows   |Coming Soon|
|MacOS               |Not Working|

## :wrench: Workings

One word, C++. It calls native methods using Dart's MethodChannel. It uses [miniaudio](https://github.com/mackron/miniaudio) & [miniaudio_engine](https://github.com/mackron/miniaudio) from [David Reid](https://github.com/mackron). 

I made [this](https://github.com/alexmercerind/flutter_audio_desktop/blob/master/audioplayer) simple implementation in C++ around his library to get this working.

###### :love_letter: Thanks to [David Reid](https://github.com/mackron)  for [miniaudio](https://github.com/mackron/miniaudio).
