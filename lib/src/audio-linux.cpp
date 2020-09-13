/* http://github.com/alexmercerind/flutter_audio_desktop */

#include <AudioPlayer.hpp>


extern "C" {


AudioPlayer *audioPlayer;

void initPlayer(int debug) {
    audioPlayer = new AudioPlayer(static_cast<bool>(debug));
}

void loadPlayer(char* fileLocation) {
    audioPlayer->load(fileLocation);
}

void playPlayer() {
    audioPlayer->play();
}

void pausePlayer() {
    audioPlayer->pause();
}

void stopPlayer() {
    audioPlayer->stop();
}

int getDuration() {
    return audioPlayer->getDuration();
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

int getVolume() {
    return audioPlayer->getVolume();
}


}

int main() {
    AudioPlayer audioPlayer(true);
    audioPlayer.load("./music.mp3");
    audioPlayer.play();
    std::cin.get();
    return 0;
}