/* http://github.com/alexmercerind/flutter_audio_desktop */

#include <AudioPlayer.hpp>


extern "C" {


AudioPlayer *audioPlayer;

__declspec( dllexport ) void initPlayer(int debug) {
    audioPlayer = new AudioPlayer(static_cast<bool>(debug));
}

__declspec( dllexport ) void loadPlayer(char* fileLocation) {
    audioPlayer->load(fileLocation);
}

__declspec( dllexport ) void playPlayer() {
    audioPlayer->play();
}

__declspec( dllexport ) void pausePlayer() {
    audioPlayer->pause();
}

__declspec( dllexport ) void stopPlayer() {
    audioPlayer->stop();
}

__declspec( dllexport ) int getDuration() {
    return audioPlayer->getDuration();
}

__declspec( dllexport ) int getPosition() {
    return audioPlayer->getPosition();
}

__declspec( dllexport ) void setPosition(int timeSeconds) {
    audioPlayer->setPosition(timeSeconds);
}

__declspec( dllexport ) void setVolume(double volume) {
    audioPlayer->setVolume(volume);
}

__declspec( dllexport ) int getVolume() {
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