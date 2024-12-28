#include "stm32f4xx.h"

// Defines to enable the on-board LED
#define GPIOAEN			(1U << 0)
#define PIN5			(1U << 5)
#define LED_PIN			PIN5


int main ()
{
	// Enable clock access to GPIOA
	RCC->AHB1ENR |= GPIOAEN;

	// Set pin mode to output
	GPIOA->MODER |= (1U << 10);
	GPIOA->MODER &= ~(1U << 11);

	while(1)
	{
		// Toggle the LED pin
		GPIOA->ODR ^= LED_PIN;

		// Here is a delay function
		for(int i = 0; i < 100000; i++){}

	}
}
