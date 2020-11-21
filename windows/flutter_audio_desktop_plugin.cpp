#include "include/flutter_audio_desktop/flutter_audio_desktop_plugin.h"

#include <windows.h>

#include <flutter/method_channel.h>
#include <flutter/plugin_registrar_windows.h>
#include <flutter/standard_method_codec.h>

#include <map>
#include <string>
#include <memory>
#include "../audioplayer/audio.cpp"
#include <sstream>

namespace
{

  class FlutterAudioDesktopPlugin : public flutter::Plugin
  {
  public:
    static void RegisterWithRegistrar(flutter::PluginRegistrarWindows *registrar);

    FlutterAudioDesktopPlugin();

    virtual ~FlutterAudioDesktopPlugin();

  private:
    void HandleMethodCall(
        const flutter::MethodCall<flutter::EncodableValue> &method_call,
        std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result);
  };

  void FlutterAudioDesktopPlugin::RegisterWithRegistrar(
      flutter::PluginRegistrarWindows *registrar)
  {
    auto channel =
        std::make_unique<flutter::MethodChannel<flutter::EncodableValue>>(
            registrar->messenger(), "flutter_audio_desktop",
            &flutter::StandardMethodCodec::GetInstance());

    auto plugin = std::make_unique<FlutterAudioDesktopPlugin>();

    channel->SetMethodCallHandler(
        [plugin_pointer = plugin.get()](const auto &call, auto result) {
          plugin_pointer->HandleMethodCall(call, std::move(result));
        });

    registrar->AddPlugin(std::move(plugin));
  }

  FlutterAudioDesktopPlugin::FlutterAudioDesktopPlugin() {}

  FlutterAudioDesktopPlugin::~FlutterAudioDesktopPlugin() {}

  void FlutterAudioDesktopPlugin::HandleMethodCall(
      const flutter::MethodCall<flutter::EncodableValue> &method_call,
      std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result)
  {

    if (method_call.method_name() == "init")
    {
      const auto *arguments = std::get_if<flutter::EncodableMap>(method_call.arguments());

      // Map Player ID
      int playerID = 0;
      auto encodedID = arguments->find(flutter::EncodableValue("id"));
      if (encodedID != arguments->end())
      {
        playerID = std::get<int>(encodedID->second);
      }
      // Map debug status
      bool debug = false;
      auto encodedDebug = arguments->find(flutter::EncodableValue("debug"));
      if (encodedDebug != arguments->end())
      {
        debug = std::get<bool>(encodedDebug->second);
      }

      Audio::initPlayer(playerID, debug);

      result->Success(flutter::EncodableValue(nullptr));
    }
    else if (method_call.method_name() == "getDevices")
    {
      // Map Device Index
      int count = Audio::getDeviceCount();
      AudioDevice *devices = new AudioDevice[count + 1];
      Audio::getDevices(devices);

      auto deviceInfo = flutter::EncodableMap();
      for (int i = 0; i < count; i++)
      {
        deviceInfo.insert_or_assign(flutter::EncodableValue(std::to_string(i).c_str()),
                                    flutter::EncodableValue(devices[i].name.c_str()));
      }
      deviceInfo.insert_or_assign(flutter::EncodableValue("default"),
                                  flutter::EncodableValue(devices[count].id));

      result->Success(flutter::EncodableValue(deviceInfo));
    }
    else if (method_call.method_name() == "setDevice")
    {
      const auto *arguments = std::get_if<flutter::EncodableMap>(method_call.arguments());

      // Map Player ID
      int playerID = 0;
      auto encodedID = arguments->find(flutter::EncodableValue("id"));
      if (encodedID != arguments->end())
      {
        playerID = std::get<int>(encodedID->second);
      }

      // Map Device Index
      int deviceIndex = 0;
      auto encodedDeviceIndex = arguments->find(flutter::EncodableValue("device_index"));
      if (encodedDeviceIndex != arguments->end())
      {
        deviceIndex = std::get<int>(encodedDeviceIndex->second);
      }
      Audio::setDevice(playerID, deviceIndex);

      result->Success(flutter::EncodableValue(nullptr));
    }

    else if (method_call.method_name() == "load")
    {

      const auto *arguments = std::get_if<flutter::EncodableMap>(method_call.arguments());

      // Map Player ID
      int playerID = 0;
      auto encodedID = arguments->find(flutter::EncodableValue("id"));
      if (encodedID != arguments->end())
      {
        playerID = std::get<int>(encodedID->second);
      }

      // Map File Location
      std::string fileLocation = "";
      auto encodedFileLocation = arguments->find(flutter::EncodableValue("file_location"));
      if (encodedFileLocation != arguments->end())
      {
        fileLocation = std::get<std::string>(encodedFileLocation->second);
      }
      Audio::loadPlayer(playerID, fileLocation.c_str());

      result->Success(flutter::EncodableValue(nullptr));
    }

    else if (method_call.method_name() == "play")
    {
      const auto *arguments = std::get_if<flutter::EncodableMap>(method_call.arguments());

      // Map Player ID
      int playerID = 0;
      auto encodedID = arguments->find(flutter::EncodableValue("id"));
      if (encodedID != arguments->end())
      {
        playerID = std::get<int>(encodedID->second);
      }
      Audio::playPlayer(playerID);

      result->Success(flutter::EncodableValue(nullptr));
    }
    else if (method_call.method_name() == "loadWave")
    {
      const auto *arguments = std::get_if<flutter::EncodableMap>(method_call.arguments());

      // Map Player ID
      int playerID = 0;
      auto encodedID = arguments->find(flutter::EncodableValue("id"));
      if (encodedID != arguments->end())
      {
        playerID = std::get<int>(encodedID->second);
      }

      // Map amplitude
      double amplitude = 0;
      auto encodedAmplitude = arguments->find(flutter::EncodableValue("amplitude"));
      if (encodedAmplitude != arguments->end())
      {
        amplitude = std::get<double>(encodedAmplitude->second);
      }

      // Map frequency
      double frequency = 0;
      auto encodedFrequency = arguments->find(flutter::EncodableValue("frequency"));
      if (encodedFrequency != arguments->end())
      {
        frequency = std::get<double>(encodedFrequency->second);
      }

      // Map wave type
      int waveType = 0;
      auto encodedWaveType = arguments->find(flutter::EncodableValue("wave_type"));
      if (encodedWaveType != arguments->end())
      {
        waveType = std::get<int>(encodedWaveType->second);
      }

      Audio::loadWave(playerID, amplitude, frequency, waveType);

      result->Success(flutter::EncodableValue(nullptr));
    }

    else if (method_call.method_name() == "pause")
    {
      // Get args
      const auto *arguments = std::get_if<flutter::EncodableMap>(method_call.arguments());

      // Map Player ID
      int playerID = 0;
      auto encodedID = arguments->find(flutter::EncodableValue("id"));
      if (encodedID != arguments->end())
      {
        playerID = std::get<int>(encodedID->second);
      }

      Audio::pausePlayer(playerID);

      result->Success(flutter::EncodableValue(nullptr));
    }

    else if (method_call.method_name() == "stop")
    {
      // Get args
      const auto *arguments = std::get_if<flutter::EncodableMap>(method_call.arguments());

      // Map Player ID
      int playerID = 0;
      auto encodedID = arguments->find(flutter::EncodableValue("id"));
      if (encodedID != arguments->end())
      {
        playerID = std::get<int>(encodedID->second);
      }

      Audio::stopPlayer(playerID);

      result->Success(flutter::EncodableValue(nullptr));
    }

    else if (method_call.method_name() == "getDuration")
    {
      // Get args
      const auto *arguments = std::get_if<flutter::EncodableMap>(method_call.arguments());

      // Map Player ID
      int playerID = 0;
      auto encodedID = arguments->find(flutter::EncodableValue("id"));
      if (encodedID != arguments->end())
      {
        playerID = std::get<int>(encodedID->second);
      }

      int playerDuration = Audio::getDuration(playerID);

      result->Success(flutter::EncodableValue(playerDuration));
    }

    else if (method_call.method_name() == "getPosition")
    {
      // Get args
      const auto *arguments = std::get_if<flutter::EncodableMap>(method_call.arguments());

      // Map Player ID
      int playerID = 0;
      auto encodedID = arguments->find(flutter::EncodableValue("id"));
      if (encodedID != arguments->end())
      {
        playerID = std::get<int>(encodedID->second);
      }

      int playerPosition = Audio::getPosition(playerID);

      result->Success(flutter::EncodableValue(playerPosition));
    }

    else if (method_call.method_name() == "setPosition")
    {
      // Get args
      const auto *arguments = std::get_if<flutter::EncodableMap>(method_call.arguments());

      // Map Player ID
      int playerID = 0;
      auto encodedID = arguments->find(flutter::EncodableValue("id"));
      if (encodedID != arguments->end())
      {
        playerID = std::get<int>(encodedID->second);
      }

      int duration = 0;
      auto encodedDuration = arguments->find(flutter::EncodableValue("duration"));
      if (encodedDuration != arguments->end())
      {
        duration = std::get<int>(encodedDuration->second);
      }

      Audio::setPosition(playerID, duration);

      result->Success(flutter::EncodableValue(nullptr));
    }

    else if (method_call.method_name() == "setVolume")
    {
      // Get args
      const auto *arguments = std::get_if<flutter::EncodableMap>(method_call.arguments());

      // Map Player ID
      int playerID = 0;
      auto encodedID = arguments->find(flutter::EncodableValue("id"));
      if (encodedID != arguments->end())
      {
        playerID = std::get<int>(encodedID->second);
      }

      // Map volume
      double volume = 0;
      auto encodedVolume = arguments->find(flutter::EncodableValue("volume"));
      if (encodedVolume != arguments->end())
      {
        volume = std::get<double>(encodedVolume->second);
      }
      Audio::setVolume(playerID, volume);

      result->Success(flutter::EncodableValue(nullptr));
    }
    else if (method_call.method_name() == "setWaveAmplitude")
    {
      // Get args
      const auto *arguments = std::get_if<flutter::EncodableMap>(method_call.arguments());

      // Map Player ID
      int playerID = 0;
      auto encodedID = arguments->find(flutter::EncodableValue("id"));
      if (encodedID != arguments->end())
      {
        playerID = std::get<int>(encodedID->second);
      }

      // Map volume
      double amplitude = 0;
      auto encodedAmplitude = arguments->find(flutter::EncodableValue("amplitude"));
      if (encodedAmplitude != arguments->end())
      {
        amplitude = std::get<double>(encodedAmplitude->second);
      }
      Audio::setWaveAmplitude(playerID, amplitude);

      result->Success(flutter::EncodableValue(nullptr));
    }
    else if (method_call.method_name() == "setWaveFrequency")
    {
      // Get args
      const auto *arguments = std::get_if<flutter::EncodableMap>(method_call.arguments());

      // Map Player ID
      int playerID = 0;
      auto encodedID = arguments->find(flutter::EncodableValue("id"));
      if (encodedID != arguments->end())
      {
        playerID = std::get<int>(encodedID->second);
      }

      // Map volume
      double frequency = 0;
      auto encodedFrequency = arguments->find(flutter::EncodableValue("frequency"));
      if (encodedFrequency != arguments->end())
      {
        frequency = std::get<double>(encodedFrequency->second);
      }
      Audio::setWaveFrequency(playerID, frequency);

      result->Success(flutter::EncodableValue(nullptr));
    }
    else if (method_call.method_name() == "setWaveSampleRate")
    {
      // Get args
      const auto *arguments = std::get_if<flutter::EncodableMap>(method_call.arguments());

      // Map Player ID
      int playerID = 0;
      auto encodedID = arguments->find(flutter::EncodableValue("id"));
      if (encodedID != arguments->end())
      {
        playerID = std::get<int>(encodedID->second);
      }

      // Map volume
      int waveSampleRate = 0;
      auto encodedSampleRate = arguments->find(flutter::EncodableValue("sample_rate"));
      if (encodedSampleRate != arguments->end())
      {
        waveSampleRate = std::get<int>(encodedSampleRate->second);
      }
      Audio::setWaveSampleRate(playerID, waveSampleRate);

      result->Success(flutter::EncodableValue(nullptr));
    }else if (method_call.method_name() == "setWaveType")
    {
      // Get args
      const auto *arguments = std::get_if<flutter::EncodableMap>(method_call.arguments());

      // Map Player ID
      int playerID = 0;
      auto encodedID = arguments->find(flutter::EncodableValue("id"));
      if (encodedID != arguments->end())
      {
        playerID = std::get<int>(encodedID->second);
      }

      // Map volume
      int waveType = 0;
      auto encodedWaveType = arguments->find(flutter::EncodableValue("wave_type"));
      if (encodedWaveType != arguments->end())
      {
        waveType = std::get<int>(encodedWaveType->second);
      }
      Audio::setWaveType(playerID, waveType);

      result->Success(flutter::EncodableValue(nullptr));
    }

    else
    {
      result->NotImplemented();
    }
  }

} // namespace

void FlutterAudioDesktopPluginRegisterWithRegistrar(
    FlutterDesktopPluginRegistrarRef registrar)
{
  FlutterAudioDesktopPlugin::RegisterWithRegistrar(
      flutter::PluginRegistrarManager::GetInstance()
          ->GetRegistrar<flutter::PluginRegistrarWindows>(registrar));
}
