/* http://github.com/alexmercerind/flutter_audio_desktop */

#include "audioplayer.hpp"

namespace Audio
{

    AudioPlayerManager *audioPlayerManager = new AudioPlayerManager();

    void initPlayer(int id, bool debug)
    {
        AudioPlayer *audioPlayer = new AudioPlayer(id, debug);
        audioPlayer->findDevices();
        audioPlayerManager->players.insert(audioPlayerManager->players.end(), audioPlayer);
    }

    int getDeviceCount()
    {
        AudioPlayer *audioPlayer = audioPlayerManager->players.at(0);
        return audioPlayer->playbackDeviceCount;
    }

    void getDevices(AudioDevice devices[])
    {
        AudioPlayer *audioPlayer = audioPlayerManager->players.at(0);
        audioPlayer->getDevices(devices);
    }

    void setDevice(int id, int deviceIndex)
    {
        AudioPlayer *audioPlayer = audioPlayerManager->players.at(id);
        audioPlayer->setDevice(deviceIndex);
    }

    void loadPlayer(int id, const char *fileLocation)
    {
        auto audioPlayer = audioPlayerManager->players.at(id);
        audioPlayer->load(fileLocation);
    }

    void playPlayer(int id)
    {
        auto audioPlayer = audioPlayerManager->players.at(id);
        audioPlayer->play();
    }

    void pausePlayer(int id)
    {
        auto audioPlayer = audioPlayerManager->players.at(id);
        audioPlayer->pause();
    }

    void stopPlayer(int id)
    {
        auto audioPlayer = audioPlayerManager->players.at(id);
        audioPlayer->stop();
    }

    int getDuration(int id)
    {
        auto audioPlayer = audioPlayerManager->players.at(id);
        return audioPlayer->getDuration();
    }

    int getPosition(int id)
    {
        auto audioPlayer = audioPlayerManager->players.at(id);
        return audioPlayer->getPosition();
    }

    void setPosition(int id, int timeMilliseconds)
    {
        auto audioPlayer = audioPlayerManager->players.at(id);
        audioPlayer->setPosition(timeMilliseconds);
    }

    void setVolume(int id, double volume)
    {
        auto audioPlayer = audioPlayerManager->players.at(id);
        audioPlayer->setVolume(volume);
    }

    void setWaveFrequency(int id, double frequency)
    {
        auto audioPlayer = audioPlayerManager->players.at(id);
        audioPlayer->setWaveFrequency(frequency);
    }

    void setWaveAmplitude(int id, double amplitude)
    {
        auto audioPlayer = audioPlayerManager->players.at(id);
        audioPlayer->setWaveAmplitude(amplitude);
    }

    void setWaveSampleRate(int id, int sampleRate)
    {
        auto audioPlayer = audioPlayerManager->players.at(id);
        audioPlayer->setWaveSampleRate(sampleRate);
    }

    void setWaveType(int id, int waveType)
    {
        auto audioPlayer = audioPlayerManager->players.at(id);
        audioPlayer->setWaveType(waveType);
    }

    void loadWave(int id, double amplitude, double frequency, int waveType)
    {
        auto audioPlayer = audioPlayerManager->players.at(id);
        audioPlayer->loadWave(amplitude, frequency, waveType);
    }

    float getVolume(int id)
    {
        auto audioPlayer = audioPlayerManager->players.at(id);
        return audioPlayer->getVolume();
    }
} // namespace Audio

int main(int argc, const char **argv)
{
    AudioPlayer audioPlayer(true);
    audioPlayer.load(argv[2]);
    audioPlayer.play();
    std::cin.get();
    return 0;
}