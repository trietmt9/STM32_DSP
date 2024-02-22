/*
 * mpu9250.c
 *
 *  Created on: Feb 4, 2024
 *      Author: pc
 */
#include <mpu9250.h>

void SPI_WriteRead(uint8_t Byte);
void SPI_Read(uint8_t Addr, uint16_t size, uint8_t* buffer);
void SPI_Write(uint8_t Addr, uint16_t size, uint8_t* buffer);

void IMU_Init()
{
    uint8_t data, address;
    HAL_GPIO_WritePin(IMU_CS_GPIO_Port, IMU_CS_Pin, RESET);
    address = PWR_MGMT_1;
    HAL_SPI_Transmit(&hspi1,&data,sizeof(data),500);
    data = 0x00u; 
    HAL_SPI_Transmit(&hspi1,&data,sizeof(data),500);

    address = SMPLRT_DIV;
    HAL_SPI_Transmit(&hspi1,&data,sizeof(data),500);
    data = 0x00u; 
    HAL_SPI_Transmit(&hspi1,&data,sizeof(data),500);
}
