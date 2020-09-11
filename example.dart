import 'dart:async';

import 'package:flutter_audio_desktop/flutter_audio_desktop.dart';

void main() {
  var audioPlayer = new AudioPlayer(debug: false);

  audioPlayer.load('./music.mp3');
  audioPlayer.play();

  Timer(Duration(hours: 1), () {});
}
