/*
 *  Here we plot using both the SWV and the serial port
 *  The signal is a mix of 1 kHz and 15 kHz sine waves.
 *  Enabling both seems kind of slow...
 */

#include "stm32f4xx.h"
#include <stdio.h>
#include "signals.h"
#include "uart.h"

extern float _5hz_signal[HZ_5_SIG_LEN];
extern float32_t input_signal_f32_1kHz_15kHz[KHZ1_15_SIG_LEN] ;
float g_in_sig_sample;

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

	while(1)
	{
			swv_plot_signal();
			serial_plot_signal();
	}


} // End of main

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


static void pseudo_dly(int dly)
{
	for(int i = 0; i < dly; i++){}
}

static void fpu_enable(void)
{
	// Enable FPU - Set bits 20, 21, 22, 23 to "1" - See page 264 of Cortex-M4 Devices Users Guide
	SCB->CPACR |= ((3UL << 20) | (3UL << 22));
}









