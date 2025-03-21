#ifndef SOUNDMANAGER_HPP
#define SOUNDMANAGER_HPP

#include <SFML/Audio.hpp>
#include <string>
#include <map>

using namespace std;
using namespace sf;

class SoundManager {
public:
    static SoundManager& getInstance() {
        static SoundManager instance;
        return instance;
    }

    bool loadSound(const string& name, const string& filename);
    void playSound(const string& name);
    void stopSound(const string& name);
    void setVolume(const string& name, float volume);
    void setLoop(const string& name, bool loop);
    static SoundManager& getInstance() {
        static SoundManager instance;
        return instance;
    }
private:
    SoundManager() {}
    ~SoundManager() {}

    SoundManager(const SoundManager&) = delete;
    SoundManager& operator=(const SoundManager&) = delete;

    map<string, SoundBuffer> soundBuffers;
    map<string, Sound> sounds;
    
};

#endif