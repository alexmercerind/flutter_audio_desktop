library flutter_audio_linux;

import 'package:ffi/ffi.dart';
import 'package:flutter_audio_desktop/src/native_functions.dart' as audio;

class AudioPlayer {
  final bool debug;

  /// Starts The Audio Service
  /// use debug: true for logging the information.
  AudioPlayer({this.debug = false}) {
    if (this.debug) {
      audio.init(1);
    } else {
      audio.init(0);
    }
  }

  /// Loads the audio file as its location path.
  void load(String fileLocation) => audio.load(Utf8.toUtf8(fileLocation));

  /// Starts playing the currently loaded audio file.
  void play() => audio.play();

  /// Pauses the playback of currently playing file.
  void pause() => audio.pause();

  /// Stops the audio service.
  void stop() => audio.stop();

  /// Gives the duration of currently loaded audio file.
  Duration getDuration() => Duration(seconds: audio.getDuration());

  /// Gets the current playback position of the currently playing audio file.
  Duration getPosition() => Duration(seconds: audio.getPosition());

  /// Gets the current playback position of the currently playing audio file.
  void setPosition(Duration duration) => audio.setPosition(duration.inSeconds);

  /// Sets the volume of the audio service.
  void setVolume(double volume) => audio.setVolume(volume);
}
