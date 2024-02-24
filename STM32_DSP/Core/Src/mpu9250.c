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
static void SPI_WriteRead(uint8_t data)
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
static void SPI_Read(uint8_t Addr, uint8_t* pBuffer, uint16_t size)
{
    uint8_t data = Addr|0x80;
    HAL_GPIO_WritePin(IMU_CS_GPIO_Port, IMU_CS_Pin, RESET);
    HAL_SPI_Transmit(&hspi1,&data, sizeof(data), SPI_TIMEOUT);
    HAL_SPI_Receive(&hspi1, (uint8_t)pBuffer, sizeof(pBuffer), SPI_TIMEOUT);
    HAL_GPIO_WritePin(IMU_CS_GPIO_Port, IMU_CS_Pin, SET);
}

/**
 * @brief  Writes multiple bytes to the MPU-9250.
 * @param  Addr: address of register in MPU-9250.
 * @param  pBuffer: buffer to write
 * @param  size: size of the buffer to write
 * @retval None
 */
static void SPI_Write(uint8_t Addr, uint8_t* pBuffer, uint16_t size)
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


void ReadRegisters(uint8_t Addr, uint8_t*pBuffer, uint16_t size)
{
    SPI_Read(Addr, pBuffer, size);
    HAL_Delay(10);
}

void WriteRegisters(uint8_t Addr, uint8_t* pBuffer, uint16_t size)
{
    SPI_Write(Addr,size,pBuffer);
    HAL_Delay(10);
}

/**
 * @brief Reads the WHO_AM_I register to check if the MPU-9250 is present.
 *
 * @return 0 if the MPU-9250 is present, 1 otherwise.
 */
uint8_t IS_MPU9250_ON() {
    uint8_t check_data;
    ReadRegisters(WHO_AM_I, &check_data, sizeof(check_data));
    return check_data;
}

/**
 * @brief Initialize the MPU-9250 sensor.
 * 
 * @return 0 if initialization was successful, 1 otherwise.
 */
void IMU_Init(void)
{
    uint8_t data;
    if (IS_MPU9250_ON() == MPU9250_IS_ON)
    {
        data = 0x00; // set power management register to normal mode
        WriteRegisters(PWR_MGMT_1, &data, 1);
        WriteRegisters(PWR_MGMT_2, &data, 1);
        data = (ACCEL_FS_16G * 8); // set accelerometer range to +/- 16g
        WriteRegisters(ACCEL_CONFIG_1, &data, 1);
        data = (GYRO_FS_250_DPS * 8); // set gyroscope range to +/- 250 degrees/s
        WriteRegisters(GYRO_CONFIG, &data, 1);
        data = 0x01; // set accelerometer bandwidth to 184 Hz
        WriteRegisters(ACCEL_CONFIG_2, &data, 1);
        data = 0x00; // set sample rate to 1 kHz
        WriteRegisters(SMPLRT_DIV, &data, 1);
    }
    else
    {
        return 1;
    }
}

/**
 * @brief Reads the raw sensor data from the MPU-9250 and stores it in the imu_t data structure.
 * 
 * @param data A pointer to the imu_t data structure that will store the raw sensor data.
 */
void IMU_Read_Raw(imu_t* data)
{
    uint8_t Accel_data[6];
    ReadRegisters(ACCEL_OUT, Accel_data, 6);
    data->Ax_RAW = (int16_t)(Accel_data[0] << 8 | Accel_data[1]);
    data->Ay_RAW = (int16_t)(Accel_data[2] << 8 | Accel_data[3]);
    data->Az_RAW = (int16_t)(Accel_data[4] << 8 | Accel_data[5]);

    uint8_t Gyro_data[6];
    ReadRegisters(GYRO_OUT, Gyro_data, 6);
    data->Gx_RAW = (int16_t)(Gyro_data[0] << 8 | Gyro_data[1]);
    data->Gy_RAW = (int16_t)(Gyro_data[2] << 8 | Gyro_data[3]);
    data->Gz_RAW = (int16_t)(Gyro_data[4] << 8 | Gyro_data[5]);

}