import 'dart:io';

import 'package:flutter_audio_desktop/source/core/channel.dart';
import 'package:flutter_audio_desktop/source/types/audio.dart';


class AudioPlayerInternal {
  int id;
  Stream<Audio> stream;
  Audio audio;
}


class AudioPlayerGetters extends AudioPlayerInternal {
  Future<Duration> get position async {
    return Duration(
      milliseconds: await channel.invokeMethod(
        'getPosition',
        {'id': this.id},
      ),
    );
  }

  Future<Duration> get duration async {
    return Duration(
      milliseconds: await channel.invokeMethod(
        'getDuration',
        {'id': this.id},
      ),
    );
  }
}


class AudioPlayerEvents  extends AudioPlayerGetters {

  Future<void> onLoad(File file) async {
    this.audio.file = file;
    this.audio.isPlaying = false;
    this.audio.isCompleted = false;
    this.audio.isStopped = false;
    this.audio.position = Duration.zero;
    this.audio.duration = await this.duration;
  }

  Future<void> onStop() async {
    this.audio.file = null;
    this.audio.isPlaying = false;
    this.audio.isCompleted = false;
    this.audio.isStopped = true;
    this.audio.position = Duration.zero;
    this.audio.duration = Duration.zero;
  }

  Future<void> onPlay() async {
    this.audio.isPlaying = true;
    this.audio.isCompleted = this.audio.duration.inSeconds == 0 ? false: this.audio.position.inSeconds == this.audio.duration.inSeconds;
    this.audio.isStopped = false;
    this.audio.position = await this.position;
    this.audio.duration = await this.duration;
  }

  Future<void> onUpdate() async {
    this.audio.isCompleted = this.audio.duration.inSeconds == 0 ? false: this.audio.position.inSeconds == this.audio.duration.inSeconds;
    this.audio.position = await this.position;
    this.audio.duration = await this.duration;
  }
}
