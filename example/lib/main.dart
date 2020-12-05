import 'package:flutter/material.dart';
import 'package:flutter_audio_desktop/flutter_audio_desktop.dart';

class Player extends StatefulWidget {
  Player({Key key}) : super(key: key);
  PlayerState createState() => PlayerState();
}

class PlayerState extends State<Player> {
  /// Simple player implementation. Setting debug: true for extra logging.
  AudioPlayer filePlayer;
  AudioPlayer wavePlayer;
  AudioPlayer noisePlayer;

  GlobalKey<ScaffoldState> _scaffoldKey = new GlobalKey<ScaffoldState>();
  String _textFieldValue = '';

  @override
  void initState() {
    super.initState();
    this.filePlayer = new AudioPlayer(id: 0, debug: true);
    this.wavePlayer = new AudioPlayer(id: 1, debug: true);
    this.noisePlayer = new AudioPlayer(id: 2, debug: true);
  }

  Widget build(BuildContext context) {
    return Scaffold(
        key: this._scaffoldKey,
        appBar: AppBar(
          centerTitle: true,
          title: Text('flutter_audio_desktop Demo'),
        ),
        body: Center(
            child:
                Column(mainAxisAlignment: MainAxisAlignment.center, children: [
          Divider(),
          Padding(
              padding: EdgeInsets.all(5),
              child: Text(
                "File Settings: ",
                style: TextStyle(fontWeight: FontWeight.bold),
              )),
          Padding(
            padding: EdgeInsets.all(5),
            child: Row(
              mainAxisAlignment: MainAxisAlignment.start,
              children: [
                SizedBox(
                  width: 150,
                ),
                SizedBox(
                  width: 320,
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
                RaisedButton(
                  child: Container(
                    child: Text(
                      "Load File",
                      style: TextStyle(color: Colors.white),
                    ),
                  ),
                  color: Colors.blue,
                  onPressed: () async {
                    /// Loading an audio file into the player.
                    bool result = await filePlayer.load(this._textFieldValue);
                    if (result) {
                      this._scaffoldKey.currentState.showSnackBar(SnackBar(
                          content: Text(
                              'Audio file is loaded. Press FAB to play.')));
                    } else {
                      this._scaffoldKey.currentState.showSnackBar(SnackBar(
                          content: Text('Audio file is could not be loaded.')));
                    }
                  },
                ),
                IconButton(
                  icon: filePlayer.isPlaying
                      ? Icon(Icons.pause)
                      : Icon(Icons.play_arrow),
                  iconSize: 36,
                  color: Colors.blue,
                  onPressed: () {
                    this.setState(() {});
                    if (!filePlayer.isLoaded) {
                      this._scaffoldKey.currentState.showSnackBar(SnackBar(
                            content: Text('Load file first.'),
                          ));
                    } else {
                      if (filePlayer.isPaused) {
                        /// Playing loaded audio file.
                        this.filePlayer.play();
                      } else if (filePlayer.isPlaying) {
                        /// Pausing playback of loaded audio file.
                        this.filePlayer.pause();
                      }
                    }
                  },
                ),
                Padding(
                    padding: EdgeInsets.all(5),
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
                            value: filePlayer.volume,
                            onChanged: (value) {
                              this.setState(() {
                                /// Changing player volume.
                                this.filePlayer.setVolume(value);
                              });
                            }),
                        Icon(
                          Icons.device_hub,
                          size: 36,
                          color: Colors.blue,
                        ),
                        Slider(
                            divisions: filePlayer.devices.length > 0
                                ? filePlayer.devices.length
                                : 1,
                            value: filePlayer.deviceIndex.toDouble(),
                            min: 0,
                            max: filePlayer.devices.length.toDouble(),
                            onChanged: (value) {
                              this.setState(() {
                                /// Changing player volume.
                                this
                                    .filePlayer
                                    .setDevice(deviceIndex: value.toInt());
                              });
                            }),
                      ],
                    )),
              ],
            ),
          ),
          Divider(),
          Padding(
              padding: EdgeInsets.all(5),
              child: Text(
                "Wave Settings: ",
                style: TextStyle(fontWeight: FontWeight.bold),
              )),
          Row(mainAxisSize: MainAxisSize.min, children: [
            RaisedButton(
              child: Container(
                child: Text(
                  "Load Wave",
                  style: TextStyle(color: Colors.white),
                ),
              ),
              color: Colors.blue,
              onPressed: () async {
                wavePlayer.loadWave(0.2, 400, 0);
              },
            ),
            IconButton(
              icon: wavePlayer.isPlaying
                  ? Icon(Icons.pause)
                  : Icon(Icons.play_arrow),
              color: Colors.blue,
              iconSize: 36,
              onPressed: () {
                this.setState(() {});
                if (!wavePlayer.isLoaded) {
                  this._scaffoldKey.currentState.showSnackBar(SnackBar(
                        content: Text('Load wave first.'),
                      ));
                } else {
                  if (wavePlayer.isPaused) {
                    /// Playing loaded audio file.
                    this.wavePlayer.play();
                  } else if (wavePlayer.isPlaying) {
                    /// Pausing playback of loaded audio file.
                    this.wavePlayer.pause();
                  }
                }
              },
            ),
            Padding(
                padding: EdgeInsets.all(5),
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
                        value: wavePlayer.volume,
                        onChanged: (value) {
                          this.setState(() {
                            /// Changing player volume.
                            this.wavePlayer.setVolume(value);
                          });
                        }),
                    Icon(
                      Icons.device_hub,
                      size: 36,
                      color: Colors.blue,
                    ),
                    Slider(
                        divisions: wavePlayer.devices.length > 0
                            ? wavePlayer.devices.length
                            : 1,
                        value: wavePlayer.deviceIndex.toDouble(),
                        min: 0,
                        max: wavePlayer.devices.length.toDouble(),
                        onChanged: (value) {
                          this.setState(() {
                            /// Changing player volume.
                            this
                                .wavePlayer
                                .setDevice(deviceIndex: value.toInt());
                          });
                        }),
                  ],
                )),
          ]),
          Padding(
            padding: EdgeInsets.all(5),
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
                    value: wavePlayer.waveAmplitude,
                    min: 0.0,
                    max: 1,
                    onChanged: (value) {
                      this.setState(() {
                        /// Changing player volume.
                        this.wavePlayer.setWaveAmplitude(value);
                      });
                    }),
                Icon(
                  Icons.radio,
                  size: 36,
                  color: Colors.blue,
                ),
                Slider(
                    divisions: 100,
                    value: wavePlayer.waveFrequency,
                    min: 0,
                    max: 1000,
                    onChanged: (value) {
                      this.setState(() {
                        /// Changing player volume.
                        this.wavePlayer.setWaveFrequency(value);
                      });
                    }),
                Icon(
                  Icons.timelapse,
                  size: 36,
                  color: Colors.blue,
                ),
                Slider(
                    divisions: 1000,
                    value: wavePlayer.waveSampleRate.toDouble(),
                    min: 1000,
                    max: 100000,
                    onChanged: (value) {
                      this.setState(() {
                        /// Changing player volume.
                        this.wavePlayer.setWaveSampleRate(value.toInt());
                      });
                    }),
                Slider(
                    divisions: 4,
                    value: wavePlayer.waveType.toDouble(),
                    min: 0,
                    max: 4,
                    onChanged: (value) {
                      this.setState(() {
                        /// Changing player volume.
                        this.wavePlayer.setWaveType(value.toInt());
                      });
                    }),
              ],
            ),
          ),
          Divider(),
          Padding(
              padding: EdgeInsets.all(5),
              child: Text(
                "Noise Settings: ",
                style: TextStyle(fontWeight: FontWeight.bold),
              )),
          Row(mainAxisSize: MainAxisSize.min, children: [
            RaisedButton(
              child: Container(
                child: Text(
                  "Load Noise",
                  style: TextStyle(color: Colors.white),
                ),
              ),
              color: Colors.blue,
              onPressed: () async {
                noisePlayer.loadNoise(0, 0.2, 0);
              },
            ),
            IconButton(
              icon: noisePlayer.isPlaying
                  ? Icon(Icons.pause)
                  : Icon(Icons.play_arrow),
              color: Colors.blue,
              iconSize: 36,
              onPressed: () {
                this.setState(() {});
                if (!noisePlayer.isLoaded) {
                  this._scaffoldKey.currentState.showSnackBar(SnackBar(
                        content: Text('Load noise first.'),
                      ));
                } else {
                  if (noisePlayer.isPaused) {
                    /// Playing loaded audio file.
                    this.noisePlayer.play();
                  } else if (noisePlayer.isPlaying) {
                    /// Pausing playback of loaded audio file.
                    this.noisePlayer.pause();
                  }
                }
              },
            ),
            Padding(
                padding: EdgeInsets.all(5),
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
                        value: noisePlayer.volume,
                        onChanged: (value) {
                          this.setState(() {
                            /// Changing player volume.
                            this.noisePlayer.setVolume(value);
                          });
                        }),
                    Icon(
                      Icons.device_hub,
                      size: 36,
                      color: Colors.blue,
                    ),
                    Slider(
                        divisions: noisePlayer.devices.length > 0
                            ? noisePlayer.devices.length
                            : 1,
                        value: noisePlayer.deviceIndex.toDouble(),
                        min: 0,
                        max: noisePlayer.devices.length.toDouble(),
                        onChanged: (value) {
                          this.setState(() {
                            /// Changing player volume.
                            this
                                .noisePlayer
                                .setDevice(deviceIndex: value.toInt());
                          });
                        }),
                  ],
                )),
          ]),
          Padding(
              padding: EdgeInsets.all(5),
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
                      value: noisePlayer.noiseAmplitude,
                      min: 0.0,
                      max: 1,
                      onChanged: (value) {
                        this.setState(() {
                          /// Changing player volume.
                          this.noisePlayer.setNoiseAmplitude(value);
                        });
                      }),
                  Icon(
                    Icons.radio,
                    size: 36,
                    color: Colors.blue,
                  ),
                  Slider(
                      divisions: 9999,
                      value: noisePlayer.noiseSeed.toDouble(),
                      min: 0,
                      max: 999999999,
                      onChanged: (value) {
                        this.setState(() {
                          /// Changing player volume.
                          this.noisePlayer.setNoiseSeed(value.toInt());
                        });
                      }),
                  Icon(
                    Icons.timelapse,
                    size: 36,
                    color: Colors.blue,
                  ),
                  Slider(
                      divisions: 3,
                      value: noisePlayer.noiseType.toDouble(),
                      min: 0,
                      max: 2,
                      onChanged: (value) {
                        this.setState(() {
                          /// Changing player volume.
                          this.noisePlayer.setNoiseType(value.toInt());
                        });
                      }),
                ],
              )),
          Divider()
        ])));
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
