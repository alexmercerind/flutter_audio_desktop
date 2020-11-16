/* http://github.com/alexmercerind/flutter_audio_desktop */

#define MINIAUDIO_IMPLEMENTATION
#include "miniaudio.h"
#include <thread>
#include <iostream>
#include <vector>

void dataCallback(ma_device *pDevice, void *pOutput, const void *pInput, ma_uint32 frameCount)
{
    ma_decoder *pDecoder = (ma_decoder *)pDevice->pUserData;
    if (pDecoder == NULL)
    {
        return;
    }
    ma_decoder_read_pcm_frames(pDecoder, pOutput, frameCount);
    (void)pInput;
}

void dataCallbackWave(ma_device *pDevice, void *pOutput, const void *pInput, ma_uint32 frameCount)
{
    ma_waveform *pSineWave;

    pSineWave = (ma_waveform *)pDevice->pUserData;
    MA_ASSERT(pSineWave != NULL);

    ma_waveform_read_pcm_frames(pSineWave, pOutput, frameCount);

    (void)pInput; /* Unused. */
}

class AudioPlayerInternal
{
public:
    ma_format sampleFormat = ma_format_f32;
    int channelCount = 2;
    int sampleRate = 48000;
    int deviceIndex = 0;

    ma_device device;
    ma_device_config deviceConfig;
    ma_decoder decoder;
    ma_device_info *pPlaybackDeviceInfos;
    ma_uint32 playbackDeviceCount;
    ma_waveform_config sineWaveConfig;
    ma_waveform sineWave;

    bool debug;
    int id;
    int audioDurationMilliseconds;

    void showDevices()
    {
        ma_context context;
        if (ma_context_init(NULL, 0, NULL, &context) != MA_SUCCESS)
        {
        }
        ma_device_info *pCaptureDeviceInfos;
        ma_uint32 captureDeviceCount;
        if (ma_context_get_devices(&context, &this->pPlaybackDeviceInfos, &this->playbackDeviceCount, &pCaptureDeviceInfos, &captureDeviceCount) != MA_SUCCESS)
        {
        }
        for (ma_uint32 index = 0; index < this->playbackDeviceCount; index += 1)
        {
            std::cout << index << " - " << this->pPlaybackDeviceInfos[index].name << std::endl;
        }
    }

    void loadFile(const char *file)
    {
        ma_decoder_config config = ma_decoder_config_init(this->sampleFormat, this->channelCount, this->sampleRate);
        ma_decoder_init_file(file, &config, &this->decoder);

        ma_uint64 audioDurationFrames = ma_decoder_get_length_in_pcm_frames(&this->decoder);
        this->audioDurationMilliseconds = ma_calculate_buffer_size_in_milliseconds_from_frames(static_cast<int>(audioDurationFrames), this->sampleRate);
    }

    void initDevice()
    {
        ma_device_id selectedDeviceId = this->pPlaybackDeviceInfos[this->deviceIndex].id;
        this->deviceConfig = ma_device_config_init(ma_device_type_playback);
        this->deviceConfig.playback.pDeviceID = &selectedDeviceId;
        this->deviceConfig.playback.format = this->decoder.outputFormat;
        this->deviceConfig.playback.channels = this->decoder.outputChannels;
        this->deviceConfig.sampleRate = this->decoder.outputSampleRate;
        this->deviceConfig.dataCallback = dataCallback;
        this->deviceConfig.pUserData = &this->decoder;
        ma_device_init(NULL, &this->deviceConfig, &this->device);
        if (this->debug)
        {
            std::cout << "Channel Count: " << this->decoder.outputChannels << std::endl;
            std::cout << "Sample Rate  : " << this->decoder.outputSampleRate << std::endl;
        }
    }

    void initDeviceWave()
    {

        ma_device_id selectedDeviceId = this->pPlaybackDeviceInfos[this->deviceIndex].id;
        this->deviceConfig = ma_device_config_init(ma_device_type_playback);
        this->deviceConfig.playback.pDeviceID = &selectedDeviceId;
        this->deviceConfig.playback.format = this->sampleFormat;
        this->deviceConfig.playback.channels = this->channelCount;
        this->deviceConfig.sampleRate = this->sampleRate;
        this->deviceConfig.dataCallback = dataCallbackWave;
        this->deviceConfig.pUserData = &this->sineWave;
        ma_device_init(NULL, &this->deviceConfig, &this->device);

        if (this->debug)
        {
            std::cout << "Channel Count: " << this->channelCount << std::endl;
            std::cout << "Sample Rate  : " << this->sampleRate << std::endl;
        }
    }
};

class AudioPlayer : public AudioPlayerInternal
{
public:
    AudioPlayer(int id = 1, bool debug = false)
    {
        this->debug = debug;
        this->id = id;
        this->showDevices();
    }

    void setDevice(int index)
    {
        this->deviceIndex = index;
        if (this->debug)
        {
            std::cout << "Selected Device: " << this->deviceIndex << " - " << this->pPlaybackDeviceInfos[deviceIndex].name << std::endl;
        }
    }

    void load(const char *file)
    {
        this->loadFile(file);
        this->initDevice();
        if (this->debug)
        {
            std::cout << "Loaded File: " << file << std::endl;
        }
    }

    void play(bool await = false)
    {
        ma_device_start(&this->device);

        if (this->debug)
        {
            std::cout << std::boolalpha;
            std::cout << "Started Playback. await = " << await << std::endl;
        }
        if (await)
            std::this_thread::sleep_for(std::chrono::milliseconds(this->getDuration()));
    }

    void pause()
    {
        ma_device_stop(&this->device);
        if (this->debug)
        {
            std::cout << "Paused Playback." << std::endl;
        }
    }

    void stop()
    {
        ma_device_stop(&this->device);
        ma_decoder_uninit(&this->decoder);
        if (this->debug)
        {
            std::cout << "Stopped Playback." << std::endl;
        }
    }

    int getDuration()
    {
        if (this->debug)
        {
            std::cout << "Audio Duration: " << this->audioDurationMilliseconds << " ms" << std::endl;
        }
        return this->audioDurationMilliseconds;
    }

    void setPosition(int timeMilliseconds)
    {
        int timeFrames = ma_calculate_buffer_size_in_frames_from_milliseconds(timeMilliseconds, this->sampleRate);
        ma_decoder_seek_to_pcm_frame(&decoder, timeFrames);
        if (this->debug)
        {
            std::cout << "Set Position: " << timeMilliseconds << " ms" << std::endl;
        }
    }

    int getPosition()
    {
        ma_uint64 positionFrames;
        ma_decoder_get_cursor_in_pcm_frames(&this->decoder, &positionFrames);
        int positionMilliseconds = ma_calculate_buffer_size_in_milliseconds_from_frames(static_cast<int>(positionFrames), this->sampleRate);
        if (this->debug)
        {
            std::cout << "Current Position: " << positionMilliseconds << " ms" << std::endl;
        }
        return positionMilliseconds;
    }

    void setVolume(double volume)
    {
        ma_device_set_master_volume(&this->device, static_cast<float>(volume));
        if (this->debug)
        {
            std::cout << "Set Volume: " << volume << std::endl;
        }
    }

    void setWaveFrequency(double frequency)
    {
        ma_waveform_set_frequency(&this->sineWave, frequency);
    }

    void setWaveAmplitude(double amplitude)
    {
        ma_waveform_set_amplitude(&this->sineWave, amplitude);
    }

    void setWaveSampleRate(int waveSampleRate)
    {
        ma_waveform_set_sample_rate(&this->sineWave, waveSampleRate);
    }

    void loadWave(double amplitude, double frequency, int waveType)
    {
        // 0 = sine
        // 1 = square
        // 2 = triangle
        // 3 = sawtooth
        ma_waveform_type maWaveType = ma_waveform_type_sine;
        switch (waveType)
        {
        case 0:
            maWaveType = ma_waveform_type_sine;
            break;
        case 1:
            maWaveType = ma_waveform_type_square;
            break;
        case 2:
            maWaveType = ma_waveform_type_triangle;
            break;
        case 3:
            maWaveType = ma_waveform_type_sawtooth;
            break;
        }

        this->sineWaveConfig =
            ma_waveform_config_init(this->sampleFormat, this->channelCount,
                                    this->sampleRate, maWaveType, amplitude, frequency);
        ma_waveform_init(&this->sineWaveConfig, &this->sineWave);
        this->initDeviceWave();
    }

    float getVolume()
    {
        float volume;
        ma_device_get_master_volume(&this->device, &volume);
        if (this->debug)
        {
            std::cout << "Current Volume: " << volume << std::endl;
        }
        return volume;
    }
};

struct AudioPlayerManager
{
    std::vector <AudioPlayer*> players;
};
