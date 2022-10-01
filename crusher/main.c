#include "usermodfx.h"

#define MAX_SKIP 64
#define RESOLUTION_LOG_SCALING_FACTOR 8
#define MAX_RESOLUTION_REDUCTION 13

// Parameters
static float skip_rate;
static float resolution;

// State
static float active_sample_left;
static float active_sample_right;
static int32_t sample_count;

float crush_float(float input, float reduction)
{
    const float num_bits = 16 - reduction;
    const float max_value = fastpow2f(num_bits);
    const uint32_t int_representation = (uint32_t) (si_fabsf(input) * max_value);
    float low_res_float = ((float) int_representation) / max_value;

    if (input < 0) {
        return low_res_float * -1;
    }
    return low_res_float;
}

void MODFX_INIT(uint32_t platform, uint32_t api)
{
    skip_rate = 0;
    active_sample_left = 0.f;
    active_sample_right = 0.f;
    sample_count = 0;
    resolution = 0;
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
        float in_left = main_xn[i * 2];
        float in_right = main_xn[i * 2 + 1];
        // skip_rate is between 0 - 1
        const uint32_t skip = skip_rate * MAX_SKIP;

        // If the sample_count is zero then store the sample.
        // We will update the sample once we have skipped enough samples.
        if (sample_count == 0)
        {
            // const float resolution_reduction = resolution * MAX_RESOLUTION_REDUCTION;
            const float resolution_reduction = clip0f(
                log10f(resolution) *
                RESOLUTION_LOG_SCALING_FACTOR +
                MAX_RESOLUTION_REDUCTION
            );

            active_sample_left = crush_float(in_left, resolution_reduction);
            active_sample_right = crush_float(in_right, resolution_reduction);
        }

        main_yn[i * 2] = active_sample_left;
        main_yn[i * 2 + 1] = active_sample_right;

        sample_count++;

        if (sample_count > skip)
        {
            sample_count = 0;
        }
    }
}

void MODFX_PARAM(uint8_t index, int32_t value)
{
    const float valf = q31_to_f32(value);

    switch (index)
    {
    case k_user_modfx_param_time:
        skip_rate = valf;
        break;
    case k_user_modfx_param_depth:
        resolution = valf;
        break;
    default:
        break;
    }
}
