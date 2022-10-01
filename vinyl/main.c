#include "usermodfx.h"

#define CRACKLE_MAX_THRESHOLD (UINT32_MAX / 300)

// Parameters
static float crackle_threshold;
static float crackle_intensity;

// State
static uint8_t is_crackling_left;
static uint32_t crackle_timer_left;
static uint8_t is_crackling_right;
static uint32_t crackle_timer_right;

void set_crackle(uint8_t *is_crackling, uint32_t *crackle_timer) {
    if (!(*is_crackling))
    {
        (*is_crackling) = _fx_rand() < crackle_threshold;

        if (*is_crackling)
        {
            (*crackle_timer) = _fx_rand();
        }
    }
}

float create_crackle(float sample) {
    return linintf(
        crackle_intensity,
        sample,
        _fx_white()
    );
}

void MODFX_INIT(uint32_t platform, uint32_t api)
{
    crackle_threshold = 0;
    crackle_intensity = 0.f;
    is_crackling_left = 0;
    crackle_timer_left = 0;
    is_crackling_right = 0;
    crackle_timer_right = 0;
}

void MODFX_PROCESS(
    const float *main_xn,
    float *main_yn,
    const float *sub_xn,
    float *sub_yn,
    uint32_t frames)
{
    set_crackle(&is_crackling_left, &crackle_timer_left);
    set_crackle(&is_crackling_right, &crackle_timer_right);

    for (uint32_t i = 0; i < frames; i++)
    {
        float left = main_xn[i * 2];
        float right = main_xn[i * 2 + 1];

        if (is_crackling_left) {
            left = create_crackle(left);
        }

        if (is_crackling_right) {
            right = create_crackle(right);
        }

        is_crackling_left = crackle_timer_left == 0;
        is_crackling_right = crackle_timer_right == 0;

        main_yn[i * 2] = left;
        main_yn[i * 2 + 1] = right;
    }
    crackle_timer_left--;
    crackle_timer_right--;
}


void MODFX_PARAM(uint8_t index, int32_t value)
{
    const float valf = q31_to_f32(value);

    switch (index)
    {
        case k_user_modfx_param_time:
            crackle_threshold = valf * CRACKLE_MAX_THRESHOLD;
            break;
        case k_user_modfx_param_depth:
            crackle_intensity = valf;
            break;
        default:
            break;
    }
}
