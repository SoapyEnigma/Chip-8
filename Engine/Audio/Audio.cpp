#include "Audio.h"

#include <miniaudio.h>

struct waveform { ma_waveform ma_waveform; };

Audio::~Audio()
{
    Shutdown();
}

void Audio::Init(i32 sampleRate)
{
    _sampleRate = sampleRate;
    EnsureInitialized();
}

void Audio::EnsureInitialized()
{
    if (_initialized)
        return;

    _engine = std::make_unique<ma_engine>();

    ma_engine_config ecfg = ma_engine_config_init();
    ecfg.sampleRate = _sampleRate;

    if (ma_engine_init(&ecfg, _engine.get()) != MA_SUCCESS)
    {
        _engine.reset();
        return;
    }

    _wave = std::make_unique<waveform>();
    ma_waveform_config wcfg = ma_waveform_config_init(ma_format_f32, 1, _sampleRate, ma_waveform_type_square, 0.2f, _freq);

    if (ma_waveform_init(&wcfg, &_wave->ma_waveform) != MA_SUCCESS)
    {
        ma_engine_uninit(_engine.get());
        _engine.reset();
        _wave.reset();
        return;
    }

    _sound = std::make_unique<ma_sound>();
    if (ma_sound_init_from_data_source(_engine.get(), &_wave->ma_waveform, 0, nullptr, _sound.get()) != MA_SUCCESS)
    {
        ma_waveform_uninit(&_wave->ma_waveform);
        ma_engine_uninit(_engine.get());
        _sound.reset();
        _wave.reset();
        _engine.reset();
        return;
    }

    ma_sound_set_looping(_sound.get(), true);
    _playing = false;
    _initialized = true;
}

void Audio::Shutdown()
{
    if (!_initialized)
        return;

    StopTone();

    if (_sound)
    {
        ma_sound_uninit(_sound.get());
        _sound.reset();
    }

    if (_wave)
    {
        ma_waveform_uninit(&_wave->ma_waveform);
        _wave.reset();
    }

    if (_engine)
    {
        ma_engine_uninit(_engine.get());
        _engine.reset();
    }

    _initialized = false;
}

void Audio::StartTone(f32 freqHz)
{
    EnsureInitialized();
    if (!_initialized)
        return;

    _freq = freqHz;
    ma_waveform_set_frequency(&_wave->ma_waveform, _freq);

    if (!_playing)
    {
        if (ma_sound_start(_sound.get()) == MA_SUCCESS)
            _playing = true;
    }
}

void Audio::StopTone()
{
    if (!_initialized || !_playing)
        return;

    ma_sound_stop(_sound.get());
    _playing = false;
}