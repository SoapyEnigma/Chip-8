#pragma once

#include "Types.h"

#include "miniaudio.h"

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
    ma_engine _engine;
    ma_waveform _wave;
    ma_sound _sound;

    f32 _freq = 440.0f;
    i32 _sampleRate = 48000;
    bool _playing = false;
};
