library flutter_audio_linux;

import 'dart:io';
import 'package:ffi/ffi.dart';
import 'package:flutter_audio_desktop/src/native_functions.dart' as audio;

class AudioPlayer {
  final bool debug;

  bool isLoaded = false;
  bool isPlaying = false;
  bool isPaused = false;
  bool isStopped = true;

  double volume = 1.0;

  /**
   * ### AudioPlayer State
   * 
   * `0` : `Loading  State`
   * 
   * `1` : `Playing  State`
   * 
   * `2` : `Pausing  State`
   * 
   * `3` : `Stopping State`
   */
  List<bool> _playerState = [false, false, false, true];

  /**
   * ### Starting Audio Service
   * 
   *     AudioPlayer audioPlayer = new AudioPlayer();
   * 
   *  You can optionally pass `debug: true` for extra logging.
   * 
   *     AudioPlayer audioPlayer = new AudioPlayer(debug: true);
   */
  AudioPlayer({this.debug = false}) {
    if (this.debug) {
      audio.init(1);
    } else {
      audio.init(0);
    }
  }

  /**
   * ### Loading Audio File
   * 
   *     await audioPlayer.load('/home/alexmercerind/music.mp3');
   * 
   *  Results in `true`, if the audio file is successfully loaded.
   * 
   *  Returns `false`, if the audio file does not exists.
   */
  Future<bool> load(String fileLocation) async {
    File audioFile = File(fileLocation);
    if (this._playerState[1]) {
      this.pause();
    }
    if (this._playerState[0]) {
      this.stop();
    }
    if (await audioFile.exists()) {
      audio.load(Utf8.toUtf8(fileLocation));
      this._setPlayerState(true, false, true, true);
      return true;
    } else {
      this._setPlayerState(false, false, false, true);
      return false;
    }
  }

  /**
   * ### Playing Loading Audio File
   * 
   *     await audioPlayer.play();
   * 
   *  Starts playing the loaded audio file & results in `true`.
   * 
   *  Returns `false` if no file is loaded or a file is already playing.
   */
  Future<bool> play() async {
    bool success;
    if (this._playerState[1]) {
      success = false;
    } else {
      if (this._playerState[0]) {
        audio.play();
        this._setPlayerState(true, true, false, false);
        success = true;
      } else {
        success = false;
      }
    }
    return success;
  }

  /**
   * ### Pausing Loading Audio File
   * 
   *     await audioPlayer.pause();
   * 
   *  Pauses the loaded audio file & results in `true`.
   * 
   *  Returns `false` if no file is loaded or playback is already paused.
   */
  Future<bool> pause() async {
    if (this._playerState[2]) {
      return false;
    } else {
      if (this._playerState[0]) {
        audio.pause();
        this._setPlayerState(true, false, true, false);
        return true;
      } else {
        return false;
      }
    }
  }

  /**
   * ### Unloading Audio File
   * 
   *     await audioPlayer.pause();
   * 
   *  Unloads loaded audio file & results in `true`.
   * 
   *  Returns `false` if no file is loaded or a playback already stopped.
   * 
   *  Once this method is called, you must call [load] once again.
   * 
   *     await audioPlayer.load('/home/alexmercerind/music.mp3');
   */
  Future<bool> stop() async {
    if (this._playerState[3]) {
      return false;
    } else {
      if (this._playerState[0]) {
        audio.pause();
        audio.stop();
        this._setPlayerState(false, false, false, true);
        return true;
      } else {
        return false;
      }
    }
  }

  /**
   * ### Getting Audio File Duration
   * 
   *     Duration audioDuration = await audioPlayer.getDuration();
   * 
   *  Results in `Duration`.
   * 
   *  Returns `false` if no file is loaded.
   */
  Future<dynamic> getDuration() async {
    if (this._playerState[0]) {
      return Duration(milliseconds: audio.getDuration());
    } else {
      return false;
    }
  }

  /**
   * ### Getting Audio Playback Position
   * 
   *     Duration audioPosition = await audioPlayer.getPosition();
   * 
   *  Results in `Duration`.
   * 
   *  Returns `false` if no file is loaded.
   */
  Future<dynamic> getPosition() async {
    if (this._playerState[0] &&
        (this._playerState[1] || this._playerState[2])) {
      return Duration(milliseconds: audio.getDuration());
    }
    if (this._playerState[0]) {
      return Duration(milliseconds: 0);
    } else {
      return false;
    }
  }

  /**
   * ### Setting Audio Playback Position
   * 
   *     await audioPlayer.setPosition(Duration(seconds: 18));
   * 
   *  Results in `true`.
   * 
   *  Returns `false` if no file is loaded.
   */
  Future<bool> setPosition(Duration duration) async {
    if (this._playerState[0]) {
      audio.setPosition(duration.inMilliseconds);
      return true;
    } else {
      return false;
    }
  }

  /**
   * ### Setting Audio Playback Volume
   * 
   *     audioPlayer.setVolume(0.25);
   * 
   * You can access the volume of the player anytime later on.
   * 
   *     double currentVolume = audioPlayer.volume;
   */
  void setVolume(double volume) {
    audio.setVolume(volume);
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
