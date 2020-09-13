import 'package:flutter/material.dart';
import 'package:flutter_audio_desktop/flutter_audio_desktop.dart';

class Player extends StatefulWidget {
  Player({Key key}) : super(key: key);
  PlayerState createState() => PlayerState();
}

class PlayerState extends State<Player> {
  /// Simple player implementation. Setting debug: true for extra logging.
  AudioPlayer audioPlayer = new AudioPlayer(debug: true);

  GlobalKey<ScaffoldState> _scaffoldKey = new GlobalKey<ScaffoldState>();
  String _textFieldValue = '';

  Widget build(BuildContext context) {
    return Scaffold(
      floatingActionButton: FloatingActionButton(
        tooltip: 'Play loaded audio',
        child:
            audioPlayer.isPlaying ? Icon(Icons.pause) : Icon(Icons.play_arrow),
        onPressed: () {
          this.setState(() {});
          if (!audioPlayer.isLoaded) {
            this._scaffoldKey.currentState.showSnackBar(SnackBar(content: Text('Load audio first.'),));
          } else {
            if (audioPlayer.isPaused) {
              /// Playing loaded audio file.
              this.audioPlayer.play();
            } else if (audioPlayer.isPlaying) {
              /// Pausing playback of loaded audio file.
              this.audioPlayer.pause();
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
                      bool result =
                          await audioPlayer.load(this._textFieldValue);
                      if (result) {
                        this._scaffoldKey.currentState.showSnackBar(SnackBar(content: Text('Audio file is loaded. Press FAB to play.')));
                      } else {
                        this._scaffoldKey.currentState.showSnackBar(SnackBar(content: Text('Audio file is could not be loaded.')));
                      }
                    },
                  ),
                ],
              ),
            ),
            Padding(
                padding: EdgeInsets.all(64),
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
                            this.audioPlayer.setVolume(value);
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