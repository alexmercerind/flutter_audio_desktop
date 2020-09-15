/* http://github.com/alexmercerind/flutter_audio_desktop */

library flutter_audio_desktop;
import 'package:flutter/services.dart';
import 'dart:io';


final MethodChannel _channel = MethodChannel('flutter_audio_desktop');


class AudioPlayer {
  
  final bool debug;
  bool isLoaded = false;
  bool isPlaying = false;
  bool isPaused = false;
  bool isStopped = true;
  double volume = 1.0;
  List<bool> _playerState = [false, false, false, true];

  /// ## Starting Audio Service
  ///
  ///     AudioPlayer audioPlayer = new AudioPlayer();
  ///
  ///  You can optionally pass `debug: true` for extra logging.
  ///
  ///     AudioPlayer audioPlayer = new AudioPlayer(debug: true);
  AudioPlayer({this.debug = false}) {
    if (this.debug) {
      _channel.invokeMethod('init', 1);
    } else {
      _channel.invokeMethod('init', 0);
    }
  }

  /// ## Loading Audio File
  ///
  ///     await audioPlayer.load('/home/alexmercerind/music.mp3');
  ///
  ///  Results in `Future<true>`, if the audio file is successfully loaded.
  ///
  ///  Returns `Future<false>`, if the audio file does not exists.
  ///
  /// NOTE: When providing path as a paramter, it is recommended to use
  /// single forward slashes or double backward slashes only.
  ///
  /// Example of valid paths:
  ///
  /// 1) `"D:\\alexmercerind\\Music\\music.mp3"`
  ///
  /// 2) `"D:/alexmercerind/Music/music.mp3"`
  Future<bool> load(String fileLocation) async {
    File audioFile = File(fileLocation);
    if (this._playerState[1]) {
      await _channel.invokeMethod('pause');
    }
    if (this._playerState[0]) {
      await _channel.invokeMethod('stop');
    }
    if (await audioFile.exists()) {
      this._setPlayerState(true, false, true, true);
      await _channel.invokeMethod('load', fileLocation);
      return true;
    } else {
      this._setPlayerState(false, false, false, true);
      return false;
    }
  }

  /// ## Playing Loaded Audio File
  ///
  ///     await audioPlayer.play();
  ///
  ///  Starts playing the loaded audio file & results in `Future<true>`.
  ///
  ///  Returns `Future<false>` if no file is loaded or a file is already playing.
  Future<bool> play() async {
    bool success;
    if (this._playerState[1]) {
      success = false;
    } else {
      if (this._playerState[0]) {
        this._setPlayerState(true, true, false, false);
        await _channel.invokeMethod('play');
        success = true;
      } else {
        success = false;
      }
    }
    return success;
  }

  /// ## Pausing Loaded Audio File
  ///
  ///     await audioPlayer.pause();
  ///
  ///  Pauses the loaded audio file & results in `Future<true>`.
  ///
  ///  Returns `Future<false>` if no file is loaded or playback is already paused.
  Future<bool> pause() async {
    if (this._playerState[2]) {
      return false;
    } else {
      if (this._playerState[0]) {
        this._setPlayerState(true, false, true, false);
        await _channel.invokeMethod('pause');
        return true;
      } else {
        return false;
      }
    }
  }

  /// ## Unloading Audio File
  ///
  ///     await audioPlayer.pause();
  ///
  ///  Unloads loaded audio file & results in `Future<true>`.
  ///
  ///  Returns `Future<false>` if no file is loaded or a playback already stopped.
  ///
  ///  Once this method is called, you must call [load] once again.
  ///
  ///     await audioPlayer.load('/home/alexmercerind/music.mp3');
  Future<bool> stop() async {
    if (this._playerState[3]) {
      return false;
    } else {
      if (this._playerState[0]) {
        this._setPlayerState(false, false, false, true);
        await _channel.invokeMethod('pause');
        await _channel.invokeMethod('stop');
        return true;
      } else {
        return false;
      }
    }
  }

  /// ## Getting Audio File Duration
  ///
  ///     Duration audioDuration = await audioPlayer.getDuration();
  ///
  ///  Results in `Future<Duration>`.
  ///
  ///  Returns `Future<false>` if no file is loaded.
  Future<dynamic> getDuration() async {
    if (this._playerState[0]) {
      return Duration(milliseconds: await _channel.invokeMethod('getDuration'));
    } else {
      return false;
    }
  }

  /// ## Getting Audio Playback Position
  ///
  ///     Duration audioPosition = await audioPlayer.getPosition();
  ///
  ///  Results in `Future<Duration>`.
  ///
  ///  Returns `Future<false>` if no file is loaded.
  Future<dynamic> getPosition() async {
    if (this._playerState[0] &&
        (this._playerState[1] || this._playerState[2])) {
      return Duration(milliseconds: await _channel.invokeMethod('getPosition'));
    }
    if (this._playerState[0]) {
      return Duration(milliseconds: 0);
    } else {
      return false;
    }
  }

  /// ## Setting Audio Playback Position
  ///
  ///     await audioPlayer.setPosition(Duration(seconds: 18));
  ///
  ///  Results in `Future<true>`.
  ///
  ///  Returns `Future<false>` if no file is loaded.
  Future<bool> setPosition(Duration duration) async {
    if (this._playerState[0]) {
      await _channel.invokeMethod('setPosition', duration.inMilliseconds);
      return true;
    } else {
      return false;
    }
  }

  /// ## Setting Audio Playback Volume
  ///
  ///     audioPlayer.setVolume(0.25);
  ///
  /// You can access the volume of the player anytime later on.
  ///
  ///     double currentVolume = audioPlayer.volume;
  void setVolume(double volume) {
    _channel.invokeMethod('setVolume', volume);
    this.volume = volume;
  }

  void _setPlayerState(
      bool isLoaded, bool isPlaying, bool isPaused, bool isStopped) {
    this._playerState = [isLoaded, isPlaying, isPaused, isStopped];
    this.isLoaded = isLoaded;
    this.isPlaying = isPlaying;
    this.isPaused = isPaused;
    this.isStopped = isStopped;
  }
}
