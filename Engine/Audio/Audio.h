#pragma once

#include "Util/Types.h"

struct ma_engine;
struct ma_sound;
struct waveform;

class Audio
{
public:
    Audio() { Init(); }
    ~Audio() { Shutdown(); }

    void Init(i32 sampleRate = 48000);
    void Shutdown();
    void StartTone(f32 freqHz = 440.0f);
    void StopTone();
    bool IsPlaying() const { return _playing; }

private:
    ma_engine* _engine = nullptr;
    waveform* _wave = nullptr;
    ma_sound* _sound = nullptr;

    f32 _freq = 440.0f;
    i32 _sampleRate = 48000;
    bool _playing = false;
};
