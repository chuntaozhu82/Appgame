#ifndef AUDIO_H
#define AUDIO_H

#include <string>
#include <memory>
#include <vector>
#include <unordered_map>
#include <OpenAL/al.h>
#include <OpenAL/alc.h>

namespace Appgame {

// 音频类型
enum class AudioType {
    SOUND,       // 音效
    MUSIC        // 音乐
};

// 音频状态
enum class AudioStatus {
    STOPPED,     // 停止
    PLAYING,     // 播放中
    PAUSED       // 暂停
};

// 音频源抽象类
class AudioSource {
public:
    virtual ~AudioSource() = default;

    // 播放
    virtual void play() = 0;

    // 暂停
    virtual void pause() = 0;

    // 停止
    virtual void stop() = 0;

    // 设置音量
    virtual void setVolume(float volume) = 0;

    // 获取音量
    virtual float getVolume() const = 0;

    // 设置循环
    virtual void setLoop(bool loop) = 0;

    // 获取循环状态
    virtual bool isLoop() const = 0;

    // 设置播放位置（秒）
    virtual void setPosition(float position) = 0;

    // 获取播放位置（秒）
    virtual float getPosition() const = 0;

    // 获取音频长度（秒）
    virtual float getDuration() const = 0;

    // 获取音频状态
    virtual AudioStatus getStatus() const = 0;

    // 检查是否正在播放
    virtual bool isPlaying() const = 0;

    // 检查是否已暂停
    virtual bool isPaused() const = 0;

    // 检查是否已停止
    virtual bool isStopped() const = 0;
};

// 音效类
class Sound : public AudioSource {
public:
    ~Sound();

    // 播放
    void play() override;

    // 暂停
    void pause() override;

    // 停止
    void stop() override;

    // 设置音量
    void setVolume(float volume) override;

    // 获取音量
    float getVolume() const override;

    // 设置循环
    void setLoop(bool loop) override;

    // 获取循环状态
    bool isLoop() const override;

    // 设置播放位置（秒）
    void setPosition(float position) override;

    // 获取播放位置（秒）
    float getPosition() const override;

    // 获取音频长度（秒）
    float getDuration() const override;

    // 获取音频状态
    AudioStatus getStatus() const override;

    // 检查是否正在播放
    bool isPlaying() const override;

    // 检查是否已暂停
    bool isPaused() const override;

    // 检查是否已停止
    bool isStopped() const override;

private:
    friend class AudioManager;

    Sound(ALuint buffer, ALuint source);

    ALuint m_buffer;
    ALuint m_source;
    float m_volume;
    bool m_loop;
    AudioStatus m_status;
};

// 音乐类
class Music : public AudioSource {
public:
    ~Music();

    // 播放
    void play() override;

    // 暂停
    void pause() override;

    // 停止
    void stop() override;

    // 设置音量
    void setVolume(float volume) override;

    // 获取音量
    float getVolume() const override;

    // 设置循环
    void setLoop(bool loop) override;

    // 获取循环状态
    bool isLoop() const override;

    // 设置播放位置（秒）
    void setPosition(float position) override;

    // 获取播放位置（秒）
    float getPosition() const override;

    // 获取音频长度（秒）
    float getDuration() const override;

    // 获取音频状态
    AudioStatus getStatus() const override;

    // 检查是否正在播放
    bool isPlaying() const override;

    // 检查是否已暂停
    bool isPaused() const override;

    // 检查是否已停止
    bool isStopped() const override;

private:
    friend class AudioManager;

    Music(ALuint buffer, ALuint source);

    ALuint m_buffer;
    ALuint m_source;
    float m_volume;
    bool m_loop;
    AudioStatus m_status;
};

// 音频管理器类
class AudioManager {
public:
    static AudioManager& getInstance();

    // 初始化音频系统
    bool init();

    // 清理音频系统
    void cleanup();

    // 加载音效
    std::unique_ptr<Sound> loadSound(const std::string& path);

    // 加载音乐
    std::unique_ptr<Music> loadMusic(const std::string& path);

    // 播放音效（快捷方法）
    void playSound(const std::string& path, float volume = 1.0f, bool loop = false);

    // 播放音乐（快捷方法）
    void playMusic(const std::string& path, float volume = 1.0f, bool loop = true);

    // 设置全局音量
    void setGlobalVolume(float volume);

    // 获取全局音量
    float getGlobalVolume() const;

    // 暂停所有音频
    void pauseAll();

    // 恢复所有音频
    void resumeAll();

    // 停止所有音频
    void stopAll();

    // 获取当前播放的音频数量
    size_t getPlayingCount() const;

private:
    AudioManager();
    ~AudioManager();

    // 内部方法
    bool loadAudioFile(const std::string& path, std::vector<unsigned char>& buffer, ALsizei& frequency, ALenum& format);
    ALuint createBuffer(const std::vector<unsigned char>& data, ALsizei frequency, ALenum format);
    ALuint createSource();

    // OpenAL设备和上下文
    ALCdevice* m_device;
    ALCcontext* m_context;

    // 全局音量
    float m_globalVolume;

    // 音频源管理
    std::vector<ALuint> m_sources;

    // 播放中的音频计数
    size_t m_playingCount;
};

} // namespace Appgame

#endif // AUDIO_H
