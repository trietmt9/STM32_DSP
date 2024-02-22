/*
 * mpu9250.c
 *
 *  Created on: Feb 4, 2024
 *      Author: pc
 */
#include <mpu9250.h>

/**
 * @brief  Writes a byte to the MPU-9250 and reads back the response.
 * @param  data: The byte to write to the MPU-9250.
 * @retval The byte read from the MPU-9250.
 */
void SPI_WriteRead(uint8_t data)
{
    uint8_t receive_data;
    if(HAL_SPI_TransmitReceive(&hspi1,(uint8_t*)&data,(uint8_t*)&receive_data, sizeof(receive_data), SPI_TIMEOUT) != HAL_OK)
    {
        return 1;
    }
    else 
    {
        return receive_data;
    }
}

/**
 * @brief  Read a byte from the MPU-9250 
 * @param  Addr: address of register in MPU-9250.
 * @param  size: size of the buffer to read
 * @param  buffer: buffer to read
 * @retval The byte read from the MPU-9250.
 */
void SPI_Read(uint8_t Addr, uint8_t* pBuffer, uint16_t size)
{
    uint8_t data = Addr|0x80;
    HAL_GPIO_WritePin(IMU_CS_GPIO_Port, IMU_CS_Pin, RESET);
    HAL_SPI_Transmit(&hspi1,&data, sizeof(data), SPI_TIMEOUT);
    HAL_SPI_Receive(&hspi1, (uint8_t)buffer, sizeof(buffer), SPI_TIMEOUT);
    HAL_GPIO_WritePin(IMU_CS_GPIO_Port, IMU_CS_Pin, SET);
}

/**
 * @brief  Writes multiple bytes to the MPU-9250.
 * @param  Addr: address of register in MPU-9250.
 * @param  size: size of the buffer to write
 * @param  pBuffer: buffer to write
 * @retval None
 */
void SPI_Write(uint8_t Addr, uint8_t* pBuffer, uint16_t size)
{
    HAL_GPIO_WritePin(IMU_CS_GPIO_Port, IMU_CS_Pin, RESET);
    SPI_WriteRead(Addr);
    while(size > 0)
    {
        SPI_WriteRead(*pBuffer);
        pBuffer++;
        size--;
    }
}

void WriteRegisters(uint8_t Addr, uint8_t* pBuffer, uint16_t size)
{
    SPI_Write(Addr,size,pBuffer);
    HAL_Delay(10);
}
void IMU_Init()
{
    
}
