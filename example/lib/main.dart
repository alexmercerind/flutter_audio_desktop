import 'dart:io';
import 'package:flutter/material.dart';
import 'package:flutter_audio_desktop/flutter_audio_desktop.dart';

class Player extends StatefulWidget {
  Player({Key key}) : super(key: key);
  PlayerState createState() => PlayerState();
}

class PlayerState extends State<Player> {
  AudioPlayer audioPlayer;
  Duration position = Duration.zero;
  Duration duration = Duration.zero;
  double volume = 1.0;
  TextEditingController textController = new TextEditingController();

  @override
  void initState() {
    super.initState();
    this.audioPlayer = new AudioPlayer(id: 0)..stream.listen(
      (Audio audio) {
        this.setState(() {
          this.position = audio.position;
          this.duration = audio.duration;
        });
      },
    );
  }

  String getDurationString(Duration duration) {
    int minutes = duration.inSeconds ~/ 60;
    String seconds = duration.inSeconds - (minutes * 60) > 9
        ? '${duration.inSeconds - (minutes * 60)}'
        : '0${duration.inSeconds - (minutes * 60)}';
    return '$minutes:$seconds';
  }

  Widget build(BuildContext context) {
    return Scaffold(
      appBar: AppBar(
        centerTitle: true,
        title: Text('flutter_audio_desktop'),
      ),
      body: ListView(
        padding: EdgeInsets.all(32.0),
        children: [
          SubHeader('File Loading'),
          Row(
            children: [
              Expanded(
                child: TextField(
                  controller: this.textController,
                  decoration: InputDecoration(
                    labelText: 'File Location',
                  ),
                ),
              ),
              Padding(
                padding: EdgeInsets.symmetric(horizontal: 16.0),
                child: IconButton(
                  icon: Icon(Icons.check),
                  iconSize: 32.0,
                  color: Colors.blue,
                  onPressed: () {
                    this.audioPlayer.load(
                      new File(this.textController.text),
                    );
                  },
                ),
              ),
            ],
          ),
          SubHeader('Playback Setters/Getters'),
          Row(
            mainAxisAlignment: MainAxisAlignment.center,
            children: [
              Padding(
                padding: EdgeInsets.all(18.0),
                child: IconButton(
                  icon: Icon(Icons.play_arrow),
                  iconSize: 32.0,
                  color: Colors.blue,
                  onPressed: () => this.audioPlayer.play(),
                ),
              ),
              Padding(
                padding: EdgeInsets.all(18.0),
                child: IconButton(
                  icon: Icon(Icons.pause),
                  iconSize: 32.0,
                  color: Colors.blue,
                  onPressed: () => this.audioPlayer.pause(),
                ),
              ),
              Slider(
                value: this.volume,
                min: 0.0,
                max: 1.0,
                onChanged: (double volume) {
                  this.setState(() {
                    this.volume = volume;
                  });
                  this.audioPlayer.setVolume(this.volume);
                },
              ),
            ],
          ),
          SubHeader('Postion & Duration Setters/Getters'), 
          Row(
            children: [
              Text(this.getDurationString(this.position)),
              Expanded(
                child: Slider(
                  value: this.position.inMilliseconds.toDouble(),
                  min: 0.0,
                  max: this.duration.inMilliseconds.toDouble(),
                  onChanged: (double position) {
                    this.setState(() {
                      this.position = Duration(milliseconds: position.toInt());
                      this.audioPlayer.setPosition(
                        Duration(milliseconds: position.toInt()),
                      );
                    });
                  },
                ),
              ),
              Text(this.getDurationString(this.duration)),
            ],
          ),  
        ],
      ),
    );
  }
}

class MyApp extends StatelessWidget {
  @override
  Widget build(BuildContext context) {
    return MaterialApp(
      home: Player(),
    );
  }
}

void main() => runApp(MyApp());


class SubHeader extends StatelessWidget {
  final String text;

  const SubHeader(this.text, {Key key}) : super(key: key);

  @override
  Widget build(BuildContext context) {
    return Container(
      alignment: Alignment.centerLeft,
      height: 48,
      color: Theme.of(context).scaffoldBackgroundColor,
      child: Text(text),
    );
  }
}
