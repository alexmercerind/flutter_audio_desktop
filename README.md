# flutter_audio_desktop

### A simple yet functional 🎵️ audio library for Flutter Desktop.
#### There is not any audio playback library for Flutter Desktop, so I made one myself.

Right as it is just a start, it supports MP3 playback. I'll try to increase the domain of support formats with time & plan is to provide metadata of a track aswell.

## :triangular_ruler: Code Example

#### Usage in Dart

- **Clone the repository**

```bash
git clone http://github.com/alexmercerind/flutter_audio_desktop.git --depth=1

cd flutter_audio_desktop
```

- **Run the example.dart in Dart interpreter**

```bash
dart example.dart
```

## :ok_hand: Easy To Use

```dart
import 'flutter_audio_linux/flutter_audio_linux.dart';

void main() {
  var audioPlayer = new AudioPlayer(debug: false);

  audioPlayer.load('./music.mp3');
  audioPlayer.play();

  Timer(Duration(hours: 1), () {});
  //Keeping the Dart interpreter started while the music is playing. You don't need this in Flutter app.
}
```

## :heart: Like the library?

Feel free to use in your Flutter Desktop app. Consider :star: starring the repository if you want to support the development & appreciate the effort.

## :heavy_check_mark: Progress

The library is only supported on x64 systems right now.

|Platform|Status    |
|--------|----------|
|Linux   |Working   |
|Windows |Not Tested|
|MacOS   |Not Tested|

## :wrench: How It Works ?

One word, C++. It simply uses **dart::ffi** for accessing Native C++ for playing audio. It uses the [BASS Audio Library](http://www.un4seen.com) under the hood. 

There is not any audio playback library for Flutter Desktop at the moment, so I decided to make one myself.