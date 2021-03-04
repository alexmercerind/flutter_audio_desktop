import 'package:flutter_audio_desktop/source/core/channel.dart';
import 'package:flutter_audio_desktop/source/core/devices.dart';
import 'package:flutter_audio_desktop/source/core/events.dart';
import 'package:flutter_audio_desktop/source/types/source.dart';
import 'package:flutter_audio_desktop/source/types/audio.dart';

class AudioPlayer extends AudioPlayerEvents {
  /// ### Creates a new audio player instance or gets an existing one.
  ///
  /// Provide a unique integer as [id] while creating new object.
  /// ```dart
  /// AudioPlayer audioPlayer = new AudioPlayer(id: 0);
  /// ```
  ///
  /// Optionally provide [AudioDevice] as [device] to change the playback device for the audio player.
  /// If a [device] is not provided, audio player will play audio on the default device.
  /// ```dart
  /// AudioPlayer audioPlayer = new AudioPlayer(
  ///   id: 0,
  ///   device: (await AudioDevices.allDevices).last,
  /// );
  /// ```
  ///
  /// **NOTE:** Use [AudioDevices.allDevices] and [AudioDevices.defaultDevice] to get devices available on your device.
  /// ```dart
  /// this.allDevices = await AudioDevices.allDevices;
  /// ```
  /// **NOTE:** If an [id] is used for the first time, a new instance of audio player will be created.
  /// If the provided [id] was already used before, no new audio player will be created.

  AudioPlayer({int id: 0, AudioDevice device}) {
    this.id = id;
    if (device == null) {
      this.deviceId = 'default';
    } else {
      this.deviceId = device.id;
    }
    this.audio = new Audio();
    this.audio.file = null;
    this.audio.isPlaying = false;
    this.audio.isCompleted = false;
    this.audio.isStopped = true;
    this.audio.position = Duration.zero;
    this.audio.duration = Duration.zero;
    this.stream = this._startStream().asBroadcastStream()..listen((_) {});
  }

  /// ### Loads an audio source to the player.
  ///
  /// Provide an [AudioSource] as parameter.
  ///
  /// - Loading a file.
  ///
  /// ```dart
  /// audioPlayer.load(
  ///   await AudioSource.fromFile(
  ///     new File(filePath),
  ///   ),
  /// );
  /// ```
  ///
  /// - Loading an asset.
  ///
  /// ```dart
  /// audioPlayer.load(
  ///   await AudioSource.fromAsset(
  ///     'assets/audio.MP3',
  ///   ),
  /// );
  /// ```
  ///
  Future<void> load(AudioSource audioSource) async {
    await channel.invokeMethod(
      'load',
      {
        'id': this.id,
        'deviceId': this.deviceId,
        'filePath': audioSource.file.path,
      },
    );
    await this.onLoad(audioSource.file);
  }

  /// ### Plays loaded audio source.
  ///
  /// ```dart
  /// audioPlayer.play();
  /// ```
  ///
  /// **NOTE:** Method does nothing if no audio source is loaded.
  ///
  Future<void> play() async {
    if (!this.audio.isStopped) {
      await channel.invokeMethod(
        'play',
        {
          'id': this.id,
          'deviceId': this.deviceId,
        },
      );
      await this.onPlay();
    }
  }

  /// ### Pauses loaded audio source.
  ///
  /// ```dart
  /// audioPlayer.pause();
  /// ```
  ///
  /// **NOTE:** Method does nothing if no audio source is loaded.
  ///
  Future<void> pause() async {
    if (!this.audio.isStopped) {
      this.audio.isPlaying = false;
      await channel.invokeMethod(
        'pause',
        {
          'id': this.id,
          'deviceId': this.deviceId,
        },
      );
    }
  }

  /// ### Stops audio player instance.
  ///
  /// ```dart
  /// audioPlayer.stop();
  /// ```
  ///
  /// **NOTE:** Once this method is called, [AudioPlayer.load] must be called again.
  ///
  Future<void> stop() async {
    if (!this.audio.isStopped) {
      await channel.invokeMethod(
        'stop',
        {
          'id': this.id,
          'deviceId': this.deviceId,
        },
      );
      await this.onStop();
    }
  }

  /// ### Seeks loaded audio source.
  ///
  /// Provide an [Duration] as parameter.
  ///
  /// ```dart
  /// audioPlayer.setPosition(
  ///   Duration(seconds: 20),
  /// );
  /// ```
  ///
  Future<void> setPosition(Duration duration) async {
    if (!this.audio.isStopped) {
      return await channel.invokeMethod(
        'setPosition',
        {
          'id': this.id,
          'deviceId': this.deviceId,
          'position': duration.inMilliseconds,
        },
      );
    }
  }

  /// ### Sets audio player instance volume.
  ///
  /// Provide an [double] as parameter.
  ///
  /// ```dart
  /// audioPlayer.setVolume(0.8);
  /// ```
  ///
  Future<void> setVolume(double volume) async {
    if (!this.audio.isStopped) {
      await channel.invokeMethod(
        'setVolume',
        {
          'id': this.id,
          'deviceId': this.deviceId,
          'volume': volume,
        },
      );
    }
  }

  Future<Audio> _notifyCurrentAudioState() async {
    if (!this.audio.isCompleted)
      await this.onUpdate();
    else
      await this.onStop();
    return this.audio;
  }

  Stream<Audio> _startStream() async* {
    bool wasAudioPaused = false;
    Stream<Future<Audio>> stream = Stream.periodic(
      Duration(milliseconds: 100),
      (_) async {
        return await this._notifyCurrentAudioState();
      },
    );
    await for (Future<Audio> audioFuture in stream) {
      Audio audio = await audioFuture;
      if (audio.isCompleted) {
        yield audio;
        await this.stop();
      } else if (audio.isPlaying) {
        yield audio;
        wasAudioPaused = false;
      } else if (!wasAudioPaused) {
        yield audio;
        wasAudioPaused = true;
      }
    }
  }
}
