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

namespace {

class FlutterAudioDesktopPlugin : public flutter::Plugin {
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
    flutter::PluginRegistrarWindows *registrar) {
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
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result) {
  

  if (method_call.method_name() == "init") {
    int debug = std::get<int>(*method_call.arguments());

    Audio::initPlayer(debug);

    result->Success(flutter::EncodableValue(nullptr));
  }

  else if (method_call.method_name() == "setDevice") {
    int deviceIndex = std::get<int>(*method_call.arguments());

    Audio::setDevice(deviceIndex);

    result->Success(flutter::EncodableValue(nullptr));
  }

  else if (method_call.method_name() == "load") {
    std::string fileName = std::get<std::string>(*method_call.arguments());
    
    Audio::loadPlayer(fileName.c_str());
    
    result->Success(flutter::EncodableValue(nullptr));
  }

  else if (method_call.method_name() == "play") {
    Audio::playPlayer();

    result->Success(flutter::EncodableValue(nullptr));
  }

  else if (method_call.method_name() == "pause") {
    Audio::pausePlayer();
    
    result->Success(flutter::EncodableValue(nullptr));
  }

  else if (method_call.method_name() == "stop") {
    Audio::stopPlayer();

    result->Success(flutter::EncodableValue(nullptr));
  }

  else if (method_call.method_name() == "getDuration") {
    int playerDuration = Audio::getDuration();
    
    result->Success(flutter::EncodableValue(playerDuration));
  }

  else if (method_call.method_name() == "getPosition") {
    int playerPosition = Audio::getPosition();

    result->Success(flutter::EncodableValue(playerPosition));
  }

  else if (method_call.method_name() == "setPosition") {
    int timeMilliseconds = std::get<int>(*method_call.arguments());

    Audio::setPosition(timeMilliseconds);

    result->Success(flutter::EncodableValue(nullptr));
  }

  else if (method_call.method_name() == "setVolume") {
    double volume = std::get<double>(*method_call.arguments());

    Audio::setVolume(volume);

    result->Success(flutter::EncodableValue(nullptr));
  }

  else {
    result->NotImplemented();
  }
}

}

void FlutterAudioDesktopPluginRegisterWithRegistrar(
    FlutterDesktopPluginRegistrarRef registrar) {
  FlutterAudioDesktopPlugin::RegisterWithRegistrar(
      flutter::PluginRegistrarManager::GetInstance()
          ->GetRegistrar<flutter::PluginRegistrarWindows>(registrar));
}
