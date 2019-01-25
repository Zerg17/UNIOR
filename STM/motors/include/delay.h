#ifndef DELAY
#define DELAY

#include <stdint.h>
#include <libopencm3/stm32/rcc.h>

#define  DWT_CYCCNT    *(volatile uint32_t *)0xE0001004
#define  DWT_CONTROL   *(volatile uint32_t *)0xE0001000
#define  SCB_DEMCR     *(volatile uint32_t *)0xE000EDFC

void delayInit();               // Инициализация инкрементального счетчика работающего с частотой главной шины
void _delay_loop(uint32_t);     // Задержка в тактах процессора до 4294967296 тактов
void _delay_us(uint32_t);       // Задержка в микросекундах до 59652323us
void _delay_ms(uint32_t);       // Задержка в миллисекундах до 59652ms

#endif