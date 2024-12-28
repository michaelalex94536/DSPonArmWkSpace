/*
 *  Import and view a low pass filter impulse response
 *
 *  View various signal using the serial wire viewer or using the serial port and another program like Tauno Serial Plotter
 */

#include "stm32f4xx.h"
#include <stdio.h>
#include "signals.h"
#include "uart.h"

extern float _5hz_signal[HZ_5_SIG_LEN];								// 5 Hz sine wave
extern float32_t input_signal_f32_1kHz_15kHz[KHZ1_15_SIG_LEN] ;		// Superposed sine waves at 1 and 15 kHz
extern float32_t  impulse_response[IMP_RSP_LENGTH];					// a low pass filter impulse response

float32_t g_in_sig_sample;
float32_t g_imp_resp_sample;

static void serial_plot_imp_resp(void);
static void swv_plot_imp_resp(void);
static void swv_plot_signal(void);
static void serial_plot_signal(void);
static void pseudo_dly(int dly);
static void fpu_enable(void);


int main(void)
{
	// Enable FPU
	fpu_enable();

	// Initialize the UART
	uart2_tx_init();

	// Send one repeat of the impulse response to the serial port
	//serial_plot_imp_resp();
	while(1)
	{
			//swv_plot_signal();
			//serial_plot_signal();
			//swv_plot_imp_resp();
			serial_plot_imp_resp();
	}


} // End of main

// Plot impulse response using SWV
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

// Function to plot using the serial port
static void serial_plot_signal(void)
{
	for(int i = 0; i < KHZ1_15_SIG_LEN; i++)
	{
		printf("%f\r\n", input_signal_f32_1kHz_15kHz[i]);
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

static void pseudo_dly(int dly)
{
	for(int i = 0; i < dly; i++){}
}

static void fpu_enable(void)
{
	// Enable FPU - Set bits 20, 21, 22, 23 to "1" - See page 264 of Cortex-M4 Devices Users Guide
	SCB->CPACR |= ((3UL << 20) | (3UL << 22));
}









