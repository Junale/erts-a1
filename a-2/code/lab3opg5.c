#include "xparameters.h"
#include "xgpio.h"
#include "xscutimer.h"
#include "xil_printf.h"
#include "xstatus.h"
#include "xtime_l.h"
#include "led_ip.h"
#include "matrix_ip.h"

#define ONE_SECOND 325000000
#define MSIZE 4

typedef union {
	unsigned char comp[MSIZE];
	unsigned int vect;
} vectorType;

typedef vectorType VectorArray[MSIZE];

VectorArray pInst;
VectorArray aInst;
VectorArray bInst;

void setInputMatrices(VectorArray A, VectorArray B)
{
	A[0].comp[0] = 1;
	A[0].comp[1] = 2;
	A[0].comp[2] = 3;
	A[0].comp[3] = 4;

	A[1].comp[0] = 5;
	A[1].comp[1] = 6;
	A[1].comp[2] = 7;
	A[1].comp[3] = 8;

	A[2].comp[0] = 9;
	A[2].comp[1] = 10;
	A[2].comp[2] = 11;
	A[2].comp[3] = 12;

	A[3].comp[0] = 13;
	A[3].comp[1] = 14;
	A[3].comp[2] = 15;
	A[3].comp[3] = 16;

	B[0].comp[0] = 1;
	B[0].comp[1] = 1;
	B[0].comp[2] = 1;
	B[0].comp[3] = 1;

	B[1].comp[0] = 2;
	B[1].comp[1] = 2;
	B[1].comp[2] = 2;
	B[1].comp[3] = 2;

	B[2].comp[0] = 3;
	B[2].comp[1] = 3;
	B[2].comp[2] = 3;
	B[2].comp[3] = 3;

	B[3].comp[0] = 4;
	B[3].comp[1] = 4;
	B[3].comp[2] = 4;
	B[3].comp[3] = 4;
}

void displayMatrix(VectorArray input)
{
	int row, col;

	for (row = 0; row < MSIZE; row++) {
		for (col = 0; col < MSIZE; col++) {
			xil_printf("%d\t", input[row].comp[col]);
		}
		xil_printf("\r\n");
	}
}

void multiMatrixSoft(VectorArray A, VectorArray B, VectorArray P)
{
	int Arow, Brow, col;
	unsigned int sum;

	for (Arow = 0; Arow < MSIZE; Arow++) {
		for (Brow = 0; Brow < MSIZE; Brow++) {
			sum = 0;

			for (col = 0; col < MSIZE; col++)
				sum += A[Arow].comp[col] * B[Brow].comp[col];

			P[Arow].comp[Brow] = sum;
		}
	}
}

void multiMatrixHard(VectorArray A, VectorArray B, VectorArray P)
{
	int row, col;

	for (row = 0; row < MSIZE; row++) {
		for (col = 0; col < MSIZE; col++) {

			MATRIX_IP_mWriteReg(XPAR_MATRIX_IP_0_S00_AXI_BASEADDR,
					MATRIX_IP_S00_AXI_SLV_REG0_OFFSET,
					A[row].vect);

			MATRIX_IP_mWriteReg(XPAR_MATRIX_IP_0_S00_AXI_BASEADDR,
					MATRIX_IP_S00_AXI_SLV_REG1_OFFSET,
					B[col].vect);

			P[row].comp[col] = MATRIX_IP_mReadReg(
					XPAR_MATRIX_IP_0_S00_AXI_BASEADDR,
					MATRIX_IP_S00_AXI_SLV_REG2_OFFSET);
		}
	}
}

int main(void)
{
	XGpio switches;

	XScuTimer Timer;
	XScuTimer_Config *ConfigPtr;
	XScuTimer *TimerInstancePtr = &Timer;

	int Status;
	int running = 1;
	int switch_value;
	int counter = 0;

	char value;

	xil_printf("-- Start of the Program --\r\n");
	xil_printf("Enter choice: 1 (SW->LEDs), 2 (Timer->LEDs), 3 (Matrix), 4 (exit)\r\n");

	/* Initialize switches */
	Status = XGpio_Initialize(&switches, XPAR_SWITCHES_DEVICE_ID);

	if (Status != XST_SUCCESS) {
		xil_printf("Switch initialization failed\r\n");
		return XST_FAILURE;
	}

	XGpio_SetDataDirection(&switches, 1, 0xffffffff);

	/* Initialize timer */
	ConfigPtr = XScuTimer_LookupConfig(XPAR_PS7_SCUTIMER_0_DEVICE_ID);

	if (ConfigPtr == NULL) {
		xil_printf("Timer configuration failed\r\n");
		return XST_FAILURE;
	}

	Status = XScuTimer_CfgInitialize(TimerInstancePtr, ConfigPtr,
			ConfigPtr->BaseAddr);

	if (Status != XST_SUCCESS) {
		xil_printf("Timer initialization failed\r\n");
		return XST_FAILURE;
	}

	XScuTimer_LoadTimer(TimerInstancePtr, ONE_SECOND);
	XScuTimer_EnableAutoReload(TimerInstancePtr);
	XScuTimer_Start(TimerInstancePtr);

	while (running) {
		xil_printf("CMD :> ");

		value = inbyte();

		switch (value) {

		case '1': {
			switch_value = XGpio_DiscreteRead(&switches, 1);
			switch_value &= 0x0F;

			LED_IP_mWriteReg(
					XPAR_LED_IP_0_S_AXI_BASEADDR,
					LED_IP_S_AXI_SLV_REG0_OFFSET,
					switch_value);

			xil_printf("Switch value: %x\r\n", switch_value);
			break;
		}

		case '2': {
			xil_printf("Starting binary counter...\r\n");

			counter = 0;

			while (1) {
				if (XScuTimer_IsExpired(TimerInstancePtr)) {
					XScuTimer_ClearInterruptStatus(TimerInstancePtr);

					LED_IP_mWriteReg(
							XPAR_LED_IP_0_S_AXI_BASEADDR,
							LED_IP_S_AXI_SLV_REG0_OFFSET,
							counter);

					xil_printf("Counter: %x\r\n", counter);

					counter++;

					if (counter > 15)
						counter = 0;
				}
			}

			break;
		}

		case '3': {
			XTime startTime;
			XTime endTime;
			XTime softTime;
			XTime hardTime;

			setInputMatrices(aInst, bInst);

			xil_printf("\r\nMatrix A:\r\n");
			displayMatrix(aInst);

			xil_printf("\r\nMatrix B:\r\n");
			displayMatrix(bInst);

			/* Software */
			XTime_GetTime(&startTime);

			multiMatrixSoft(aInst, bInst, pInst);

			XTime_GetTime(&endTime);

			softTime = endTime - startTime;

			xil_printf("\r\nSoftware result:\r\n");
			displayMatrix(pInst);

			xil_printf("Software time: %d clock ticks\r\n",
					(int)softTime);

			/* Hardware */
			XTime_GetTime(&startTime);

			multiMatrixHard(aInst, bInst, pInst);

			XTime_GetTime(&endTime);

			hardTime = endTime - startTime;

			xil_printf("\r\nHardware result:\r\n");
			displayMatrix(pInst);

			xil_printf("Hardware time: %d clock ticks\r\n",
					(int)hardTime);

			break;
		}

		case '4': {
			running = 0;
			break;
		}

		default: {
			xil_printf("Unknown command\r\n");
			break;
		}
		}
	}

	xil_printf("-- End of Program --\r\n");

	return 0;
}
