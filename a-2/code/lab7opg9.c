/******************************************************************************
 *
 * neuralNet.c
 *
 * Measures and compares:
 *
 *   1. Software neural network execution on ARM Cortex-A9
 *   2. Hardware IP:
 *        - input transfer time
 *        - weight transfer time
 *        - neural network computation time
 *        - output transfer time
 *        - total hardware execution time
 *
 * The measurements can be used to identify the bottleneck in the
 * hardware-accelerated implementation.
 *
 ******************************************************************************/

#include <stdio.h>
#include <stdlib.h>

#include "xil_printf.h"
#include "XScuTimer.h"
#include "xneuralnet.h"


/* ========================================================================= */
/* Configuration                                                             */
/* ========================================================================= */

#define ONE_SECOND 333000000

#define BITS            15

#define INPUT_LEN       196
#define NEURONS_LEN     32
#define OUTPUT_LEN      10

#define WEIGHTS_I_LEN   (INPUT_LEN * NEURONS_LEN)
#define WEIGHTS_O_LEN   (NEURONS_LEN * OUTPUT_LEN)


/* ========================================================================= */
/* Hardware IP instance                                                      */
/* ========================================================================= */

XNeuralnet neuralNetHLS;


/* ========================================================================= */
/* Timer                                                                      */
/* ========================================================================= */

XScuTimer Timer;

XScuTimer_Config *ConfigPtr;
XScuTimer *TimerInstancePtr = &Timer;


/*
 * Timer is a down-counter.
 * startTimer() stores the starting counter value.
 * stopTimer() returns the number of timer ticks elapsed.
 */

unsigned int startTimer(void)
{
    unsigned int start;

    start = XScuTimer_GetCounterValue(TimerInstancePtr);
    return start;
}


unsigned int stopTimer(unsigned int start)
{
    unsigned int stop;

    stop = XScuTimer_GetCounterValue(TimerInstancePtr);

    /*
     * Timer counts down.
     */
    return start - stop;
}


void initTimer(void)
{
    int Status;

    ConfigPtr = XScuTimer_LookupConfig(
        XPAR_PS7_SCUTIMER_0_DEVICE_ID
    );

    Status = XScuTimer_CfgInitialize(
        TimerInstancePtr,
        ConfigPtr,
        ConfigPtr->BaseAddr
    );

    if (Status != XST_SUCCESS) {
        print("Timer initialization failed\r\n");
        return;
    }

    XScuTimer_LoadTimer(TimerInstancePtr, ONE_SECOND);

    XScuTimer_EnableAutoReload(TimerInstancePtr);

    XScuTimer_Start(TimerInstancePtr);
}


/* ========================================================================= */
/* Neural network data                                                        */
/* ========================================================================= */

signed char inputs[INPUT_LEN];

short weightsIS[WEIGHTS_I_LEN];
float weightsI[WEIGHTS_I_LEN];

short weightsOS[WEIGHTS_O_LEN];
float weightsO[WEIGHTS_O_LEN];

int outputsS[OUTPUT_LEN];
float outputsF[OUTPUT_LEN];


/* ========================================================================= */
/* Software implementation                                                   */
/* ========================================================================= */

void neuralNetFloat(
    signed char input[INPUT_LEN],
    float weightsI[WEIGHTS_I_LEN],
    float weightsO[WEIGHTS_O_LEN],
    float output[OUTPUT_LEN]
)
{
    float neurons[NEURONS_LEN];

    float neuron;
    float result;

    int i;
    int j;

    int offsetI;
    int offsetO;


    /* --------------------------------------------------------------------- */
    /* Input layer                                                           */
    /* --------------------------------------------------------------------- */

    for (i = 0; i < NEURONS_LEN; i++) {

        neuron = 0;

        offsetI = NEURONS_LEN * i;

        for (j = 0; j < INPUT_LEN; j++) {

            neuron +=
                input[j] *
                weightsI[j + offsetI];
        }

        /* ReLU */
        if (neuron < 0)
            neuron = 0;

        neurons[i] = neuron;
    }


    /* --------------------------------------------------------------------- */
    /* Output layer                                                          */
    /* --------------------------------------------------------------------- */

    for (i = 0; i < OUTPUT_LEN; i++) {

        result = 0;

        offsetO = OUTPUT_LEN * i;

        for (j = 0; j < NEURONS_LEN; j++) {

            result +=
                neurons[j] *
                weightsO[j + offsetO];
        }

        output[i] = result;
    }
}


/* ========================================================================= */
/* Hardware implementation                                                   */
/* ========================================================================= */

/*
 * The hardware implementation is deliberately split into individual
 * measurements.
 *
 * This allows us to determine whether the bottleneck is:
 *
 *   - transferring inputs
 *   - transferring weights
 *   - executing the neural network
 *   - reading the output
 *
 */

void neuralNetHW(
    signed char input[INPUT_LEN],
    short wI[WEIGHTS_I_LEN],
    short wO[WEIGHTS_O_LEN],
    int output[OUTPUT_LEN]
)
{
    unsigned int start;

    unsigned int inputTransferTicks;
    unsigned int weightTransferTicks;
    unsigned int outputTransferTicks;
    unsigned int computeTicks;

    unsigned int totalTicks;


    /* Wait until IP is ready */

    while (XNeuralnet_IsReady(&neuralNetHLS) == 0);


    /* ===================================================================== */
    /* Transfer input data                                                   */
    /* ===================================================================== */

    start = XScuTimer_GetCounterValue(TimerInstancePtr);

    XNeuralnet_Write_input_r_Words(
        &neuralNetHLS,
        0,
        (int *)input,
        INPUT_LEN / 4
    );

    inputTransferTicks = stopTimer(start);


    /* ===================================================================== */
    /* Transfer input-layer weights                                          */
    /* ===================================================================== */

    start = XScuTimer_GetCounterValue(TimerInstancePtr);

    XNeuralnet_Write_weightsI_Words(
        &neuralNetHLS,
        0,
        (int *)wI,
        WEIGHTS_I_LEN / 2
    );

    weightTransferTicks = stopTimer(start);


    /* ===================================================================== */
    /* Transfer output-layer weights                                         */
    /* ===================================================================== */

    start = XScuTimer_GetCounterValue(TimerInstancePtr);

    XNeuralnet_Write_weightsO_Words(
        &neuralNetHLS,
        0,
        (int *)wO,
        WEIGHTS_O_LEN / 2
    );

    weightTransferTicks += stopTimer(start);


    /* ===================================================================== */
    /* Execute neural network hardware IP                                     */
    /* ===================================================================== */

    start = XScuTimer_GetCounterValue(TimerInstancePtr);

    XNeuralnet_Start(&neuralNetHLS);

    while (XNeuralnet_IsDone(&neuralNetHLS) == 0);

    computeTicks = stopTimer(start);


    /* ===================================================================== */
    /* Read output from hardware                                              */
    /* ===================================================================== */

    start = XScuTimer_GetCounterValue(TimerInstancePtr);

    XNeuralnet_Read_output_r_Words(
        &neuralNetHLS,
        0,
        (int *)output,
        OUTPUT_LEN
    );

    outputTransferTicks = stopTimer(start);


    /* ===================================================================== */
    /* Total hardware time                                                    */
    /* ===================================================================== */

    totalTicks =
        inputTransferTicks +
        weightTransferTicks +
        computeTicks +
        outputTransferTicks;


    /* ===================================================================== */
    /* Print measurements                                                     */
    /* ===================================================================== */

    printf("\r\n");
    printf("----- Hardware Neural Network Timing -----\r\n");

    printf("Input transfer       : %u ticks\r\n",
           inputTransferTicks);

    printf("Weight transfer      : %u ticks\r\n",
           weightTransferTicks);

    printf("HW computation       : %u ticks\r\n",
           computeTicks);

    printf("Output transfer      : %u ticks\r\n",
           outputTransferTicks);

    printf("HW total             : %u ticks\r\n",
           totalTicks);

    printf("------------------------------------------\r\n");
}


/* ========================================================================= */
/* Test neural network                                                       */
/* ========================================================================= */

void testNeuralNet(void)
{
    unsigned int start;
    unsigned int softwareTicks;

    int scale;
    int i;

    float diff;


    scale = (1 << BITS);


    /* ===================================================================== */
    /* Generate test input data                                              */
    /* ===================================================================== */

    for (i = 0; i < INPUT_LEN; i++) {

        inputs[i] = i;
    }


    /* ===================================================================== */
    /* Generate input-layer weights                                          */
    /* ===================================================================== */

    for (i = 0; i < WEIGHTS_I_LEN; i++) {

        weightsI[i] = 0.0001f * i;

        /*
         * Convert floating-point weights to fixed point.
         */
        weightsIS[i] =
            (short)(weightsI[i] * scale + 0.5f);
    }


    /* ===================================================================== */
    /* Generate output-layer weights                                         */
    /* ===================================================================== */

    for (i = 0; i < WEIGHTS_O_LEN; i++) {

        weightsO[i] = 0.001f * i;

        /*
         * Convert floating-point weights to fixed point.
         */
        weightsOS[i] =
            (short)(weightsO[i] * scale + 0.5f);
    }


    /* ===================================================================== */
    /* SOFTWARE IMPLEMENTATION                                               */
    /* ===================================================================== */

    printf("\r\n");
    printf("==========================================\r\n");
    printf("Software implementation\r\n");
    printf("==========================================\r\n");

    start = XScuTimer_GetCounterValue(TimerInstancePtr);

    neuralNetFloat(
        inputs,
        weightsI,
        weightsO,
        outputsF
    );

    softwareTicks = stopTimer(start);

    printf("CPU computation      : %u ticks\r\n",
           softwareTicks);


    /* ===================================================================== */
    /* HARDWARE IMPLEMENTATION                                               */
    /* ===================================================================== */

    printf("\r\n");
    printf("==========================================\r\n");
    printf("Hardware implementation\r\n");
    printf("==========================================\r\n");

    neuralNetHW(
        inputs,
        weightsIS,
        weightsOS,
        outputsS
    );


    /* ===================================================================== */
    /* Compare results                                                       */
    /* ===================================================================== */

    printf("\r\n");
    printf("==========================================\r\n");
    printf("Result comparison\r\n");
    printf("==========================================\r\n");

    printf("Fixed-point scale : %d\r\n\r\n", scale);

    diff = 0;

    for (i = 0; i < OUTPUT_LEN; i++) {

        printf(
            "Output[%d] = %d    Software = %0.2f\r\n",
            i,
            outputsS[i],
            outputsF[i]
        );

        diff += abs(
            outputsS[i] -
            (int)outputsF[i]
        );
    }


    if (diff < 10)
        printf("\r\nResult: CORRECT\r\n");
    else
        printf("\r\nResult: DIFFERENT\r\n");


    /* ===================================================================== */
    /* Speedup                                                               */
    /* ===================================================================== */

    /*
     * Note:
     *
     * To calculate hardware total time we need the individual HW
     * measurements again. For this reason the timing values can also
     * be returned from neuralNetHW if numerical speedup is required.
     *
     * The important comparison is:
     *
     *   CPU computation
     *
     * versus
     *
     *   HW computation
     *
     * and separately:
     *
     *   CPU computation
     *
     * versus
     *
     *   HW transfer + computation
     */
}


/* ========================================================================= */
/* Main                                                                     */
/* ========================================================================= */

int main(void)
{
    int depth;
    int bitwidth;


    print("==========================================\r\n");
    print("Neural Network Software/Hardware Test\r\n");
    print("==========================================\r\n");


    /* Initialize timer */

    initTimer();


    /* ===================================================================== */
    /* Initialize neural network HLS IP                                      */
    /* ===================================================================== */

    if (XNeuralnet_Initialize(
            &neuralNetHLS,
            XPAR_NEURALNET_0_DEVICE_ID
        ) != XST_SUCCESS)
    {
        print("Neural network initialization failed\r\n");
        return XST_FAILURE;
    }


    /* ===================================================================== */
    /* Display IP interface information                                      */
    /* ===================================================================== */

    depth =
        XNeuralnet_Get_input_r_Depth(
            &neuralNetHLS
        );

    bitwidth =
        XNeuralnet_Get_input_r_BitWidth(
            &neuralNetHLS
        );

    printf(
        "Input: depth = %d, bitwidth = %d\r\n",
        depth,
        bitwidth
    );


    depth =
        XNeuralnet_Get_weightsI_Depth(
            &neuralNetHLS
        );

    bitwidth =
        XNeuralnet_Get_weightsI_BitWidth(
            &neuralNetHLS
        );

    printf(
        "Input weights: depth = %d, bitwidth = %d\r\n",
        depth,
        bitwidth
    );


    depth =
        XNeuralnet_Get_weightsO_Depth(
            &neuralNetHLS
        );

    bitwidth =
        XNeuralnet_Get_weightsO_BitWidth(
            &neuralNetHLS
        );

    printf(
        "Output weights: depth = %d, bitwidth = %d\r\n",
        depth,
        bitwidth
    );


    /* ===================================================================== */
    /* Run test                                                              */
    /* ===================================================================== */

    testNeuralNet();


    return 0;
}
