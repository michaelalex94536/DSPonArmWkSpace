#include "stm32f4xx.h"
#include "signals.h"


extern float _5hz_signal[HZ_5_SIG_LEN];
float g_in_sig_sample;  // Global variable that holds the input sample(s)

static void swv_plot_signal(void);
static void pseudo_dly(int dly);

int main ()
{
	// Enable FPU
	SCB->CPACR |= ((3UL << 20) | (3UL << 22));


	while(1)
	{
		swv_plot_signal();
	}
}


static void swv_plot_signal(void)
{
	for(int i = 0; i < HZ_5_SIG_LEN; i++)
	{
		g_in_sig_sample = _5hz_signal[i];
		pseudo_dly(9000);
	}
}

static void pseudo_dly(int dly)
{
	for(int i = 0; i < dly; i++){}
}
