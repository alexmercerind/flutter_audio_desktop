import 'package:flutter/material.dart';
import 'package:flutter_audio_desktop/flutter_audio_desktop.dart';

class Player extends StatefulWidget {
  Player({Key key}) : super(key: key);
  PlayerState createState() => PlayerState();
}

class PlayerState extends State<Player> {
  /// Simple player implementation. Setting debug: true for extra logging.
  AudioPlayer audioPlayer;
  AudioPlayer audioPlayer2;

  GlobalKey<ScaffoldState> _scaffoldKey = new GlobalKey<ScaffoldState>();
  String _textFieldValue = '';

  @override
  void initState() {
    super.initState();
    this.audioPlayer = new AudioPlayer(id: 0, debug: true);
    this.audioPlayer2 = new AudioPlayer(id: 1, debug: true);
  }

  Widget build(BuildContext context) {
    return Scaffold(
      floatingActionButton: FloatingActionButton(
        tooltip: 'Play loaded audio',
        child:
            audioPlayer.isPlaying ? Icon(Icons.pause) : Icon(Icons.play_arrow),
        onPressed: () {
          this.setState(() {});
          if (!audioPlayer.isLoaded) {
            this._scaffoldKey.currentState.showSnackBar(SnackBar(
                  content: Text('Load audio first.'),
                ));
          } else {
            if (audioPlayer.isPaused) {
              /// Playing loaded audio file.
              this.audioPlayer.play();
              //this.audioPlayer2.play();
            } else if (audioPlayer.isPlaying) {
              /// Pausing playback of loaded audio file.
              this.audioPlayer.pause();
              //this.audioPlayer2.pause();
            }
          }
        },
      ),
      key: this._scaffoldKey,
      appBar: AppBar(
        centerTitle: true,
        title: Text('flutter_audio_desktop Demo'),
      ),
      body: Center(
        child: Column(
          mainAxisAlignment: MainAxisAlignment.center,
          children: [
            Row(mainAxisSize: MainAxisSize.min, children: [
              Text("Load Wave"),
              IconButton(
                color: Colors.blue,
                icon: Icon(
                  Icons.check,
                  size: 28,
                ),
                onPressed: () async {
                  audioPlayer.loadWave(0.2, 400, 0);
                  audioPlayer2.loadWave(0.2, 700, 0);
                },
              ),
            ]),
            Padding(
              padding: EdgeInsets.all(32),
              child: Row(
                mainAxisAlignment: MainAxisAlignment.center,
                children: [
                  SizedBox(
                    width: 480,
                    child: TextField(
                      onChanged: (String value) {
                        this._textFieldValue = value;
                      },
                      decoration: InputDecoration(
                        hintText: 'Enter path to an audio file...',
                        labelText: 'Audio Location',
                        border: OutlineInputBorder(),
                      ),
                    ),
                  ),
                  SizedBox(
                    width: 24,
                  ),
                  IconButton(
                    color: Colors.blue,
                    icon: Icon(
                      Icons.check,
                      size: 28,
                    ),
                    tooltip: 'Load Audio',
                    onPressed: () async {
                      /// Loading an audio file into the player.
                      bool result =
                          await audioPlayer.load(this._textFieldValue);
                      if (result) {
                        this._scaffoldKey.currentState.showSnackBar(SnackBar(
                            content: Text(
                                'Audio file is loaded. Press FAB to play.')));
                      } else {
                        this._scaffoldKey.currentState.showSnackBar(SnackBar(
                            content:
                                Text('Audio file is could not be loaded.')));
                      }
                    },
                  ),
                ],
              ),
            ),
            Padding(
                padding: EdgeInsets.all(24),
                child: Row(
                  mainAxisAlignment: MainAxisAlignment.center,
                  children: [
                    Icon(
                      Icons.volume_down,
                      size: 36,
                      color: Colors.blue,
                    ),
                    Slider(
                        divisions: 10,
                        value: audioPlayer.volume,
                        onChanged: (value) {
                          this.setState(() {
                            /// Changing player volume.
                            this.audioPlayer.setVolume(value);
                          });
                        }),
                    Icon(
                      Icons.device_hub,
                      size: 36,
                      color: Colors.blue,
                    ),
                    Slider(
                        divisions: audioPlayer.devices.length > 0
                            ? audioPlayer.devices.length
                            : 1,
                        value: audioPlayer.deviceIndex.toDouble(),
                        min: 0,
                        max: audioPlayer.devices.length.toDouble(),
                        onChanged: (value) {
                          this.setState(() {
                            /// Changing player volume.
                            this
                                .audioPlayer
                                .setDevice(deviceIndex: value.toInt());
                          });
                        }),
                  ],
                )),
            Padding(
                padding: EdgeInsets.all(24), child: Text("Wave Settings: ")),
            Padding(
                padding: EdgeInsets.all(24),
                child: Row(
                  mainAxisAlignment: MainAxisAlignment.center,
                  children: [
                    Icon(
                      Icons.multiline_chart,
                      size: 36,
                      color: Colors.blue,
                    ),
                    Slider(
                        divisions: 10,
                        value: audioPlayer.waveAmplitude,
                        min: 0.0,
                        max: 1,
                        onChanged: (value) {
                          this.setState(() {
                            /// Changing player volume.
                            this.audioPlayer.setWaveAmplitude(value);
                          });
                        }),
                    Icon(
                      Icons.radio,
                      size: 36,
                      color: Colors.blue,
                    ),
                    Slider(
                        divisions: 100,
                        value: audioPlayer.waveFrequency,
                        min: 0,
                        max: 1000,
                        onChanged: (value) {
                          this.setState(() {
                            /// Changing player volume.
                            this.audioPlayer.setWaveFrequency(value);
                          });
                        }),
                    Icon(
                      Icons.timelapse,
                      size: 36,
                      color: Colors.blue,
                    ),
                    Slider(
                        divisions: 1000,
                        value: audioPlayer.waveSampleRate.toDouble(),
                        min: 1000,
                        max: 100000,
                        onChanged: (value) {
                          this.setState(() {
                            /// Changing player volume.
                            this.audioPlayer.setWaveSampleRate(value.toInt());
                          });
                        }),
                    Slider(
                        divisions: 4,
                        value: audioPlayer.waveType.toDouble(),
                        min: 0,
                        max: 4,
                        onChanged: (value) {
                          this.setState(() {
                            /// Changing player volume.
                            this.audioPlayer.setWaveType(value.toInt());
                          });
                        }),
                  ],
                )),
          ],
        ),
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
