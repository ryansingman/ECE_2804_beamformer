#pragma once

#include <cmath>
#include <cstring>
#include <complex>

#define NUM_BEAMS 33
#define NUM_PHONES 2         // number of microphones used
#define NUM_FFT_POINTS 256

#define SIG_FREQ 1009       // signal frequency (Hz)
#define MIC_SEP 0.01        // distance between mics in m
#define SPEED_SOUND 343     // speed of sound in m/s
#define SAMP_RATE 9600      // sampling rate in Hz

typedef float time_del_arr[NUM_PHONES][NUM_BEAMS];
typedef float freq_arr[NUM_FFT_POINTS];
typedef std::complex<float> weight_arr[NUM_PHONES][NUM_BEAMS];
typedef std::complex<float> elem_arr[NUM_PHONES][NUM_FFT_POINTS];
typedef std::complex<float> bf_arr[NUM_BEAMS][NUM_FFT_POINTS];
typedef float power_arr[NUM_BEAMS];

void find_time_delays(time_del_arr *delays);
void find_freqs(freq_arr *freqs);
void generate_weights(weight_arr *weights, freq_arr *freqs, time_del_arr *t_delays);
void beamform_data(bf_arr *bf_data, elem_arr *elem_data, weight_arr *weights);
void find_beam_powers(power_arr *power, bf_arr *bf_data);
