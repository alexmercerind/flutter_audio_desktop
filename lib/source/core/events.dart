import 'dart:io';

import 'package:flutter_audio_desktop/source/core/channel.dart';
import 'package:flutter_audio_desktop/source/types/audio.dart';

class AudioPlayerInternal {
  /// Unique ID of the audio player instance.
  int id;

  /// Device ID of the device ,to which the audio player is playing.
  String deviceId;

  /// Broadcast stream to listen to playback events e.g. completion, loading, play/pause etc.
  Stream<Audio> stream;

  /// Current playback state of audio player.
  Audio audio;
}

class AudioPlayerGetters extends AudioPlayerInternal {
  /// ### Gets position of current playback.
  ///
  /// Returns [Duration].
  ///
  /// ```dart
  /// Duration position = await audioPlayer.position;
  /// ```
  ///
  Future<Duration> get position async {
    return Duration(
      milliseconds: await channel.invokeMethod(
        'getPosition',
        {
          'id': this.id,
          'deviceId': this.deviceId,
        },
      ),
    );
  }

  /// ### Gets duration of currently loaded audio source.
  ///
  /// Returns [Duration].
  ///
  /// ```dart
  /// Duration duration = await audioPlayer.duration;
  /// ```
  ///
  Future<Duration> get duration async {
    return Duration(
      milliseconds: await channel.invokeMethod(
        'getDuration',
        {
          'id': this.id,
          'deviceId': this.deviceId,
        },
      ),
    );
  }
}

class AudioPlayerEvents extends AudioPlayerGetters {
  /// Internal method used by plugin for managing playback state.
  Future<void> onLoad(File file) async {
    this.audio.file = file;
    this.audio.isPlaying = false;
    this.audio.isCompleted = false;
    this.audio.isStopped = false;
    this.audio.position = Duration.zero;
    this.audio.duration = await this.duration;
  }

  /// Internal method used by plugin for managing playback state.
  Future<void> onStop() async {
    this.audio.file = null;
    this.audio.isPlaying = false;
    this.audio.isCompleted = false;
    this.audio.isStopped = true;
    this.audio.position = Duration.zero;
    this.audio.duration = Duration.zero;
  }

  /// Internal method used by plugin for managing playback state.
  Future<void> onPlay() async {
    this.audio.isPlaying = true;
    this.audio.isCompleted = this.audio.duration.inSeconds == 0
        ? false
        : this.audio.position.inSeconds == this.audio.duration.inSeconds;
    this.audio.isStopped = false;
    this.audio.position = await this.position;
    this.audio.duration = await this.duration;
  }

  /// Internal method used by plugin for managing playback state.
  Future<void> onUpdate() async {
    this.audio.isCompleted = this.audio.duration.inSeconds == 0
        ? false
        : this.audio.position.inSeconds == this.audio.duration.inSeconds;
    this.audio.position = await this.position;
    this.audio.duration = await this.duration;
  }
}
