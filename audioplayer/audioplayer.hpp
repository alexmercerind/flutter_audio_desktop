#define MINIAUDIO_IMPLEMENTATION
#include <thread>
#include <iostream>
#include <vector>
#include <string>

#include "miniaudio/miniaudio.h"
#include "miniaudio/miniaudio_engine.h"

#include "internal/audiodevices.hpp"
#include "internal/callbacks.hpp"


class AudioPlayerInternal {
    protected:
    std::vector<AudioDevice*> audioDevices;
    AudioDevice* defaultAudioDevice;
    ma_device device;
    ma_resource_manager resourceManager;
    ma_resource_manager_data_source dataSource;
    bool isLoaded = false;
    
    void initialize(AudioDevice* device) {
        this->audioDevices = AudioDevices::getAll();
        this->defaultAudioDevice = AudioDevices::getDefault();
        this->initDevice(device == nullptr ? this->defaultAudioDevice: device);
        this->initResourceManager();
    }

    void uninitialize() {
        ma_device_uninit(&this->device);
        ma_resource_manager_data_source_uninit(&this->dataSource);
        ma_resource_manager_uninit(&this->resourceManager);
        ma_context_uninit(&deviceContext);
    }

    private:
    ma_device_config deviceConfig;
    ma_resource_manager_config resourceManagerConfig;

    void initDevice(AudioDevice* audioDevice) {
        this->deviceConfig = ma_device_config_init(ma_device_type_playback);
        this->deviceConfig.dataCallback = dataCallbackStream;
        this->deviceConfig.playback.pDeviceID = &audioDevice->info.id;
        this->deviceConfig.pUserData = &this->dataSource;
        ma_device_init(&deviceContext, &this->deviceConfig, &this->device);
    }

    void initResourceManager() {
        this->resourceManagerConfig = ma_resource_manager_config_init();
        this->resourceManagerConfig.decodedFormat     = this->device.playback.format;
        this->resourceManagerConfig.decodedChannels   = this->device.playback.channels;
        this->resourceManagerConfig.decodedSampleRate = this->device.sampleRate;
        ma_resource_manager_init(&resourceManagerConfig, &resourceManager);
    }
};


class AudioPlayer: protected AudioPlayerInternal {
    public:
    AudioPlayer(AudioDevice* audioDevice = nullptr) {
        this->initialize(audioDevice);
    };

    void play(bool isLooping = false) {
        if (this->isLoaded) {
            ma_resource_manager_data_source_set_looping(&this->dataSource, isLooping);
            ma_device_start(&this->device);
        }
    }

    void pause() {
        ma_device_stop(&this->device);
    }

    void load(std::string filePath) {
        this->isLoaded = true;
        ma_resource_manager_data_source_init(
            &this->resourceManager,
            filePath.c_str(),
            MA_DATA_SOURCE_FLAG_STREAM,
            NULL,
            &this->dataSource
        );
    }

    void stop() {
        this->uninitialize();
    }

    void setVolume(float volume) {
        if (this->isLoaded) {
            ma_device_set_master_volume(&this->device, volume);
        }
    }

    void setPosition(int durationMilliseconds) {
        if (this->isLoaded) {
            unsigned long long durationPCMFrame = ma_calculate_buffer_size_in_frames_from_milliseconds(durationMilliseconds, this->device.sampleRate);
            ma_resource_manager_data_source_seek_to_pcm_frame(&this->dataSource, durationPCMFrame);
        }
    }

    int getDuration() {
        if (this->isLoaded) {
            unsigned long long durationPCMFrame;
            ma_resource_manager_data_source_get_length_in_pcm_frames(&this->dataSource, &durationPCMFrame);
            return ma_calculate_buffer_size_in_milliseconds_from_frames(durationPCMFrame, this->device.sampleRate);
        }
        else return 0;
    }

    int getPosition() {
        if (this->isLoaded) {
            unsigned long long durationPCMFrame;
            ma_resource_manager_data_source_get_cursor_in_pcm_frames(&this->dataSource, &durationPCMFrame);
            return ma_calculate_buffer_size_in_milliseconds_from_frames(durationPCMFrame, this->device.sampleRate);
        }
        else return 0;
    }
};
