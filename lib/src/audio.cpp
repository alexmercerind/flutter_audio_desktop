/*

    http://github.com/alexmercerind/flutter_audio_desktop

*/

#include "audio.hpp"

extern "C" {
    AudioPlayer *audioPlayer;

    void init(int debug) {
        audioPlayer = new AudioPlayer(static_cast<bool>(debug));
    }

    void load(char* fileLocation) {
        audioPlayer->load(fileLocation);
    }

    void play() {
        audioPlayer->play();
    }

    void pause() {
        audioPlayer->pause();
    }

    int getDuration() {
        return audioPlayer->getDuration();
    }

    void stop() {
        audioPlayer->stop();
    }

    int getPosition() {
        return audioPlayer->getPosition();
    }

    void setPosition(int timeSeconds) {
        audioPlayer->setPosition(timeSeconds);
    }

    void setVolume(double volume) {
        audioPlayer->setVolume(volume);
    }
}

int main() {
    return 0;
}