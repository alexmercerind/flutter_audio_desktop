# [flutter_audio_desktop](https://github.com/alexmercerind/flutter_audio_desktop)

#### A simple :musical_note: audio playback library for Flutter Desktop.

Right now, as it is just a start, it supports MP3, FLAC & WAV playback. We'll try to increase the domain of support formats with time.

Feel free to open issue anytime.


## :arrow_down: Install

Mention in your pubspec.yaml requirements:
```yaml
dependencies:
  ...
  flutter_audio_desktop: ^0.0.8
```
Fetch the dependencies by following command:
```
flutter pub get
```

## :triangular_ruler: Usage
#### Playing an audio file
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
#### Playing an audio wave or noise
```dart
// New player for wave
AudioPlayer wavePlayer = new AudioPlayer(id: 1);

// Load wave
wavePlayer.loadWave(amplitude, frequency, type)

// Set frequency
wavePlayer.setWaveFrequency(frequency);

// Set amplitude
wavePlayer.setWaveAmplitude(amplitude);

// Set sample rate
wavePlayer.setWaveSampleRate(sampleRate)

// Set wave type
wavePlayer.setWaveType(type)

// New player for noise
AudioPlayer noisePlayer = new AudioPlayer(id: 2);

// Load noise
noisePlayer.loadNoise(seed, amplitude, type);

// Set seed
noisePlayer.setNoiseSeed(seed);

// Set amplitude
noisePlayer.setNoiseAmplitude(amplitude);

// Set noise type
noisePlayer.setNoiseType(type)
```

You can see [this](https://github.com/alexmercerind/flutter_audio_desktop/blob/master/example/lib/main.dart) simple example app, if you are confused with the usage.

## :ok_hand: Contributions

Thanks a lot to [@MichealReed](https://github.com/MichealReed) for adding multiple player instances, along with wave & noise APIs to the project.

## :heavy_check_mark: Progress

The library is supported on Microsoft Windows & Linux.

|Platform            |Status     |
|--------------------|-----------|
|Microsoft Windows   |Working    |
|Linux               |Working    |
|MacOS               |Not Working|

## :wrench: How It Works ?

One word, C++. It calls native methods using dart's MethodChannel. It uses [miniaudio](https://github.com/mackron/miniaudio) from [David Reid](https://github.com/mackron). 

I wrote [this](https://github.com/alexmercerind/flutter_audio_desktop/blob/master/audioplayer/audioplayer.hpp) simple wrapper in C++ around his library to get this working.

There is not any audio playback library for Flutter Desktop at the moment, so I decided to write one myself.

###### :love_letter: Thanks to [David Reid](https://github.com/mackron)  for [miniaudio](https://github.com/mackron/miniaudio).
