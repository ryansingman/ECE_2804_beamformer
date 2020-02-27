#include "beamformer.h"

#include <iostream>

void find_time_delays(time_del_arr *delays) {

    float angle, angleRad;

    // iterate thru phones
    for (int ii = 0; ii < NUM_PHONES; ++ii) {
        // iterate thru beams
        for (int jj = 0; jj < NUM_BEAMS; ++jj) {
            angle = -90 + 180 * ((float)jj / (float)(NUM_BEAMS - 1));
            angleRad = M_PI * (float)angle / (float)180;

            (*delays)[ii][jj] = (ii * MIC_SEP) * sin(angleRad) / (float)SPEED_SOUND;
        }
    }
}

void find_freqs(freq_arr *freqs) {

    // find frequency step
    float freq_step = ((SAMP_RATE) / (NUM_FFT_POINTS - 1));

    // find each frequency
    for (int ii = 0; ii < NUM_FFT_POINTS; ++ii) {
        (*freqs)[ii] = (float)ii * freq_step - (SAMP_RATE / 2);
    }
}

void generate_weights(weight_arr *weights, freq_arr *freqs, time_del_arr *t_delays) {

    std::complex<float> exponent;

    for (int ii = 0; ii < NUM_PHONES; ++ii) {
        for (int jj = 0; jj < NUM_BEAMS; ++jj) {
            exponent = std::complex<float>(0.0, 2 * M_PI * (*freqs)[jj] * (*t_delays)[ii][jj]);
            (*weights)[ii][jj] = std::exp(exponent);
        }
    }
}

void beamform_data(bf_arr *bf_data, elem_arr *elem_data, weight_arr *weights) {
   
    // iterate thru beams
    for (int ii = 0; ii < NUM_BEAMS; ++ii) {
        // iterate thru phones
        for (int jj = 0; jj < NUM_PHONES; ++jj) {
            // iterate thru fft points
            for (int kk = 0; kk < NUM_FFT_POINTS; ++kk) { 
                (*bf_data)[ii][kk] += (*weights)[jj][ii] * (*elem_data)[jj][kk];
           }
        }
    }
}

void find_beam_powers(power_arr *power, bf_arr *bf_data) {

    // iterate thru beams
    for (int ii = 0; ii < NUM_BEAMS; ++ii) {
        // iterate thru fft points
        for (int jj = 0; jj < NUM_FFT_POINTS; ++jj) {
            (*power)[ii] += std::pow(std::abs((*bf_data)[ii][jj]), 2);
        }
    }
}
