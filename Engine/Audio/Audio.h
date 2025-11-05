#pragma once

#include "Util/Types.h"

#include <memory>

struct ma_engine;
struct ma_sound;
struct waveform;

class Audio
{
public:
    Audio() = default;
    ~Audio();

    void Init(i32 sampleRate = 48000);
    void Shutdown();
    void StartTone(f32 freqHz = 440.0f);
    void StopTone();
    bool IsPlaying() const { return _playing; }

private:
    void EnsureInitialized();

    std::unique_ptr<ma_engine> _engine;
    std::unique_ptr<waveform> _wave;
    std::unique_ptr<ma_sound> _sound;

    f32 _freq = 440.0f;
    i32 _sampleRate = 48000;
    bool _playing = false;
    bool _initialized = false;
};
