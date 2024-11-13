#ifndef DRV8711_H
#define DRV8711_H

#include "stm32l4xx_hal.h"

uint16_t spi_transfer(uint16_t data);
void drv8711_write_register(uint8_t address, uint16_t data);
void configure_drv8711(void);
void motor_step(uint16_t steps, uint8_t direction);
void Error_Handler(void);

#endif
