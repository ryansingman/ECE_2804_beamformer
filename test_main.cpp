#include "beamformer.h"

#include <cstdio>
#include <iostream>

#define SIGNAL_FREQ 1009
#define SIGNAL_BIN 157           // corresponds to 1000 Hz signal for sampling rate 9600
#define SIGNAL_AMP 10
#define NUM_SIGNALS 1
#define SIGNAL_ANGLE 90

void generate_input_signal(elem_arr *elem_data) {

    float t_delay = 0;
    float phase_shift = 0;

    for (int ii = 0; ii < NUM_SIGNALS; ++ii) {
        for (int jj = 0; jj < NUM_PHONES; ++jj) {
            t_delay = (jj * MIC_SEP) * sin(SIGNAL_ANGLE * (M_PI/180.)) / (float)SPEED_SOUND;
            phase_shift = 2 * M_PI * SIGNAL_FREQ * t_delay;
            (*elem_data)[jj][SIGNAL_BIN] += std::polar((float)SIGNAL_AMP, phase_shift);
        }
    }
}

void display_power(power_arr *power) {

    float angle = 0;

    printf("Displayed as Angle1 / Power1 || Angle2 / Power2 || ...\n");
    printf("Beam powers for signal angle: %f\n", (float)SIGNAL_ANGLE);
    for (int ii = 0; ii < (NUM_BEAMS-1); ++ii) {
        angle = -90 + 180 * ((float)ii / (float)(NUM_BEAMS - 1));
        printf("%0.2f / %0.2f || ", angle, (*power)[ii]);
    }
    printf("%0.2f / %0.2f\n", 90.0, (*power)[NUM_BEAMS -1]);

}

int main(void) {

    // generate input signal FFT
    elem_arr elem_data = { {0} };
    generate_input_signal(&elem_data);

    // generate weights
    time_del_arr t_delays = { {0} };
    freq_arr freqs        =   {0};
    weight_arr weights    = { {0} };
    
    find_time_delays(&t_delays);
    find_freqs(&freqs);
    generate_weights(&weights, &freqs, &t_delays);

    // beamform data
    bf_arr bf_data = { {0} };
    beamform_data(&bf_data, &elem_data, &weights);

    // find beam powers
    power_arr power = {0};
    find_beam_powers(&power, &bf_data);

    // display beam powers 
    display_power(&power);

    return 0;
}
