#include "Audio.h"

void Audio::Init(i32 sampleRate)
{
    _sampleRate = sampleRate;

    ma_engine_config ecfg = ma_engine_config_init();
    ecfg.sampleRate = _sampleRate;

    ma_engine_init(&ecfg, &_engine);

    ma_waveform_config wcfg = ma_waveform_config_init(ma_format_f32, 1, _sampleRate, ma_waveform_type_square, 0.2f, _freq);
    ma_waveform_init(&wcfg, &_wave);

    ma_sound_init_from_data_source(&_engine, &_wave, 0, nullptr, &_sound);

    ma_sound_set_looping(&_sound, true);
    _playing = false;
}

void Audio::Shutdown()
{
    StopTone();

    ma_sound_uninit(&_sound);
    ma_waveform_uninit(&_wave);
    ma_engine_uninit(&_engine);
}

void Audio::StartTone(f32 freqHz)
{
    _freq = freqHz;
    ma_waveform_set_frequency(&_wave, _freq);

    if (!_playing)
    { 
        ma_sound_start(&_sound);
        _playing = true;
    }
}

void Audio::StopTone()
{
    if (_playing)
    {
        ma_sound_stop(&_sound);
        _playing = false;
    }
}