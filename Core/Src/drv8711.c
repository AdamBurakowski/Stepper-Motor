#include "drv8711.h"
#include "main.h"

uint16_t spi_transfer(uint16_t data){
    uint8_t write_data[2];
    uint8_t read_data[2];

    write_data[0] = (data >> 8) & 0xFF;  // MSB
    write_data[1] = data & 0xFF;         // LSB

    // Activating SCS
    HAL_GPIO_WritePin(GPIOA, SCS_Pin, GPIO_PIN_RESET);

    if(HAL_SPI_TransmitReceive(&hspi1, write_data, read_data, 2, HAL_MAX_DELAY) != HAL_OK){
    // Deactivating SCS
    HAL_GPIO_WritePin(GPIOA, SCS_Pin, GPIO_PIN_SET);
    Error_Handler(); // Handle Error
    }

    // Deactivating SCS
    HAL_GPIO_WritePin(GPIOA, SCS_Pin, GPIO_PIN_SET);

    // Convert read_data from two bytes to 16 bit int and return it
    return ((uint16_t)read_data[0] << 8) | read_data[1];
}

void drv8711_write_register(uint8_t address, uint16_t data){
    uint16_t packet = (address << 12) | (data & 0x0FFF);
    spi_transfer(packet);
}

void configure_drv8711(void){
    drv8711_write_register(0x00, 0x00A8);  // CTRL Register
    drv8711_write_register(0x02, 0x01FF);  // TORQUE Register
}

void motor_step(uint16_t steps, uint8_t direction){
    HAL_GPIO_WritePin(GPIOA, DIR_Pin, (direction ? GPIO_PIN_SET : GPIO_PIN_RESET));
    for(uint16_t i = 0; i < steps; i++){
        HAL_GPIO_WritePin(GPIOA, STEP_Pin, GPIO_PIN_SET);
        HAL_Delay(1);
        HAL_GPIO_WritePin(GPIOA, STEP_Pin, GPIO_PIN_RESET);
        HAL_Delay(1);
    }
}

void Error_Handler(void){
    while(1){
        // LED ON/OFF
        HAL_GPIO_TogglePin(LD2_GPIO_Port, LD2_Pin);
        // IMPORTANT: The LD2_GPIO_Port and LD2_Pin are not declared
        // NOTE TO SELF: Do that in CubeIDE, when you can
        HAL_Delay(100);
    }
}
