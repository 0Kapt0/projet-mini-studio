#ifndef SOUNDMANAGER_HPP
#define SOUNDMANAGER_HPP

#include <SFML/Audio.hpp>
#include <string>
#include <map>

using namespace std;
using namespace sf;

class SoundManager {
public:
    SoundManager();
    ~SoundManager();

    bool loadSound(const string& name, const string& filename);
    void playSound(const string& name);
    void stopSound(const string& name);
    void setVolume(const string& name, float volume);
    void setLoop(const string& name, bool loop);
    bool addSoundEffect(const string& name, const string& filename);

private:
    map<string, SoundBuffer> soundBuffers;
    map<string, Sound> sounds;
};

#endif // SOUNDMANAGER_HPP