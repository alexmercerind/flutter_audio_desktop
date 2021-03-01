import 'package:flutter_audio_desktop/source/core/channel.dart';
import 'package:flutter_audio_desktop/source/core/audioevents.dart';
import 'package:flutter_audio_desktop/source/types/audiosource.dart';
import 'package:flutter_audio_desktop/source/types/audio.dart';


class AudioPlayer extends AudioPlayerEvents {
  
  AudioPlayer({id: 0}) {
    this.id = id;
    this.audio = new Audio();
    this.audio.file = null;
    this.audio.isPlaying = false;
    this.audio.isCompleted = false;
    this.audio.isStopped = true;
    this.audio.position = Duration.zero;
    this.audio.duration = Duration.zero;
    this.stream = this._startStream().asBroadcastStream()..listen((_) {});
  }

  Future<void> load(AudioSource audioSource) async {
    await channel.invokeMethod(
      'load',
      {'id': this.id, 'filePath': audioSource.file.path},
    );
    await this.onLoad(audioSource.file);
  }

  Future<void> play() async {
    if (!this.audio.isStopped) {
      await channel.invokeMethod(
        'play',
        {'id': this.id},
      );
      await this.onPlay();
    }
  }

  Future<void> pause() async {
    if (!this.audio.isStopped) {
      this.audio.isPlaying = false;
      await channel.invokeMethod(
        'pause',
        {'id': this.id},
      );
    }
  }

  Future<void> stop() async {
    if (!this.audio.isStopped) {
      await channel.invokeMethod(
        'stop',
        {'id': this.id},
      );
      await this.onStop();
    }
  }

  Future<void> setPosition(Duration duration) async {
    if (!this.audio.isStopped) {
      return await channel.invokeMethod(
      'setPosition',
        {'id': this.id, 'position': duration.inMilliseconds},
      );
    }
  }

  Future<void> setVolume(double volume) async {
    if (!this.audio.isStopped) {
      await channel.invokeMethod(
        'setVolume',
        {'id': this.id, 'volume': volume},
      );
    }
  }

  Future<Audio> _notifyCurrentAudioState() async {
    if (!this.audio.isCompleted) await this.onUpdate();
    else await this.onStop();
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
    await for(Future<Audio> audioFuture in stream) {
      Audio audio = await audioFuture;
      if (audio.isCompleted) {
        yield audio;
        await this.stop();
      }
      else if (audio.isPlaying) {
        yield audio;
        wasAudioPaused = false;
      }
      else if (!wasAudioPaused) {
        yield audio;
        wasAudioPaused = true;
      }
    }
  }
}
