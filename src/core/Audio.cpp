#include "core/Audio.h"
#include <fstream>
#include <vector>

namespace Appgame {

// Sound 类实现

Sound::Sound(ALuint buffer, ALuint source)
    : m_buffer(buffer), m_source(source), m_volume(1.0f), m_loop(false), m_status(AudioStatus::STOPPED) {
    alSourcei(m_source, AL_BUFFER, m_buffer);
    alSourcef(m_source, AL_GAIN, m_volume);
    alSourcei(m_source, AL_LOOPING, AL_FALSE);
}

Sound::~Sound() {
    // 注意：缓冲区和源的清理由AudioManager负责
}

void Sound::play() {
    alSourcePlay(m_source);
    m_status = AudioStatus::PLAYING;
}

void Sound::pause() {
    alSourcePause(m_source);
    m_status = AudioStatus::PAUSED;
}

void Sound::stop() {
    alSourceStop(m_source);
    alSourceRewind(m_source);
    m_status = AudioStatus::STOPPED;
}

void Sound::setVolume(float volume) {
    m_volume = volume;
    alSourcef(m_source, AL_GAIN, m_volume);
}

float Sound::getVolume() const {
    return m_volume;
}

void Sound::setLoop(bool loop) {
    m_loop = loop;
    alSourcei(m_source, AL_LOOPING, loop ? AL_TRUE : AL_FALSE);
}

bool Sound::isLoop() const {
    return m_loop;
}

void Sound::setPosition(float position) {
    alSourcef(m_source, AL_SEC_OFFSET, position);
}

float Sound::getPosition() const {
    ALfloat position;
    alGetSourcef(m_source, AL_SEC_OFFSET, &position);
    return position;
}

float Sound::getDuration() const {
    ALint size;
    ALint frequency;
    ALenum format;
    alGetBufferi(m_buffer, AL_SIZE, &size);
    alGetBufferi(m_buffer, AL_FREQUENCY, &frequency);
    alGetBufferi(m_buffer, AL_FORMAT, &format);

    // 计算每秒字节数
    int bytesPerSample = 0;
    if (format == AL_FORMAT_MONO8) bytesPerSample = 1;
    else if (format == AL_FORMAT_MONO16) bytesPerSample = 2;
    else if (format == AL_FORMAT_STEREO8) bytesPerSample = 2;
    else if (format == AL_FORMAT_STEREO16) bytesPerSample = 4;

    if (bytesPerSample > 0 && frequency > 0) {
        return static_cast<float>(size) / (bytesPerSample * frequency);
    }
    return 0.0f;
}

AudioStatus Sound::getStatus() const {
    ALint status;
    alGetSourcei(m_source, AL_SOURCE_STATE, &status);
    switch (status) {
    case AL_PLAYING:
        return AudioStatus::PLAYING;
    case AL_PAUSED:
        return AudioStatus::PAUSED;
    default:
        return AudioStatus::STOPPED;
    }
}

bool Sound::isPlaying() const {
    return getStatus() == AudioStatus::PLAYING;
}

bool Sound::isPaused() const {
    return getStatus() == AudioStatus::PAUSED;
}

bool Sound::isStopped() const {
    return getStatus() == AudioStatus::STOPPED;
}

// Music 类实现

Music::Music(ALuint buffer, ALuint source)
    : m_buffer(buffer), m_source(source), m_volume(1.0f), m_loop(true), m_status(AudioStatus::STOPPED) {
    alSourcei(m_source, AL_BUFFER, m_buffer);
    alSourcef(m_source, AL_GAIN, m_volume);
    alSourcei(m_source, AL_LOOPING, AL_TRUE);
}

Music::~Music() {
    // 注意：缓冲区和源的清理由AudioManager负责
}

void Music::play() {
    alSourcePlay(m_source);
    m_status = AudioStatus::PLAYING;
}

void Music::pause() {
    alSourcePause(m_source);
    m_status = AudioStatus::PAUSED;
}

void Music::stop() {
    alSourceStop(m_source);
    alSourceRewind(m_source);
    m_status = AudioStatus::STOPPED;
}

void Music::setVolume(float volume) {
    m_volume = volume;
    alSourcef(m_source, AL_GAIN, m_volume);
}

float Music::getVolume() const {
    return m_volume;
}

void Music::setLoop(bool loop) {
    m_loop = loop;
    alSourcei(m_source, AL_LOOPING, loop ? AL_TRUE : AL_FALSE);
}

bool Music::isLoop() const {
    return m_loop;
}

void Music::setPosition(float position) {
    alSourcef(m_source, AL_SEC_OFFSET, position);
}

float Music::getPosition() const {
    ALfloat position;
    alGetSourcef(m_source, AL_SEC_OFFSET, &position);
    return position;
}

float Music::getDuration() const {
    ALint size;
    ALint frequency;
    ALenum format;
    alGetBufferi(m_buffer, AL_SIZE, &size);
    alGetBufferi(m_buffer, AL_FREQUENCY, &frequency);
    alGetBufferi(m_buffer, AL_FORMAT, &format);

    // 计算每秒字节数
    int bytesPerSample = 0;
    if (format == AL_FORMAT_MONO8) bytesPerSample = 1;
    else if (format == AL_FORMAT_MONO16) bytesPerSample = 2;
    else if (format == AL_FORMAT_STEREO8) bytesPerSample = 2;
    else if (format == AL_FORMAT_STEREO16) bytesPerSample = 4;

    if (bytesPerSample > 0 && frequency > 0) {
        return static_cast<float>(size) / (bytesPerSample * frequency);
    }
    return 0.0f;
}

AudioStatus Music::getStatus() const {
    ALint status;
    alGetSourcei(m_source, AL_SOURCE_STATE, &status);
    switch (status) {
    case AL_PLAYING:
        return AudioStatus::PLAYING;
    case AL_PAUSED:
        return AudioStatus::PAUSED;
    default:
        return AudioStatus::STOPPED;
    }
}

bool Music::isPlaying() const {
    return getStatus() == AudioStatus::PLAYING;
}

bool Music::isPaused() const {
    return getStatus() == AudioStatus::PAUSED;
}

bool Music::isStopped() const {
    return getStatus() == AudioStatus::STOPPED;
}

// AudioManager 类实现

AudioManager::AudioManager()
    : m_device(nullptr), m_context(nullptr), m_globalVolume(1.0f), m_playingCount(0) {
}

AudioManager::~AudioManager() {
    cleanup();
}

AudioManager& AudioManager::getInstance() {
    static AudioManager instance;
    return instance;
}

bool AudioManager::init() {
    if (!m_device) {
        // 打开默认音频设备
        m_device = alcOpenDevice(nullptr);
        if (!m_device) {
            return false;
        }

        // 创建音频上下文
        m_context = alcCreateContext(m_device, nullptr);
        if (!m_context) {
            alcCloseDevice(m_device);
            m_device = nullptr;
            return false;
        }

        // 激活音频上下文
        alcMakeContextCurrent(m_context);
    }
    return true;
}

void AudioManager::cleanup() {
    if (m_context) {
        alcMakeContextCurrent(nullptr);
        alcDestroyContext(m_context);
        m_context = nullptr;
    }

    if (m_device) {
        alcCloseDevice(m_device);
        m_device = nullptr;
    }

    // 清理音频源
    for (ALuint source : m_sources) {
        alDeleteSources(1, &source);
    }
    m_sources.clear();

    m_playingCount = 0;
}

std::unique_ptr<Sound> AudioManager::loadSound(const std::string& path) {
    std::vector<unsigned char> buffer;
    ALsizei frequency;
    ALenum format;

    if (!loadAudioFile(path, buffer, frequency, format)) {
        return nullptr;
    }

    ALuint alBuffer = createBuffer(buffer, frequency, format);
    if (alBuffer == 0) {
        return nullptr;
    }

    ALuint source = createSource();
    if (source == 0) {
        alDeleteBuffers(1, &alBuffer);
        return nullptr;
    }

    return std::make_unique<Sound>(alBuffer, source);
}

std::unique_ptr<Music> AudioManager::loadMusic(const std::string& path) {
    std::vector<unsigned char> buffer;
    ALsizei frequency;
    ALenum format;

    if (!loadAudioFile(path, buffer, frequency, format)) {
        return nullptr;
    }

    ALuint alBuffer = createBuffer(buffer, frequency, format);
    if (alBuffer == 0) {
        return nullptr;
    }

    ALuint source = createSource();
    if (source == 0) {
        alDeleteBuffers(1, &alBuffer);
        return nullptr;
    }

    return std::make_unique<Music>(alBuffer, source);
}

void AudioManager::playSound(const std::string& path, float volume, bool loop) {
    auto sound = loadSound(path);
    if (sound) {
        sound->setVolume(volume * m_globalVolume);
        sound->setLoop(loop);
        sound->play();
        m_playingCount++;
    }
}

void AudioManager::playMusic(const std::string& path, float volume, bool loop) {
    auto music = loadMusic(path);
    if (music) {
        music->setVolume(volume * m_globalVolume);
        music->setLoop(loop);
        music->play();
        m_playingCount++;
    }
}

void AudioManager::setGlobalVolume(float volume) {
    m_globalVolume = volume;
    // 这里可以更新所有现有音频源的音量
}

float AudioManager::getGlobalVolume() const {
    return m_globalVolume;
}

void AudioManager::pauseAll() {
    // 暂停所有音频源
    for (ALuint source : m_sources) {
        ALint status;
        alGetSourcei(source, AL_SOURCE_STATE, &status);
        if (status == AL_PLAYING) {
            alSourcePause(source);
        }
    }
}

void AudioManager::resumeAll() {
    // 恢复所有音频源
    for (ALuint source : m_sources) {
        ALint status;
        alGetSourcei(source, AL_SOURCE_STATE, &status);
        if (status == AL_PAUSED) {
            alSourcePlay(source);
        }
    }
}

void AudioManager::stopAll() {
    // 停止所有音频源
    for (ALuint source : m_sources) {
        alSourceStop(source);
        alSourceRewind(source);
    }
    m_playingCount = 0;
}

size_t AudioManager::getPlayingCount() const {
    return m_playingCount;
}

bool AudioManager::loadAudioFile(const std::string& path, std::vector<unsigned char>& buffer, ALsizei& frequency, ALenum& format) {
    // 这里实现音频文件加载逻辑
    // 注意：实际项目中需要支持WAV、OGG等格式的音频文件
    // 这里仅作为示例，返回false
    return false;
}

ALuint AudioManager::createBuffer(const std::vector<unsigned char>& data, ALsizei frequency, ALenum format) {
    ALuint buffer;
    alGenBuffers(1, &buffer);
    alBufferData(buffer, format, data.data(), data.size(), frequency);
    return buffer;
}

ALuint AudioManager::createSource() {
    ALuint source;
    alGenSources(1, &source);
    alSourcef(source, AL_GAIN, m_globalVolume);
    alSourcei(source, AL_LOOPING, AL_FALSE);
    m_sources.push_back(source);
    return source;
}

} // namespace Appgame
