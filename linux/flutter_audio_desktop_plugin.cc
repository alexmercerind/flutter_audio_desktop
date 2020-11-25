#include "../audioplayer/audio.cpp"

#include "include/flutter_audio_desktop/flutter_audio_desktop_plugin.h"
#include <flutter_linux/flutter_linux.h>
#include <gtk/gtk.h>
#include <sys/utsname.h>

#define FLUTTER_AUDIO_DESKTOP_PLUGIN(obj)                                     \
  (G_TYPE_CHECK_INSTANCE_CAST((obj), flutter_audio_desktop_plugin_get_type(), \
                              FlutterAudioDesktopPlugin))

struct _FlutterAudioDesktopPlugin
{
  GObject parent_instance;
};

G_DEFINE_TYPE(FlutterAudioDesktopPlugin, flutter_audio_desktop_plugin, g_object_get_type())

static void flutter_audio_desktop_plugin_handle_method_call(
    FlutterAudioDesktopPlugin *self,
    FlMethodCall *method_call)
{
  g_autoptr(FlMethodResponse) response = nullptr;

  const gchar *method = fl_method_call_get_name(method_call);

  if (strcmp(method, "init") == 0)
  {
    int id = fl_value_get_int(fl_value_lookup(fl_method_call_get_args(method_call), fl_value_new_string("id")));
    bool debug = fl_value_get_bool(fl_value_lookup(fl_method_call_get_args(method_call), fl_value_new_string("debug")));

    Audio::initPlayer(id, debug);

    response = FL_METHOD_RESPONSE(fl_method_success_response_new(fl_value_new_null()));
  }
  else if (strcmp(method, "getDevices") == 0)
  {
    int id = fl_value_get_int(fl_value_lookup(fl_method_call_get_args(method_call), fl_value_new_string("id")));

    // Get count, new array, fill array
    int count = Audio::getDeviceCount(id);
    AudioDevice* devices = new AudioDevice[count + 1];
    Audio::getDevices(id, devices);

    // Map for flutter
    g_autoptr(FlValue) deviceInfo = fl_value_new_map();

    // Fill Map
    for (int i = 0; i < count; i++)
    {
	        const char *c = devices[i].name.c_str();        
          fl_value_set_string_take(
          deviceInfo, std::to_string(i).c_str(),
          fl_value_new_string(c));
    }
    
    // Set default
    fl_value_set_string_take(
        deviceInfo, "default",
        fl_value_new_int(devices[count].id));

    response = FL_METHOD_RESPONSE(fl_method_success_response_new(deviceInfo));
  }
  else if (strcmp(method, "setDevice") == 0)
  {
    const int id = fl_value_get_int(fl_value_lookup(fl_method_call_get_args(method_call), fl_value_new_string("id")));
    int deviceIndex = fl_value_get_int(fl_value_lookup(fl_method_call_get_args(method_call), fl_value_new_string("device_index")));

    Audio::setDevice(id, deviceIndex);

    response = FL_METHOD_RESPONSE(fl_method_success_response_new(fl_value_new_null()));
  }
  else if (strcmp(method, "load") == 0)
  {
    const int id = fl_value_get_int(fl_value_lookup(fl_method_call_get_args(method_call), fl_value_new_string("id")));
    const gchar *fileName = fl_value_get_string(fl_value_lookup(fl_method_call_get_args(method_call), fl_value_new_string("file_location")));

    Audio::loadPlayer(id, fileName);

    response = FL_METHOD_RESPONSE(fl_method_success_response_new(fl_value_new_null()));
  }
  else if (strcmp(method, "play") == 0)
  {
    const int id = fl_value_get_int(fl_value_lookup(fl_method_call_get_args(method_call), fl_value_new_string("id")));

    Audio::playPlayer(id);

    response = FL_METHOD_RESPONSE(fl_method_success_response_new(fl_value_new_null()));
  }
  else if (strcmp(method, "pause") == 0)
  {
    const int id = fl_value_get_int(fl_value_lookup(fl_method_call_get_args(method_call), fl_value_new_string("id")));

    Audio::pausePlayer(id);

    response = FL_METHOD_RESPONSE(fl_method_success_response_new(fl_value_new_null()));
  }
  else if (strcmp(method, "stop") == 0)
  {
    const int id = fl_value_get_int(fl_value_lookup(fl_method_call_get_args(method_call), fl_value_new_string("id")));

    Audio::stopPlayer(id);

    response = FL_METHOD_RESPONSE(fl_method_success_response_new(fl_value_new_null()));
  }
  else if (strcmp(method, "getDuration") == 0)
  {
    const int id = fl_value_get_int(fl_value_lookup(fl_method_call_get_args(method_call), fl_value_new_string("id")));

    int playerDuration = Audio::getDuration(id);

    g_autoptr(FlValue) playerDurationResponse = fl_value_new_int(playerDuration);

    response = FL_METHOD_RESPONSE(fl_method_success_response_new(playerDurationResponse));
  }
  else if (strcmp(method, "getPosition") == 0)
  {
    const int id = fl_value_get_int(fl_value_lookup(fl_method_call_get_args(method_call), fl_value_new_string("id")));

    int playerPostion = Audio::getPosition(id);

    g_autoptr(FlValue) playerPostionResponse = fl_value_new_int(playerPostion);

    response = FL_METHOD_RESPONSE(fl_method_success_response_new(playerPostionResponse));
  }
  else if (strcmp(method, "setPosition") == 0)
  {
    const int id = fl_value_get_int(fl_value_lookup(fl_method_call_get_args(method_call), fl_value_new_string("id")));
    // in Miliseconds
    int duration = fl_value_get_int(fl_value_lookup(fl_method_call_get_args(method_call), fl_value_new_string("duration")));

    Audio::setPosition(id, duration);

    response = FL_METHOD_RESPONSE(fl_method_success_response_new(fl_value_new_null()));
  }
  else if (strcmp(method, "setVolume") == 0)
  {
    const int id = fl_value_get_int(fl_value_lookup(fl_method_call_get_args(method_call), fl_value_new_string("id")));
    float volume = fl_value_get_float(fl_value_lookup(fl_method_call_get_args(method_call), fl_value_new_string("volume")));

    Audio::setVolume(id, volume);

    response = FL_METHOD_RESPONSE(fl_method_success_response_new(fl_value_new_null()));
  }
    //
    //
    //  *** WAVES ***
    //
    //
  else if (strcmp(method, "loadWave") == 0)
  {
    const int id = fl_value_get_int(fl_value_lookup(fl_method_call_get_args(method_call), fl_value_new_string("id")));
    const float amplitude = fl_value_get_float(fl_value_lookup(fl_method_call_get_args(method_call), fl_value_new_string("amplitude")));
    const float frequency = fl_value_get_float(fl_value_lookup(fl_method_call_get_args(method_call), fl_value_new_string("frequency")));
    const int waveType = fl_value_get_int(fl_value_lookup(fl_method_call_get_args(method_call), fl_value_new_string("wave_type")));
    Audio::loadWave(id, amplitude, frequency, waveType);
    response = FL_METHOD_RESPONSE(fl_method_success_response_new(fl_value_new_null()));
  }
  else if (strcmp(method, "setWaveAmplitude") == 0)
  {
    const int id = fl_value_get_int(fl_value_lookup(fl_method_call_get_args(method_call), fl_value_new_string("id")));

    float amplitude = fl_value_get_float(fl_value_lookup(fl_method_call_get_args(method_call), fl_value_new_string("amplitude")));

    Audio::setWaveAmplitude(id, amplitude);

    response = FL_METHOD_RESPONSE(fl_method_success_response_new(fl_value_new_null()));
  }
  else if (strcmp(method, "setWaveFrequency") == 0)
  {
    const int id = fl_value_get_int(fl_value_lookup(fl_method_call_get_args(method_call), fl_value_new_string("id")));

    float frequency = fl_value_get_float(fl_value_lookup(fl_method_call_get_args(method_call), fl_value_new_string("frequency")));
    Audio::setWaveFrequency(id, frequency);

    response = FL_METHOD_RESPONSE(fl_method_success_response_new(fl_value_new_null()));
  }
  else if (strcmp(method, "setWaveSampleRate") == 0)
  {
    const int id = fl_value_get_int(fl_value_lookup(fl_method_call_get_args(method_call), fl_value_new_string("id")));
    const int sampleRate = fl_value_get_int(fl_value_lookup(fl_method_call_get_args(method_call), fl_value_new_string("sample_rate")));

    Audio::setWaveSampleRate(id, sampleRate);

    response = FL_METHOD_RESPONSE(fl_method_success_response_new(fl_value_new_null()));
  }
  else if (strcmp(method, "setWaveType") == 0)
  {
    const int id = fl_value_get_int(fl_value_lookup(fl_method_call_get_args(method_call), fl_value_new_string("id")));
    const int waveType = fl_value_get_int(fl_value_lookup(fl_method_call_get_args(method_call), fl_value_new_string("wave_type")));

    Audio::setWaveType(id, waveType);

    response = FL_METHOD_RESPONSE(fl_method_success_response_new(fl_value_new_null()));
  }
    //
    //
    //  *** NOISE ***
    //
    //
  else if (strcmp(method, "loadNoise") == 0)
  {
    const int id = fl_value_get_int(fl_value_lookup(fl_method_call_get_args(method_call), fl_value_new_string("id")));
    const float amplitude = fl_value_get_float(fl_value_lookup(fl_method_call_get_args(method_call), fl_value_new_string("amplitude")));
    const int seed = fl_value_get_int(fl_value_lookup(fl_method_call_get_args(method_call), fl_value_new_string("seed")));
    const int noiseType = fl_value_get_int(fl_value_lookup(fl_method_call_get_args(method_call), fl_value_new_string("noise_type")));
    Audio::loadNoise(id, seed, amplitude, noiseType);
    response = FL_METHOD_RESPONSE(fl_method_success_response_new(fl_value_new_null()));
  }
  else if (strcmp(method, "setNoiseAmplitude") == 0)
  {
    const int id = fl_value_get_int(fl_value_lookup(fl_method_call_get_args(method_call), fl_value_new_string("id")));

    float amplitude = fl_value_get_float(fl_value_lookup(fl_method_call_get_args(method_call), fl_value_new_string("amplitude")));

    Audio::setNoiseAmplitude(id, amplitude);

    response = FL_METHOD_RESPONSE(fl_method_success_response_new(fl_value_new_null()));
  }
  else if (strcmp(method, "setNoiseSeed") == 0)
  {
    const int id = fl_value_get_int(fl_value_lookup(fl_method_call_get_args(method_call), fl_value_new_string("id")));

    int seed = fl_value_get_int(fl_value_lookup(fl_method_call_get_args(method_call), fl_value_new_string("seed")));
    Audio::setNoiseSeed(id, seed);

    response = FL_METHOD_RESPONSE(fl_method_success_response_new(fl_value_new_null()));
  }  
  else if (strcmp(method, "setNoiseType") == 0)
  {
    const int id = fl_value_get_int(fl_value_lookup(fl_method_call_get_args(method_call), fl_value_new_string("id")));

    int noiseType = fl_value_get_int(fl_value_lookup(fl_method_call_get_args(method_call), fl_value_new_string("noise_type")));
    Audio::setNoiseType(id, noiseType);

    response = FL_METHOD_RESPONSE(fl_method_success_response_new(fl_value_new_null()));
  }
  else
  {
    response = FL_METHOD_RESPONSE(fl_method_not_implemented_response_new());
  }
  fl_method_call_respond(method_call, response, nullptr);
}

static void flutter_audio_desktop_plugin_dispose(GObject *object)
{
  G_OBJECT_CLASS(flutter_audio_desktop_plugin_parent_class)->dispose(object);
}

static void flutter_audio_desktop_plugin_class_init(FlutterAudioDesktopPluginClass *klass)
{
  G_OBJECT_CLASS(klass)->dispose = flutter_audio_desktop_plugin_dispose;
}

static void flutter_audio_desktop_plugin_init(FlutterAudioDesktopPlugin *self) {}

static void method_call_cb(FlMethodChannel *channel, FlMethodCall *method_call,
                           gpointer user_data)
{
  FlutterAudioDesktopPlugin *plugin = FLUTTER_AUDIO_DESKTOP_PLUGIN(user_data);
  flutter_audio_desktop_plugin_handle_method_call(plugin, method_call);
}

void flutter_audio_desktop_plugin_register_with_registrar(FlPluginRegistrar *registrar)
{
  FlutterAudioDesktopPlugin *plugin = FLUTTER_AUDIO_DESKTOP_PLUGIN(
      g_object_new(flutter_audio_desktop_plugin_get_type(), nullptr));

  g_autoptr(FlStandardMethodCodec) codec = fl_standard_method_codec_new();
  g_autoptr(FlMethodChannel) channel =
      fl_method_channel_new(fl_plugin_registrar_get_messenger(registrar),
                            "flutter_audio_desktop",
                            FL_METHOD_CODEC(codec));
  fl_method_channel_set_method_call_handler(channel, method_call_cb,
                                            g_object_ref(plugin),
                                            g_object_unref);

  g_object_unref(plugin);
}
