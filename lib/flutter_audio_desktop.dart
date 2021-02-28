import 'dart:io';
import 'package:flutter/services.dart';

final MethodChannel _channel = MethodChannel('flutter_audio_desktop');


class Audio {
  File file;
  bool isPlaying;
  bool isStopped;
  bool isCompleted;
  Duration position;
  Duration duration;

  Audio({this.file, this.isPlaying, this.isStopped, this.isCompleted, this.position, this.duration});
}


class AudioPlayer {
  final int id;
  File file;
  Audio audio;
  Stream<Audio> stream;
  
  AudioPlayer({this.id: 0}) {
    this.audio = this.audio = Audio(
      file: this.file,
      isPlaying: false,
      isStopped: true,
      isCompleted: false,
      position: Duration.zero,
      duration: Duration.zero,
    );
    this.stream = this._startStream().asBroadcastStream()..listen((_) {});
  }

  Stream<Audio> _startStream() async* {
    bool wasAudioPaused = false;
    Stream<Future<Audio>> stream = Stream.periodic(
      Duration(milliseconds: 100),
      (_) async {
        if (!this.audio.isStopped) {
          this.audio.position = await this.position;
          this.audio.duration = await this.duration;
          this.audio.isCompleted = this.audio.duration.inSeconds == 0 ? false: this.audio.position.inSeconds == this.audio.duration.inSeconds;
        }
        else {
          this.audio.file = null;
          this.audio.isPlaying = false;
          this.audio.isCompleted = false;
          this.audio.position = Duration.zero;
          this.audio.duration = Duration.zero;
        }
        return this.audio;
      },
    );
    await for(Future<Audio> audioFuture in stream) {
      Audio audio = await audioFuture;
      if (audio.isCompleted) {
        yield audio;
        this.stop();
        this.audio.isStopped = true;
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

  Future<void> load(File file) async {
    this.audio.isStopped = false;
    this.audio.isPlaying = false;
    this.audio.isCompleted = false;
    this.audio.position = Duration.zero;
    if (await file.exists()) {
      this.file = file;
      await _channel.invokeMethod(
        'load',
        {'id': this.id, 'filePath': file.path},
      );
      this.audio.file = this.file;
      this.audio.duration = await this.duration;
    }
    else {
      this.audio.file = this.file;
      this.audio.duration = Duration.zero;
      throw 'EXCEPTION: File does not exist.';
    }
  }

  Future<void> play() async {
    this.audio.isPlaying = true;
    await _channel.invokeMethod(
      'play',
      {'id': this.id},
    );
  }

  Future<void> pause() async {
    this.audio.isPlaying = false;
    await _channel.invokeMethod(
      'pause',
      {'id': this.id},
    );
  }

  Future<void> stop() async {
    this.audio.isStopped = true;
    await _channel.invokeMethod(
      'stop',
      {'id': this.id},
    );
  }

  Future<Duration> get position async {
    return Duration(
      milliseconds: await _channel.invokeMethod(
        'getPosition',
        {'id': this.id},
      ),
    );
  }

  Future<Duration> get duration async {
    return Duration(
      milliseconds: await _channel.invokeMethod(
        'getDuration',
        {'id': this.id},
      ),
    );
  }

  Future<void> setPosition(Duration duration) async {
    return await _channel.invokeMethod(
      'setPosition',
      {'id': this.id, 'position': duration.inMilliseconds},
    );
  }

  void setVolume(double volume) {
    _channel.invokeMethod(
      'setVolume',
      {'id': this.id, 'volume': volume},
    );
  }
}
