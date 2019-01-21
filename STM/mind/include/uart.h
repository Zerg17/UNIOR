#ifndef UART
#define UART

#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/usart.h>
#include <libopencm3/stm32/dma.h>
#include <stdint.h>

void usart_setup();
void uPD(char*);
uint16_t strl(char*);

#endif