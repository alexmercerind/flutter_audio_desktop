import 'dart:io';


class Audio {
  File file;
  bool isPlaying;
  bool isCompleted;
  bool isStopped;
  Duration position;
  Duration duration;

  Audio({this.file, this.isPlaying, this.isStopped, this.isCompleted, this.position, this.duration});
}
