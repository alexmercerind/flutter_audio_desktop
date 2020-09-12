# flutter_audio_desktop_example

#### Demonstrates how to use the [flutter_audio_desktop](http://github.com/alexmercerind/flutter_audio_desktop) plugin.


# A Simple Flutter App

```dart

import 'dart:io';

import 'package:flutter/material.dart';
import 'package:flutter_audio_desktop/flutter_audio_desktop.dart';

class Player extends StatefulWidget {
  Player({Key key}) : super(key: key);
  PlayerState createState() => PlayerState();
}

class PlayerState extends State<Player> {
  GlobalKey<ScaffoldState> _scaffoldKey = new GlobalKey<ScaffoldState>();
  String textFieldValue = '';
  String fileLocation = '';
  bool isPlaying = false;
  double volumeValue = 1.0;

  /// Simple player implementation. Setting debug: true for extra logging.
  AudioPlayer audioPlayer = new AudioPlayer(debug: true);

  Widget build(BuildContext context) {
    return Scaffold(
      floatingActionButton: FloatingActionButton(
        tooltip: 'Play loaded audio',
        child: this.isPlaying ? Icon(Icons.pause) : Icon(Icons.play_arrow),
        onPressed: () {
          if (this.fileLocation == '') {
            this._scaffoldKey.currentState.showSnackBar(SnackBar(
                  content: Text('Load audio first.'),
                  behavior: SnackBarBehavior.floating,
                ));
          } else {
            this.setState(() {
              if (!this.isPlaying) {
                /// Playing loaded audio file.
                this.audioPlayer.play();
                this._scaffoldKey.currentState.showSnackBar(SnackBar(
                      content: Text('Playing...'),
                      behavior: SnackBarBehavior.floating,
                    ));
                isPlaying = true;
              } else if (this.isPlaying) {
                /// Pausing playback of loaded audio file.
                this.audioPlayer.pause();
                this._scaffoldKey.currentState.showSnackBar(SnackBar(
                      content: Text('Paused.'),
                      behavior: SnackBarBehavior.floating,
                    ));
                isPlaying = false;
              }
            });
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
              padding: EdgeInsets.all(64),
              child: Row(
                mainAxisAlignment: MainAxisAlignment.center,
                children: [
                  SizedBox(
                    width: 480,
                    child: TextField(
                      onChanged: (String value) {
                        this.textFieldValue = value;
                      },
                      onEditingComplete: () {
                        this.fileLocation = this.textFieldValue;
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
                    onPressed: () {
                      this.fileLocation = this.textFieldValue;
                      if (File(this.fileLocation).existsSync()) {
                        this._scaffoldKey.currentState.showSnackBar(SnackBar(
                              content: Text(
                                  'Audio file is loaded. Play using FAB below.'),
                              behavior: SnackBarBehavior.floating,
                            ));
                      } else {
                        this._scaffoldKey.currentState.showSnackBar(SnackBar(
                              content: Text('Audio file does not exist.'),
                              behavior: SnackBarBehavior.floating,
                            ));
                      }
                      if (this.fileLocation != '') {
                        /// Loading an audio file.
                        audioPlayer.load(fileLocation);
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
                        value: this.volumeValue,
                        onChanged: (value) {
                          this.setState(() {
                            this.volumeValue = value;

                            /// Changing playback volume.
                            this.audioPlayer.setVolume(this.volumeValue);
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
      theme: ThemeData(
        primarySwatch: Colors.blue,
      ),
      home: Player(),
    );
  }
}

void main() {
  runApp(MyApp());
}

```