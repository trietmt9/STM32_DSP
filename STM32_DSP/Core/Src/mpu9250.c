/*
 * mpu9250.c
 *
 *  Created on: Feb 4, 2024
 *      Author: Triet
 */
#include <mpu9250.h>

/**
 * @brief Activates the SPI bus by setting the Chip Select pin to low.
 */
static inline void SPI_Activate(void) 
{
    HAL_GPIO_WritePin(IMU_CS_GPIO_Port, IMU_CS_Pin, RESET);
}

/**
 * @brief Deactivates the SPI bus by setting the Chip Select pin to high.
 */
static inline void SPI_Deactivate(void)
{
    HAL_GPIO_WritePin(IMU_CS_GPIO_Port, IMU_CS_Pin, SET);
}

/**
 * @brief Writes a byte to the MPU-9250 and reads back the response.
 * @param  data: The byte to write to the MPU-9250.
 * @retval The byte read from the MPU-9250.
 */
static void WriteReadRegister(uint8_t data)
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
 * @brief Reads a single register from the MPU-9250.
 *
 * @param Register The register to read from.
 * @param pBuffer A pointer to the buffer that will store the data read from the register.
 * @param size The size of the buffer, in bytes.
 */
static void ReadRegister(uint8_t Register, uint8_t* pBuffer, uint16_t size)
{
    uint8_t data = Register|0x80;
    SPI_Active();
    HAL_SPI_Transmit(&hspi1,&data, sizeof(data), SPI_TIMEOUT);
    HAL_SPI_Receive(&hspi1, (uint8_t)pBuffer, sizeof(pBuffer), SPI_TIMEOUT);
    SPI_Deactivate();
}
/**
 * @brief Writes a single register to the MPU-9250.
 *
 * @param Register The register to write to.
 * @param pBuffer A pointer to the data to write to the register.
 */
static void WriteRegister(uint8_t Register, uint8_t* pBuffer)
{
    SPI_Activate();
    HAL_SPI_Transmit(&hspi1, &Register, sizeof(Register), SPI_TIMEOUT);
    HAL_SPI_Transmit(&hspi1, (uint8_t*)pBuffer, sizeof(pBuffer), SPI_TIMEOUT);
    SPI_Deactivate();
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
        // set power management register to normal mode
        data = 0x00; 
        WriteRegister(PWR_MGMT_1, &data);
        WriteRegister(PWR_MGMT_2, &data);

        // set accelerometer range to +/- 16g
        data = (ACCEL_FS_16G * _8_BIT_Register); 
        WriteRegister(ACCEL_CONFIG_1, &data);

        // set gyroscope range to +/- 250 degrees/s
        data = (GYRO_FS_250_DPS * _8_BIT_Register); 
        WriteRegister(GYRO_CONFIG, &data);

        // set accelerometer bandwidth to 184 Hz
        data = DLPF_CFG_184Hz; 
        WriteRegister(ACCEL_CONFIG_2, &data);

        // set sample rate to 1 kHz
        data = 0x00; 
        WriteRegister(SMPLRT_DIV, &data);
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
    ReadRegister(ACCEL_OUT, Accel_data, 6);
    data->Ax_RAW = (int16_t)(Accel_data[0] << 8 | Accel_data[1]);
    data->Ay_RAW = (int16_t)(Accel_data[2] << 8 | Accel_data[3]);
    data->Az_RAW = (int16_t)(Accel_data[4] << 8 | Accel_data[5]);

    uint8_t Gyro_data[6];
    SPI_Read(GYRO_OUT, Gyro_data, 6);
    data->Gx_RAW = (int16_t)(Gyro_data[0] << 8 | Gyro_data[1]);
    data->Gy_RAW = (int16_t)(Gyro_data[2] << 8 | Gyro_data[3]);
    data->Gz_RAW = (int16_t)(Gyro_data[4] << 8 | Gyro_data[5]);

}