import 'dart:io';
import 'package:flutter/material.dart';
import 'package:flutter_audio_desktop/flutter_audio_desktop.dart';

class Player extends StatefulWidget {
  Player({Key key}) : super(key: key);
  PlayerState createState() => PlayerState();
}

class PlayerState extends State<Player> {
  AudioDevice defaultDevice;
  List<AudioDevice> allDevices;
  AudioPlayer audioPlayer;
  File file;
  bool isPlaying = false;
  bool isStopped = true;
  bool isCompleted = false;
  Duration position = Duration.zero;
  Duration duration = Duration.zero;
  double volume = 1.0;
  TextEditingController textController = new TextEditingController();

  @override
  void didChangeDependencies() async {
    super.didChangeDependencies();
    // Create AudioPlayer object by providing any id.
    // You can change playback device by providing device.
    this.audioPlayer = new AudioPlayer(id: 0)
      // Listen to AudioPlayer events.
      ..stream.listen(
        (Audio audio) {
          this.setState(() {
            this.file = audio.file;
            this.isPlaying = audio.isPlaying;
            this.isStopped = audio.isStopped;
            this.isCompleted = audio.isCompleted;
            this.position = audio.position;
            this.duration = audio.duration;
          });
        },
      );
    // Get default & all devices to initialize in AudioPlayer.
    // Here we are just showing it to the user.
    this.defaultDevice = await AudioDevices.defaultDevice;
    this.allDevices = await AudioDevices.allDevices;
  }

  // Get AudioPlayer events without stream.
  void updatePlaybackState() {
    this.setState(() {
      this.file = this.audioPlayer.audio.file;
      this.isPlaying = this.audioPlayer.audio.isPlaying;
      this.isStopped = this.audioPlayer.audio.isStopped;
      this.isCompleted = this.audioPlayer.audio.isCompleted;
      this.position = this.audioPlayer.audio.position;
      this.duration = this.audioPlayer.audio.duration;
    });
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
        padding: EdgeInsets.only(
          top: 8.0,
          bottom: 8.0,
          left: 8.0,
          right: 8.0,
        ),
        children: [
          Card(
            elevation: 2.0,
            color: Colors.white,
            margin: EdgeInsets.all(8.0),
            child: Padding(
              padding: EdgeInsets.only(left: 16.0, right: 16.0, bottom: 12.0),
              child: Column(
                children: [
                  SubHeader('File Loading'),
                  Row(
                    children: [
                      Expanded(
                        child: TextField(
                          controller: this.textController,
                          autofocus: true,
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
                          onPressed: () async {
                            // Load AudioSource.
                            await this.audioPlayer.load(
                                  AudioSource.fromFile(
                                    new File(this.textController.text),
                                  ),
                                );
                            this.updatePlaybackState();
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
                          onPressed: this.isStopped
                              ? null
                              : () async {
                                  await this.audioPlayer.play();
                                  this.updatePlaybackState();
                                },
                        ),
                      ),
                      Padding(
                        padding: EdgeInsets.all(18.0),
                        child: IconButton(
                          icon: Icon(Icons.pause),
                          iconSize: 32.0,
                          color: Colors.blue,
                          onPressed: this.isStopped
                              ? null
                              : () async {
                                  await this.audioPlayer.pause();
                                  this.updatePlaybackState();
                                },
                        ),
                      ),
                      Padding(
                        padding: EdgeInsets.all(18.0),
                        child: IconButton(
                          icon: Icon(Icons.stop),
                          iconSize: 32.0,
                          color: Colors.blue,
                          onPressed: this.isStopped
                              ? null
                              : () async {
                                  await this.audioPlayer.stop();
                                  this.updatePlaybackState();
                                },
                        ),
                      ),
                      Slider(
                        value: this.volume,
                        min: 0.0,
                        max: 1.0,
                        onChanged: this.isStopped
                            ? null
                            : (double volume) async {
                                this.volume = volume;
                                // Change Volume.
                                await this.audioPlayer.setVolume(this.volume);
                                this.updatePlaybackState();
                              },
                      ),
                    ],
                  ),
                  SubHeader('Position & Duration Setters/Getters'),
                  Row(
                    children: [
                      Text(this.getDurationString(this.position)),
                      Expanded(
                        child: Slider(
                          value: this.position.inMilliseconds.toDouble(),
                          min: 0.0,
                          max: this.duration.inMilliseconds.toDouble(),
                          onChanged: this.isStopped
                              ? null
                              : (double position) async {
                                  // Get or set playback position.
                                  await this.audioPlayer.setPosition(
                                        Duration(
                                            milliseconds: position.toInt()),
                                      );
                                },
                        ),
                      ),
                      Text(this.getDurationString(this.duration)),
                    ],
                  ),
                ],
              ),
            ),
          ),
          Card(
            elevation: 2.0,
            color: Colors.white,
            margin: EdgeInsets.all(8.0),
            child: Padding(
              padding: EdgeInsets.only(left: 16.0, right: 16.0, bottom: 12.0),
              child: Column(
                children: [
                  SubHeader('Playback State'),
                  Table(
                    children: [
                      TableRow(children: [
                        Text(
                          'audio.file',
                          style: TextStyle(
                            fontSize: 16,
                          ),
                        ),
                        Text(
                          '${this.file}',
                          style: TextStyle(
                            fontSize: 16,
                          ),
                        ),
                      ]),
                      TableRow(children: [
                        Text(
                          'audio.isPlaying',
                          style: TextStyle(
                            fontSize: 16,
                          ),
                        ),
                        Text(
                          '${this.isPlaying}',
                          style: TextStyle(
                            fontSize: 16,
                          ),
                        ),
                      ]),
                      TableRow(children: [
                        Text(
                          'audio.isStopped',
                          style: TextStyle(
                            fontSize: 16,
                          ),
                        ),
                        Text(
                          '${this.isStopped}',
                          style: TextStyle(
                            fontSize: 16,
                          ),
                        ),
                      ]),
                      TableRow(children: [
                        Text(
                          'audio.isCompleted',
                          style: TextStyle(
                            fontSize: 16,
                          ),
                        ),
                        Text(
                          '${this.isCompleted}',
                          style: TextStyle(
                            fontSize: 16,
                          ),
                        ),
                      ]),
                      TableRow(children: [
                        Text(
                          'audio.position',
                          style: TextStyle(
                            fontSize: 16,
                          ),
                        ),
                        Text(
                          '${this.position}',
                          style: TextStyle(
                            fontSize: 16,
                          ),
                        ),
                      ]),
                      TableRow(children: [
                        Text(
                          'audio.position',
                          style: TextStyle(
                            fontSize: 16,
                          ),
                        ),
                        Text(
                          '${this.duration}',
                          style: TextStyle(
                            fontSize: 16,
                          ),
                        ),
                      ]),
                    ],
                  ),
                ],
              ),
            ),
          ),
          Card(
            elevation: 2.0,
            color: Colors.white,
            margin: EdgeInsets.all(8.0),
            child: Padding(
              padding: EdgeInsets.only(left: 16.0, right: 16.0, bottom: 12.0),
              child: Column(
                children: [
                      SubHeader('Default Device'),
                      ListTile(
                        title: Text('${this.defaultDevice?.name}'),
                        subtitle: Text('${this.defaultDevice?.id}'),
                      ),
                      SubHeader('All Devices'),
                    ] +
                    ((this.allDevices != null)
                        ? this.allDevices.map((AudioDevice device) {
                            return ListTile(
                              title: Text('${device?.name}'),
                              subtitle: Text('${device?.id}'),
                            );
                          }).toList()
                        : []),
              ),
            ),
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
      height: 56.0,
      child: Text(
        text,
        style: TextStyle(
          fontSize: 14,
          color: Colors.black.withOpacity(0.67),
        ),
      ),
    );
  }
}
