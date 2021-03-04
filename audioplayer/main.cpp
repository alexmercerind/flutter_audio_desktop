#include <map>
#include <iostream>

#include "audioplayer.hpp"


class AudioPlayers {
    public:
    std::map<int, AudioPlayer*> audioPlayers;

    AudioPlayer* get(int id) {
        if (this->audioPlayers.find(id) == this->audioPlayers.end()) {
            this->audioPlayers[id] = new AudioPlayer();
        }
        return this->audioPlayers[id];
    }
};


AudioPlayers* audioPlayers = new AudioPlayers();


int main(int argc, const char **argv) {
    try {
        AudioDevices::getDefault();
        AudioPlayer* audioPlayer = audioPlayers->get(0);
        audioPlayer->load(argv[1]);
        audioPlayer->play();
        std::cin.get();
    }
    catch (std::string exception) {
        std::cout << exception << std::endl;
    }
    return 0;
}