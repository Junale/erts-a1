#include "xil_printf.h"

int main(void)
{
    xil_printf("\r\n");
    xil_printf("NeuralNet FPGA test\r\n");
    while (1)
    {
        /*
         * The NeuralNet is connected directly to the
         * switches and LEDs in the FPGA fabric.
         *
         * Nothing needs to be done by the processor.
         */
    }

    return 0;
}
