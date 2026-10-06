#include "neuralNet.h"

void neuralNet(
    char input[INPUT_LEN],
    short weightsI[WEIGHTS_I_LEN],
    short weightsO[WEIGHTS_O_LEN],
    int output[OUTPUT_LEN],
    ap_int<SL_LEN> inSwitch,
    ap_int<SL_LEN> *outLeds)
{
#pragma HLS INTERFACE s_axilite port=input bundle=CTRL
#pragma HLS INTERFACE s_axilite port=weightsI bundle=CTRL
#pragma HLS INTERFACE s_axilite port=weightsO bundle=CTRL
#pragma HLS INTERFACE s_axilite port=output bundle=CTRL
#pragma HLS INTERFACE ap_none port=inSwitch
#pragma HLS INTERFACE ap_none port=outLeds
#pragma HLS INTERFACE s_axilite port=return bundle=CTRL

    int neurons[NEURONS_LEN];

    *outLeds = inSwitch;

    hidden_neurons:
    for (int i = 0; i < NEURONS_LEN; ++i) {
        long long sum = 0;
        const int offset = NEURONS_LEN * i;

        hidden_inputs:
        for (int j = 0; j < INPUT_LEN; ++j) {
#pragma HLS PIPELINE II=1
            sum += static_cast<int>(input[j]) * weightsI[offset + j];
        }

        int value = static_cast<int>(sum >> BITS);
        neurons[i] = value < 0 ? 0 : value;
    }

    output_neurons:
    for (int i = 0; i < OUTPUT_LEN; ++i) {
        long long sum = 0;
        const int offset = OUTPUT_LEN * i;

        output_inputs:
        for (int j = 0; j < NEURONS_LEN; ++j) {
#pragma HLS PIPELINE II=1
            sum += static_cast<long long>(neurons[j]) * weightsO[offset + j];
        }

        output[i] = static_cast<int>(sum >> BITS);
    }
}
