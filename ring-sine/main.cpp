#include "usermodfx.h"
#include "osc_api.h"

#include <simplelfo.hpp>

#define MAX_FREQUENCY 1500
#define MIN_FREQUENCY 250

#define SINE(f) fx_sinf(si_fabsf(f)) * (f < 0 ? -1 : 1)

static float interpolation;
static dsp::SimpleLFO LFO;

static float step;

void MODFX_INIT(uint32_t platform, uint32_t api)
{
    interpolation = 0;
    LFO.reset();
    LFO.setF0(MIN_FREQUENCY, k_samplerate_recipf);
}

void MODFX_PROCESS(
    const float *main_xn,
    float *main_yn,
    const float *sub_xn,
    float *sub_yn,
    uint32_t frames)
{
    for (uint32_t i = 0; i < frames; i++)
    {
        float left = main_xn[i * 2];
        float mod_wave = linintf(
            interpolation,
            1,
            LFO.sine_bi()
        );
        left = left * mod_wave;

        main_yn[i * 2] = left;
        main_yn[i * 2 + 1] = left;
        LFO.cycle();
    }
}

void MODFX_PARAM(uint8_t index, int32_t value)
{
    const float valf = q31_to_f32(value);

    switch (index)
    {
        case k_user_modfx_param_time:
            LFO.setF0(
                MIN_FREQUENCY + valf * (MAX_FREQUENCY - MIN_FREQUENCY),
                k_samplerate_recipf
            );
            break;
        case k_user_modfx_param_depth:
            interpolation = valf;
            break;
        default:
            break;
    }
}
