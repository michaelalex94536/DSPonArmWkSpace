/*
 * uart.c
 *
 *  Created on: Dec 22, 2024
 *      Author: michael
 */

#include "uart.h"
#include "stm32f4xx.h"

#define GPIOAEN					(1U << 0)		// GPIOA clock enable bit in AHB1ENR register
#define UART2EN					(1U << 17)	// UART2 enable in APB1ENR register
#define CR1_TE					(1U << 3)		// Enable USART transmit
#define CR1_UE					(1U << 13)	// Enable USART

#define SR_TXE					(1U << 7)		// Transmit data register status

#define SYS_FREQ				16000000
#define APB1_CLK				SYS_FREQ

#define UART_BAUDRATE		115200

static void uart2_set_baudrate(uint32_t periph_clk, uint32_t baudrate);
static uint16_t compute_uart_bd(uint32_t periph_clk, uint32_t baudrate);

void uart2_write(int ch);

int __io_putchar(int ch)
{
	uart2_write(ch);

	return ch;

}

// Function to initialize UART2
void uart2_tx_init(void)
{
	/****** [1] Configure UART2 Pins on Port A, PA2 (Tx) and PA3 (Rx) ******/
	/*  Enable clock access to GPIOA on AHB1 bus  */
	RCC->AHB1ENR |= GPIOAEN;

	/*  Set PA2 mode to alternate function mode. (Set bits 5 and 4 to 0b10) */
	GPIOA->MODER |= (1U << 5);
	GPIOA->MODER &= ~(1U << 4);

	/*  Set PA2 alternate function mode to UART_TX (AF07) . (Set bits 11, 10, 9, 8 to 0b0111, respectively.) */
	/*  Note this is in the alternate function low register, since we are working with PA2  */
	/*  Also note that the AF low register is AFR[0] and the AF high register is AFR[1]  */
	GPIOA->AFR[0] |= (1U << 8);
	GPIOA->AFR[0] |= (1U << 9);
	GPIOA->AFR[0] |= (1U << 10);
	GPIOA->AFR[0] &= ~(1U << 11);


	/****** [2] Configure UART  ******/
	/*  Enable clock access to UART2 on APB1*/
	RCC->APB1ENR |= UART2EN;

	/*  Configure baud rate  */
	uart2_set_baudrate(APB1_CLK, UART_BAUDRATE);

	/*  Configure data transfer direction for Tx  */
	USART2->CR1 |= CR1_TE;

	/*  Enable UART module */
	USART2->CR1 |= CR1_UE;

}

static void uart2_set_baudrate(uint32_t periph_clk, uint32_t baudrate)
{
	USART2->BRR = compute_uart_bd(periph_clk, baudrate);
}

static uint16_t compute_uart_bd(uint32_t periph_clk, uint32_t baudrate)
{
	return ((baudrate/2U) + periph_clk)/baudrate;
}

void uart2_write(int ch)
{
	/*  First, make sure data transmit register is empty - need to read the USART status register, SR */
	/*  Read the status register  */
	while(!(USART2->SR & SR_TXE) ) {}// Sit here until SR bit is a "1" (Data has been transferred)

	/*  Write to the transmit data register.  */
	USART2->DR = (ch & 0xFF);
}





