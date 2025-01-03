/*
 *  Implements the convolution of a signal with a filter impulse response
 */

#include "stm32f4xx.h"
#include <stdio.h>
#include "signals.h"
#include "uart.h"

extern float _5hz_signal[HZ_5_SIG_LEN];
extern float32_t input_signal_f32_1kHz_15kHz[KHZ1_15_SIG_LEN] ;
extern float32_t  impulse_response[IMP_RSP_LENGTH];


// Buffer to hold the result of the convolution operation
float32_t output_sig_arr[KHZ1_15_SIG_LEN + IMP_RSP_LENGTH - 1];


float32_t g_in_sig_sample;
float32_t g_imp_resp_sample;

static void serial_plot_all(void);
static void serial_plot_output_signal(void);
static void serial_plot_imp_resp(void);
static void swv_plot_imp_resp(void);
static void swv_plot_signal(void);
static void serial_plot_signal(void);
static void pseudo_dly(int dly);
static void fpu_enable(void);

// Convolution function prototype declaration
void convolution(float32_t * sig_src_arr,		// Here is the input signal
				 float32_t * sig_dest_arr,		// Here is the post-convolved output signal
				 float32_t * imp_resp_arr,		// Here is the filter impulse response
				 uint32_t sig_src_length,		// Length of the input signal
				 uint32_t imp_resp_length);		// Length of the filter impulse response


int main(void)
{
	// Enable FPU
	fpu_enable();

	// Initialize the UART
	uart2_tx_init();

	// Here, perform the convolution
	convolution((float32_t *) input_signal_f32_1kHz_15kHz,		// Here is the input signal
					 (float32_t *) output_sig_arr,		// Here is the post-convolved output signal
					 (float32_t *) impulse_response,		// Here is the filter impulse response
					 (uint32_t) KHZ1_15_SIG_LEN,		// Length of the input signal
					 (uint32_t) IMP_RSP_LENGTH);		// Length of the filter impulse response


	while(1)
	{
		serial_plot_all();
	}

} // End of main


// Here is a function that convolves a signal with a filter impulse response
void convolution(float32_t *sig_src_arr,		// Here is the input signal
				 float32_t *sig_dest_arr,		// Here is the post-convolved output signal
				 float32_t *imp_resp_arr,		// Here is the filter impulse response
				 uint32_t sig_src_length,		// Length of the input signal
				 uint32_t imp_resp_length)		// Length of the filter impulse response
{
	uint32_t i, j;

	// Here is the length of the post-convolved output signal
	 uint32_t conv_length = sig_src_length + imp_resp_length - 1;

	// Clear the output signal buffer - is this really needed?  Try doing this at initialization up top.
	for(i = 0; i < conv_length; i++)
	{
		sig_dest_arr[i] = 0.0;
	}

	// Convolution performed here
	for(i = 0; i < sig_src_length; i++)
	{
		for(j = 0; j < imp_resp_length; j++)
		{
			sig_dest_arr[i + j] = sig_dest_arr[i + j] + (sig_src_arr[i] * imp_resp_arr[j]);
		}
	}

}





// Function to plot impulse response using SWV
static void swv_plot_imp_resp(void)
{
	for(int i = 0; i < IMP_RSP_LENGTH; i++)
	{
		g_imp_resp_sample = impulse_response[i];
	}
}

// Function to plot using the SWV
static void swv_plot_signal(void)
{
	for(int i = 0; i < KHZ1_15_SIG_LEN; i++)
	{
		g_in_sig_sample = input_signal_f32_1kHz_15kHz[i];
		pseudo_dly(9000);
	}
}

// Function to plot signal using the serial port
static void serial_plot_signal(void)
{
	for(int i = 0; i < KHZ1_15_SIG_LEN; i++)
	{
		printf("%f\r\n", input_signal_f32_1kHz_15kHz[i]);
		pseudo_dly(9000);
	}
}

// Function to plot convolution output result using the serial port
static void serial_plot_output_signal(void)
{
	for(int i = 0; i < (KHZ1_15_SIG_LEN + IMP_RSP_LENGTH - 1); i++)
	{
		printf("%f\r\n", output_sig_arr[i]);
		pseudo_dly(9000);
	}
}


// Function to plot impulse response  using the serial port
static void serial_plot_imp_resp(void)
{
	for(int i = 0; i < IMP_RSP_LENGTH; i++)
	{
		printf("%f\r\n", impulse_response[i]);
		pseudo_dly(9000);
	}
}

// Plot several signals - that have different lengths - ugh
static void serial_plot_all(void)
{
	uint32_t i = 0, j = 0;

	for(i = 0; i < (KHZ1_15_SIG_LEN + IMP_RSP_LENGTH - 1); i++)
	{
		j++;

		if(j == KHZ1_15_SIG_LEN)
		{
			j = 0;
		}

		// Use tabs for Tauno serial plotter or SerialPlot

		printf("%f\t", output_sig_arr[i]);
		printf("%f\r\n", input_signal_f32_1kHz_15kHz[j]);

		pseudo_dly(20000);

	}


}



static void pseudo_dly(int dly)
{
	for(int i = 0; i < dly; i++){}
}

static void fpu_enable(void)
{
	// Enable FPU - Set bits 20, 21, 22, 23 to "1" - See page 264 of Cortex-M4 Devices Users Guide
	SCB->CPACR |= ((3UL << 20) | (3UL << 22));
}









