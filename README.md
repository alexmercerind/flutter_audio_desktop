# [flutter_audio_desktop](https://github.com/alexmercerind/flutter_audio_desktop)

#### An audio playback library for Flutter Desktop.

Feel free to open issue anytime.


## Installing

Mention in your pubspec.yaml:

```yaml
dependencies:
  ...
  flutter_audio_desktop: ^0.0.9
```

## Using

```dart
// Create new instance.
var audioPlayer = new AudioPlayer(id: 0)
..stream.listen(
  (Audio audio) {
    // Listen to playback events.
  },
);
// Load audio source
audioPlayer.load(
  new AudioSource.fromFile(
    new File('/home/alexmercerind/music.mp3'),
  ),
);
// Start playback.
audioPlayer.play();
// Get audio duration.
audioPlayer.audio.duration;
// Change playback volume.
audioPlayer.setVolume(0.5);
// Change playback position.
audioPlayer.setPosition(Duration(seconds: 10));
// Get playback position.
audioPlayer.audio.position;
Timer(Duration(seconds: 10), () {
    // Pause playback.
    audioPlayer.pause();
}
// Few other things.
audioPlayer.audio.file;
audioPlayer.audio.isPlaying;
audioPlayer.audio.isCompleted;
audioPlayer.audio.isStopped;

```

See more features in [this](https://github.com/alexmercerind/flutter_audio_desktop/blob/master/example/lib/main.dart) example.

#### Windows

<img src="https://github.com/alexmercerind/flutter_audio_desktop/blob/assets/windows.png?raw=true" height="600"></img>

#### Linux

<img src="https://github.com/alexmercerind/flutter_audio_desktop/blob/assets/linux.png?raw=true" height="600"></img>

## Support

If you want to be kind to me, then you may buy me a coffee.

<a href="https://www.buymeacoffee.com/alexmercerind"><img src="https://img.buymeacoffee.com/button-api/?text=Buy me a coffee&emoji=&slug=alexmercerind&button_colour=FFDD00&font_colour=000000&font_family=Cookie&outline_colour=000000&coffee_colour=ffffff"></a>

Thankyou!


## Progress

|Platform            |Status                                                    |
|--------------------|----------------------------------------------------------|
|Linux               |Working                                                   |
|Microsoft Windows   |Working                                                   |
|MacOS               |[Learn More](https://www.youtube.com/watch?v=dQw4w9WgXcQ) |

## Workings

One word, C++. It calls native methods using Dart's MethodChannel. It uses [miniaudio](https://github.com/mackron/miniaudio) & [miniaudio_engine](https://github.com/mackron/miniaudio) from [David Reid](https://github.com/mackron). 

I made [this](https://github.com/alexmercerind/flutter_audio_desktop/blob/master/audioplayer) simple implementation in C++ around his library to get this working.

## Acknowledgments

- Thanks to [MichealReed](https://github.com/MichealReed) for his support to the project.
