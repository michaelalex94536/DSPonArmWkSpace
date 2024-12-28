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
float32_t g_in_sig_sample;

static void swv_plot_signal(void);
static void serial_plot_signal(void);
static void pseudo_dly(int dly);
static void fpu_enable(void);
static float32_t signal_mean(float32_t *sig_src_array, int32_t sig_length);
static float32_t signal_variance(float32_t *sig_src_array, float32_t sig_mean, int32_t sig_length);
static float32_t signal_stddev(float32_t variance);

float32_t g_mean_value;
float32_t g_variance_value;
float32_t g_stddev_value;

int main(void)
{
	// Enable FPU
	fpu_enable();

	// Initialize the UART
	uart2_tx_init();

	g_mean_value = signal_mean(input_signal_f32_1kHz_15kHz, KHZ1_15_SIG_LEN);

	g_variance_value = signal_variance(input_signal_f32_1kHz_15kHz, g_mean_value, KHZ1_15_SIG_LEN);

	g_stddev_value = signal_stddev(g_variance_value);

	while(1)
	{
			//swv_plot_signal();
			serial_plot_signal();
	}


} // End of main


static float32_t signal_mean(float32_t *sig_src_array, int32_t sig_length)
{
	float32_t _mean = 0.0;

	for(uint32_t i = 0; i < sig_length; i++)
	{
		_mean = _mean +  sig_src_array[i];
	}

	_mean = _mean/(float32_t)sig_length;

	return _mean;
}

static float32_t signal_variance(float32_t *sig_src_array, float32_t sig_mean, int32_t sig_length)
{
	float32_t _var = 0.0;

	for(uint32_t i = 0; i < sig_length; i++)
	{
		_var = _var +  powf((sig_mean - sig_src_array[i]), 2);
	}

	_var = _var/(float32_t)(sig_length - 1);

	return _var;
}

static float32_t signal_stddev(float32_t variance)
{
	return sqrt(variance);
}


// Function to plot using the SWV
static void swv_plot_signal(void)
{
	for(int i = 0; i < KHZ1_15_SIG_LEN; i++)
	{
		g_in_sig_sample = input_signal_f32_1kHz_15kHz[i];
		pseudo_dly(90000);
	}
}

// Function to plot using the serial port
static void serial_plot_signal(void)
{
	for(int i = 0; i < KHZ1_15_SIG_LEN; i++)
	{
		//printf("%f\r\n", input_signal_f32_1kHz_15kHz[i]);			// Original syntax

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









