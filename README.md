# [flutter_audio_desktop](https://github.com/alexmercerind/flutter_audio_desktop)

#### A simple yet functional :notes: audio library for Flutter Desktop.

Right now, as it is just a start, it supports MP3, FLAC & WAV playback. I'll try to increase the domain of support formats with time & plan is to provide metadata of a track aswell.

Feel free to open issue anytime.


## :arrow_down: Install

Mention in your pubspec.yaml requirements:

```yaml
dependencies:
  ...
  flutter_audio_desktop: ^0.0.7
```


## :triangular_ruler: Usage

```dart
// Start AudioPlayer and provide int for id.
// Note: You must provide new IDs to additional instances.
var audioPlayer = new AudioPlayer(id: 0);

// See a list of available system audio devices.
// You can then change the device with setDevice,
// and the ID included in the returned map.
Map<String, dynamic> result = await audioPlayer.getDevices();
// or if you don't need to refresh, access from
audioPlayer.devices

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

// New player for waves
AudioPlayer wavePlayer = new AudioPlayer(id:1);

// Load Wave
// type = int
// 0 = sine
// 1 = square
// 2 = triangle
// 3 = sawtooth
wavePlayer.loadWave(amplitude, frequency, type)

// Set Frequency
wavePlayer.setWaveFrequency(double frequency);

// Set Amplitude
wavePlayer.setWaveAmplitude(double amplitude);

// Set Sample Rate
wavePlayer.setWaveSampleRate(int sampleRate)

// Set Wave Type

wavePlayer.setWaveType(int type)

// New player for noise
AudioPlayer noisePlayer = new AudioPlayer(id:2);

// Load Noise
// type = int
// 0 = white
// 1 = pink
// 2 = brownian
noisePlayer.loadNoise(seed, amplitude, type);

// Set Seed
noisePlayer.setNoiseSeed(int seed);

// Set Amplitude
noisePlayer.setNoiseAmplitude(double amplitude);

// Set Noise Type
noisePlayer.setNoiseType(int type)

```

You can see [this](https://github.com/alexmercerind/flutter_audio_desktop/blob/master/example/lib/main.dart) simple example app, if you are confused with the usage.


## :heart: Like the library?

Feel free to use in your Flutter Desktop app. Consider :star: starring the repository if you want to show YOUR SUPPORT to the development & appreciate the effort.

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

###### :love_letter: Thanks to [David Reid](https://github.com/mackron).
