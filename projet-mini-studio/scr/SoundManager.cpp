#include "SoundManager.hpp"
#include <iostream>

SoundManager::SoundManager() {}

SoundManager::~SoundManager() {}

bool SoundManager::loadSound(const string& name, const string& filename) {
    SoundBuffer buffer;
    if (!buffer.loadFromFile(filename)) {
        cerr << "Error loading sound file: " << filename << endl;
        return false;
    }
    soundBuffers[name] = buffer;
    sounds[name].setBuffer(soundBuffers[name]);
    return true;
}

void SoundManager::playSound(const string& name) {
    if (sounds.find(name) != sounds.end()) {
        sounds[name].play();
    }
    else {
        cerr << "Sound not found: " << name << endl;
    }
}

void SoundManager::stopSound(const string& name) {
    if (sounds.find(name) != sounds.end()) {
        sounds[name].stop();
    }
    else {
        cerr << "Sound not found: " << name << endl;
    }
}

void SoundManager::setVolume(const string& name, float volume) {
    if (sounds.find(name) != sounds.end()) {
        sounds[name].setVolume(volume);
    }
    else {
        cerr << "Sound not found: " << name << endl;
    }
}

void SoundManager::setLoop(const string& name, bool loop) {
    if (sounds.find(name) != sounds.end()) {
        sounds[name].setLoop(loop);
    }
    else {
        cerr << "Sound not found: " << name << endl;
    }
}