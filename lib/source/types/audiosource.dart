import 'dart:io';
import 'package:flutter/widgets.dart';
import 'package:flutter/services.dart' show rootBundle;
import 'package:path/path.dart' as path;
import 'package:path_provider/path_provider.dart' as path;


class AudioSource {
  File file;

  AudioSource({@required this.file});

  static AudioSource fromFile(File file) {
    if (file.existsSync()) {
      return new AudioSource(file: file);
    }
    else {
      throw FileSystemException('EXCEPTION: File does not exist.', file.path);
    }
  }

  static Future<AudioSource> fromAsset(String asset) async {
    String temporaryDirectoryPath = (await path.getTemporaryDirectory()).path;
    String temporaryFilePath = path.join(
      temporaryDirectoryPath,
      'audio.${asset.split('.').last}',
    );
    File audioFile = new File(temporaryFilePath);
    await audioFile.writeAsBytes(
      (await rootBundle.load(asset)).buffer.asUint8List(),
    );
    return new AudioSource(
      file: audioFile,
    );
  }
}
