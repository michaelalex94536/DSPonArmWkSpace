#include "stm32f4xx.h"
#include <stdio.h>
#include "signals.h"
#include "uart.h"

extern float _5hz_signal[HZ_5_SIG_LEN];
float g_in_sig_sample;

void static plot_input_signal(void);
void static pseudo_dly(int dly);

int main(void)
{
	// Enable FPU - Set bits 20, 21, 22, 23 to "1" - See page 264 of Cortex-M4 Devices Users Guide
	SCB->CPACR |= ((3UL << 20) | (3UL << 22));

	// Initialize the UART
	uart2_tx_init();

	while(1)
		{
			//plot_input_signal();

			printf("Hello from STM32...\n\r");

		}
}


void static plot_input_signal(void)
{
	int i;
	for(i = 0; i < HZ_5_SIG_LEN; i++)
	{
		g_in_sig_sample = _5hz_signal[i];
		pseudo_dly(9000);
	}

}
void static pseudo_dly(int dly)
{
	for(int i = 0; i < dly; i++){}

}











