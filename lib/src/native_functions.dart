/* http://github.com/alexmercerind/flutter_audio_desktop */

library native_functions;

import 'dart:ffi' as ffi;
import 'package:ffi/ffi.dart';
import 'dart:io' show Platform;
import 'package:path/path.dart' as path;

final dylib = ffi.DynamicLibrary.open(path.join(
  '/' + Platform.script.pathSegments.sublist(0, Platform.script.pathSegments.length - 2).join('/'),
  'lib',
  'src',
  Platform.isWindows ? 'audio.dll' : 'audio.so',
));

typedef InitFunction = ffi.Void Function(ffi.Int32 debug);
typedef Init = void Function(int debug);
final Init init = dylib.lookup<ffi.NativeFunction<InitFunction>>('initPlayer').asFunction();

typedef LoadFunction = ffi.Void Function(ffi.Pointer<Utf8> fileName);
typedef Load = void Function(ffi.Pointer<Utf8> fileName);
final Load load = dylib.lookup<ffi.NativeFunction<LoadFunction>>('loadPlayer').asFunction();

typedef PlayFunction = ffi.Void Function();
typedef Play = void Function();
final Play play = dylib.lookup<ffi.NativeFunction<PlayFunction>>('playPlayer').asFunction();

typedef PauseFunction = ffi.Void Function();
typedef Pause = void Function();
final Play pause = dylib.lookup<ffi.NativeFunction<PauseFunction>>('pausePlayer').asFunction();

typedef StopFunction = ffi.Void Function();
typedef Stop = void Function();
final Stop stop = dylib.lookup<ffi.NativeFunction<StopFunction>>('stopPlayer').asFunction();

typedef GetDurationFunction = ffi.Int32 Function();
typedef GetDuration = int Function();
final GetDuration getDuration = dylib.lookup<ffi.NativeFunction<GetDurationFunction>>('getDuration').asFunction<GetDuration>();

typedef GetPositionFunction = ffi.Int32 Function();
typedef GetPosition = int Function();
final GetPosition getPosition = dylib.lookup<ffi.NativeFunction<GetPositionFunction>>('getPosition').asFunction<GetPosition>();

typedef SetPositionFunction = ffi.Void Function(ffi.Int32 timeMilliseconds);
typedef SetPosition = void Function(int timeMilliseconds);
final SetPosition setPosition = dylib.lookup<ffi.NativeFunction<SetPositionFunction>>('setPosition').asFunction<SetPosition>();

typedef SetVolumeFunction = ffi.Void Function(ffi.Double volume);
typedef SetVolume = void Function(double volume);
final SetVolume setVolume = dylib.lookup<ffi.NativeFunction<SetVolumeFunction>>('setVolume').asFunction<SetVolume>();
