/*

    http://github.com/alexmercerind/flutter_audio_desktop

*/

#include <iostream>
#include <thread>
#include "bass.h"


class AudioLoader {

    public:
    bool debug;
    BASS_DEVICEINFO deviceInfo;
    HSAMPLE audioSample;
    HCHANNEL audioChannel;

    void startService() {
        BASS_Init(-1, 44100, 0, 0, NULL);
        BASS_SetVolume(1);

        if (this->debug) {
            std::cout << "http://github.com/alexmercerind/flutter_audio_desktop" << std::endl;
            BASS_GetDeviceInfo(BASS_GetDevice(), &this->deviceInfo);
            std::cout << "Device Driver: " << deviceInfo.driver << std::endl;
            std::cout << "Device Name  : " << deviceInfo.name << std::endl;
            std::cout << "Device Flags : " << deviceInfo.flags << std::endl;
        }
    }
};


class AudioPlayer : public AudioLoader {

    public:
    AudioPlayer(bool debug = false) {
        this->debug = debug;
        this->startService();
    }

    void enableDebug() {
        this->debug = true;
    }

    void disableDebug() {
        this->debug = false;
    }
    
    void load(const char* audioLocation) {
        this->audioSample = BASS_SampleLoad(FALSE, audioLocation, 0, 0, 1, BASS_SAMPLE_MONO);
        this->audioChannel = BASS_SampleGetChannel(this->audioSample, FALSE);
        if (this->debug) std::cout << "Loaded: " << audioLocation << std::endl;
    }

    void play(bool await = false) {
        BASS_ChannelPlay(this->audioChannel, FALSE);
        if (this->debug) {
            std::string awaitLabel;
            if (await) {awaitLabel = "true";} else awaitLabel = "false";
            std::cout << "Started Playback: await = " << awaitLabel << std::endl; 
        }
        if (await) std::this_thread::sleep_for(std::chrono::seconds(this->getDuration()));;
    }

    void pause() {
        BASS_ChannelPause(this->audioChannel);
        if (this->debug) std::cout << "Paused Playback" << std::endl;
    }

    void stop() {
        BASS_ChannelStop(this->audioChannel);
        if (this->debug) std::cout << "Terminated Playback" << std::endl;
    }

    int getDuration() {
        int duration = BASS_ChannelBytes2Seconds(
            this->audioChannel,
            BASS_ChannelGetLength(this->audioChannel, 0)
        );
        if (this->debug) std::cout << "Audio Duration: " << duration << std::endl;
        return duration;
    }

    int getPosition() {
        int position = BASS_ChannelBytes2Seconds(
            this->audioChannel, 
            BASS_ChannelGetPosition(this->audioChannel, 0)
        );
        if (this->debug) std::cout << "Current Position: " << position << std::endl;
        return position;
    }

    void setPosition(int timeSeconds) {
        BASS_ChannelSetPosition(
            this->audioChannel, 
            BASS_ChannelSeconds2Bytes(this->audioChannel, timeSeconds), 
            BASS_POS_BYTE
        );
        if (this->debug) std::cout << "Changed Playback Position: bytes = " << BASS_ChannelSeconds2Bytes(this->audioChannel, timeSeconds) << std::endl;
    }

    void setVolume(float volume) {
        BASS_SetVolume(volume);
        if (this->debug) std::cout << "Volume: " << volume << std::endl;
    }  
};