import 'dart:async';

import 'package:flutter_audio_desktop/flutter_audio_desktop.dart';

void main() {
  // Start AudioPlayer. Set debug: true for extra logs.
  var audioPlayer = new AudioPlayer(debug: false);

  // Load audio file.
  audioPlayer.load('./music.mp3');

  // Start playing loaded audio file.
  audioPlayer.play();
  print('Duration Of Track: ${audioPlayer.getDuration()}');

  // Change playback volume.
  audioPlayer.setVolume(0.5);
  print('Changed volume to 50%.');

  // Change playback position.
  audioPlayer.setPosition(Duration(seconds: 10));
  print(
      'Position of playback after skipping 60 seconds: ${audioPlayer.getPosition()}');

  Timer(Duration(seconds: 10), () {
    // Pause the playback.
    audioPlayer.pause();
    print('Playback of audio stopped after 10 seconds.');
  });
}
