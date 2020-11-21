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

struct AudioDevice
{
    int id;
    std::string name;
    ma_device_id deviceID;
};

class AudioPlayerInternal
{
public:
    ma_format sampleFormat = ma_format_f32;
    int channelCount = 2;
    int sampleRate = 48000;
    int deviceIndex = 0;
    int deviceCount = 0;
    bool fileLoaded = false;
    bool waveLoaded = false;
    bool isPlaying = false;

    ma_device device;
    ma_device_config deviceConfig;
    ma_decoder decoder;
    ma_device_info *pPlaybackDeviceInfos;
    ma_uint32 playbackDeviceCount = 0;
    ma_waveform_config sineWaveConfig;
    ma_waveform sineWave;

    bool debug;
    int id;
    int audioDurationMilliseconds;

    int findDevices()
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
        this->deviceCount = 0;
        for (ma_uint32 index = 0; index < this->playbackDeviceCount; index += 1)
        {
            this->deviceCount += 1;
        }
        return this->deviceCount;
    }

    void getDevices(AudioDevice devices[])
    {
        int count = this->deviceCount;
        for (int index = 0; index < count; index += 1)
        {
            if (this->pPlaybackDeviceInfos[index].isDefault)
            {
                devices[count].name = this->pPlaybackDeviceInfos[index].name;
                devices[count].id = index;
                devices[count].deviceID = this->pPlaybackDeviceInfos[index].id;
            }
            devices[index].name = this->pPlaybackDeviceInfos[index].name;
            devices[index].id = index;
            devices[index].deviceID = this->pPlaybackDeviceInfos[index].id;
        }
    }

    ma_device_info getDefaultDevice()
    {
        findDevices();
        ma_device_info defaultDevice = this->pPlaybackDeviceInfos[0];
        for (ma_uint32 index = 0; index < this->playbackDeviceCount; index++)
        {
            // default becomes system default
            if (this->pPlaybackDeviceInfos[index].isDefault)
            {
                defaultDevice = this->pPlaybackDeviceInfos[index];
            }
        }
        return defaultDevice;
    }

    void loadFile(const char *file)
    {
        ma_decoder_config config = ma_decoder_config_init(this->sampleFormat, this->channelCount, this->sampleRate);
        ma_decoder_init_file(file, &config, &this->decoder);

        ma_uint64 audioDurationFrames = ma_decoder_get_length_in_pcm_frames(&this->decoder);
        this->audioDurationMilliseconds = ma_calculate_buffer_size_in_milliseconds_from_frames(static_cast<int>(audioDurationFrames), this->sampleRate);
    }

    void initDevice(ma_device_id selectedDeviceId)
    {
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
            // TODO: Move debug output to message channel
            //std::cout << "Channel Count: " << this->decoder.outputChannels << std::endl;
            //std::cout << "Sample Rate  : " << this->decoder.outputSampleRate << std::endl;
        }
    }

    void initDeviceWave(ma_device_id selectedDeviceId)
    {

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
            //std::cout << "Channel Count: " << this->channelCount << std::endl;
            //std::cout << "Sample Rate  : " << this->sampleRate << std::endl;
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
    }

    void setDevice(int index)
    {
        if (this->fileLoaded == true || this->waveLoaded == true)
        {
            // Get count, new array, fill array
            int count = this->findDevices();
            AudioDevice *devices = new AudioDevice[count + 1];
            this->getDevices(devices);
            ma_device_stop(&this->device);
            ma_device_uninit(&this->device);

            if (this->fileLoaded == true)
            {
                this->initDevice(devices[index].deviceID);
            }
            else if (this->waveLoaded == true)
            {
                this->initDeviceWave(devices[index].deviceID);
            }

            if (this->isPlaying == true)
            {
                ma_device_start(&this->device);
            }
            else
            {
                ma_device_stop(&this->device);
            }

            this->deviceIndex = index;
            if (this->debug)
            {
                // TODO: Move debug output to message channel
                //std::cout << "Selected Device: " << this->deviceIndex << " - " << this->pPlaybackDeviceInfos[deviceIndex].name << std::endl;
            }
        }
    }

    void load(const char *file)
    {
        this->loadFile(file);
        ma_device_id selectedDeviceId = this->getDefaultDevice().id;
        this->initDevice(selectedDeviceId);
        this->waveLoaded = false;
        this->fileLoaded = true;
        if (this->debug)
        {
            // TODO: Move debug output to message channel
            //std::cout << "Loaded File: " << file << std::endl;
        }
    }

    void play(bool await = false)
    {
        this->isPlaying = true;
        ma_device_start(&this->device);

        if (this->debug)
        {
            // TODO: Move debug output to message channel
            //std::cout << std::boolalpha;
            //std::cout << "Started Playback. await = " << await << std::endl;
        }
        if (await)
            std::this_thread::sleep_for(std::chrono::milliseconds(this->getDuration()));
    }

    void pause()
    {
        this->isPlaying = false;
        ma_device_stop(&this->device);
        if (this->debug)
        {
            // TODO: Move debug output to message channel
            //std::cout << "Paused Playback." << std::endl;
        }
    }

    void stop()
    {
        this->isPlaying = false;
        ma_device_stop(&this->device);
        //ma_decoder_uninit(&this->decoder);
        if (this->debug)
        {
            // TODO: Move debug output to message channel
            //std::cout << "Stopped Playback." << std::endl;
        }
    }

    int getDuration()
    {
        if (this->debug)
        {
            // TODO: Move debug output to message channel
            //std::cout << "Audio Duration: " << this->audioDurationMilliseconds << " ms" << std::endl;
        }
        return this->audioDurationMilliseconds;
    }

    void setPosition(int timeMilliseconds)
    {
        int timeFrames = ma_calculate_buffer_size_in_frames_from_milliseconds(timeMilliseconds, this->sampleRate);
        ma_decoder_seek_to_pcm_frame(&decoder, timeFrames);
        if (this->debug)
        {
            // TODO: Move debug output to message channel
            //std::cout << "Set Position: " << timeMilliseconds << " ms" << std::endl;
        }
    }

    int getPosition()
    {
        ma_uint64 positionFrames;
        ma_decoder_get_cursor_in_pcm_frames(&this->decoder, &positionFrames);
        int positionMilliseconds = ma_calculate_buffer_size_in_milliseconds_from_frames(static_cast<int>(positionFrames), this->sampleRate);
        if (this->debug)
        {
            // TODO: Move debug output to message channel
            //std::cout << "Current Position: " << positionMilliseconds << " ms" << std::endl;
        }
        return positionMilliseconds;
    }

    float getVolume()
    {
        float volume;
        ma_device_get_master_volume(&this->device, &volume);
        if (this->debug)
        {
            // TODO: Move debug output to message channel
            //std::cout << "Current Volume: " << volume << std::endl;
        }
        return volume;
    }

    void setVolume(double volume)
    {
        ma_device_set_master_volume(&this->device, static_cast<float>(volume));
        if (this->debug)
        {
            // TODO: Move debug output to message channel
            //std::cout << "Set Volume: " << volume << std::endl;
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
    
    void setWaveType(int waveType)
    {
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

        ma_waveform_set_type(&this->sineWave, maWaveType);
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
        ma_device_id selectedDeviceId = this->getDefaultDevice().id;
        this->initDeviceWave(selectedDeviceId);
        this->waveLoaded = true;
        this->fileLoaded = false;
    }
};

struct AudioPlayerManager
{
    std::vector<AudioPlayer *> players;
};
