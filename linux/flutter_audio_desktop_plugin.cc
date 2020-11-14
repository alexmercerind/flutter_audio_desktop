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
    int debug = fl_value_get_int(fl_method_call_get_args(method_call));

    Audio::initPlayer(debug);

    response = FL_METHOD_RESPONSE(fl_method_success_response_new(fl_value_new_null()));
  }

  if (strcmp(method, "setDevice") == 0)
  {
    int deviceIndex = fl_value_get_int(fl_method_call_get_args(method_call));

    Audio::setDevice(deviceIndex);

    response = FL_METHOD_RESPONSE(fl_method_success_response_new(fl_value_new_null()));
  }

  else if (strcmp(method, "load") == 0)
  {
    const gchar *fileName = fl_value_get_string(fl_method_call_get_args(method_call));

    Audio::loadPlayer(fileName);

    response = FL_METHOD_RESPONSE(fl_method_success_response_new(fl_value_new_null()));
  }

  else if (strcmp(method, "loadWave") == 0)
  {
    const float amplitude = fl_value_get_float(fl_value_lookup(fl_method_call_get_args(method_call), fl_value_new_string("amplitude")));
    const float frequency = fl_value_get_float(fl_value_lookup(fl_method_call_get_args(method_call), fl_value_new_string("frequency")));
    const int waveType = fl_value_get_int(fl_value_lookup(fl_method_call_get_args(method_call), fl_value_new_string("waveType")));
    Audio::loadWave(amplitude, frequency, waveType);
    response = FL_METHOD_RESPONSE(fl_method_success_response_new(fl_value_new_null()));
  }

  else if (strcmp(method, "play") == 0)
  {
    Audio::playPlayer();

    response = FL_METHOD_RESPONSE(fl_method_success_response_new(fl_value_new_null()));
  }

  else if (strcmp(method, "pause") == 0)
  {
    Audio::pausePlayer();

    response = FL_METHOD_RESPONSE(fl_method_success_response_new(fl_value_new_null()));
  }

  else if (strcmp(method, "stop") == 0)
  {
    Audio::stopPlayer();

    response = FL_METHOD_RESPONSE(fl_method_success_response_new(fl_value_new_null()));
  }

  else if (strcmp(method, "getDuration") == 0)
  {
    int playerDuration = Audio::getDuration();

    g_autoptr(FlValue) playerDurationResponse = fl_value_new_int(playerDuration);

    response = FL_METHOD_RESPONSE(fl_method_success_response_new(playerDurationResponse));
  }

  else if (strcmp(method, "getPosition") == 0)
  {
    int playerPostion = Audio::getPosition();

    g_autoptr(FlValue) playerPostionResponse = fl_value_new_int(playerPostion);

    response = FL_METHOD_RESPONSE(fl_method_success_response_new(playerPostionResponse));
  }

  else if (strcmp(method, "setPosition") == 0)
  {

    int timeMilliseconds = fl_value_get_int(fl_method_call_get_args(method_call));

    Audio::setPosition(timeMilliseconds);

    response = FL_METHOD_RESPONSE(fl_method_success_response_new(fl_value_new_null()));
  }
    else if (strcmp(method, "setWaveAmplitude") == 0)
  {
    float amplitude = fl_value_get_float(fl_method_call_get_args(method_call));

    Audio::setWaveAmplitude(amplitude);

    response = FL_METHOD_RESPONSE(fl_method_success_response_new(fl_value_new_null()));
  }
    else if (strcmp(method, "setWaveFrequency") == 0)
  {
    float frequency = fl_value_get_float(fl_method_call_get_args(method_call));

    Audio::setWaveFrequency(frequency);

    response = FL_METHOD_RESPONSE(fl_method_success_response_new(fl_value_new_null()));
  }
    else if (strcmp(method, "setWaveSampleRate") == 0)
  {
    int sampleRate = fl_value_get_int(fl_method_call_get_args(method_call));

    Audio::setWaveSampleRate(sampleRate);

    response = FL_METHOD_RESPONSE(fl_method_success_response_new(fl_value_new_null()));
  }

  else if (strcmp(method, "setVolume") == 0)
  {
    float volume = fl_value_get_float(fl_method_call_get_args(method_call));

    Audio::setVolume(volume);

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
