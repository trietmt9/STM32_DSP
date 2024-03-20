/*
 * mpu9250-i2c.c
 *
 *  Created on: Feb 27, 2024
 *      Author: Triet
 */

#include <mpu9250-i2c.h>
extern I2C_HandleTypeDef hi2c1;
uint8_t CHECK_MPU9250(uint8_t addr)
{   
    uint8_t check;
    uint8_t _addr = addr << 1;
    HAL_I2C_Mem_Read(&hi2c1, _addr, WHO_AM_I, 1,&check, 1,I2C_TIMEOUT);
    if(check == MPU9250_IS_ON)
    {
        return 0;
    }
    else 
    {
        return 1;
    }
}   
// void ACCEL_SCALE(I2C_HandleTypeDef* I2Cx, uint8_t _addr, uint8_t Scale, imu_t* pScaleAcc)
// {
//     switch(Scale)
//     {
//         case ACCEL_FS_2G:
//             pScaleAcc->ScaleData_t.AccelerometerScaleFactor = 16384.0;
//             HAL_I2C_Mem_Write(&I2Cx, _addr, ACCEL_CONFIG_1, 1, ACCEL_FS_2G, 1, I2C_TIMEOUT);  
//             break;
//         case ACCEL_FS_4G:
//             pScaleAcc->ScaleData_t.AccelerometerScaleFactor = 8192.0;
//             HAL_I2C_Mem_Write(&I2Cx, _addr, ACCEL_CONFIG_1, 1, ACCEL_FS_4G, 1, I2C_TIMEOUT);  
//             break;
//         case ACCEL_FS_8G:
//             pScaleAcc->ScaleData_t.AccelerometerScaleFactor = 4096.0;
//             HAL_I2C_Mem_Write(&I2Cx, _addr, ACCEL_CONFIG_1, 1, ACCEL_FS_8G, 1, I2C_TIMEOUT);  
//             break;
//         case ACCEL_FS_16G:
//             pScaleAcc->ScaleData_t.AccelerometerScaleFactor = 2048.0;
//             HAL_I2C_Mem_Write(&I2Cx, _addr, ACCEL_CONFIG_1, 1, ACCEL_FS_16G, 1, I2C_TIMEOUT);  
//             break;
//         default:
//             pScaleAcc->ScaleData_t.AccelerometerScaleFactor = 16384.0;
//             HAL_I2C_Mem_Write(&I2Cx, _addr, ACCEL_CONFIG_1, 1, ACCEL_FS_2G, 1, I2C_TIMEOUT);  
//             break;
//     }
// }

// void GYRO_SCALE(I2C_HandleTypeDef* I2Cx, uint8_t _addr, uint8_t Scale, imu_t* pScaleGyro)
// {
//     switch(Scale)
//     {
//         case GYRO_FS_250_DPS:
//             pScaleGyro->ScaleData_t.GyroScaleFactor = 131.0;
//             HAL_I2C_Mem_Write(&I2Cx, _addr, GYRO_CONFIG, 1, GYRO_FS_250_DPS, 1, I2C_TIMEOUT);  
//             break;
//         case GYRO_FS_500_DPS:
//             pScaleGyro->ScaleData_t.GyroScaleFactor = 65.5;
//             HAL_I2C_Mem_Write(&I2Cx, _addr, GYRO_CONFIG, 1, GYRO_FS_500_DPS, 1, I2C_TIMEOUT);  
//             break;
//         case GYRO_FS_1000_DPS:
//             pScaleGyro->ScaleData_t.GyroScaleFactor = 32.8;
//             HAL_I2C_Mem_Write(&I2Cx, _addr, GYRO_CONFIG, 1, GYRO_FS_1000_DPS, 1, I2C_TIMEOUT);  
//             break;
//         case GYRO_FS_2000_DPS:
//             pScaleGyro->ScaleData_t.GyroScaleFactor = 16.4;
//             HAL_I2C_Mem_Write(&I2Cx, _addr, GYRO_CONFIG, 1, GYRO_FS_2000_DPS, 1, I2C_TIMEOUT);  
//             break;
//         default:
//             pScaleGyro->ScaleData_t.GyroScaleFactor = 131.0;
//             HAL_I2C_Mem_Write(&I2Cx, _addr, GYRO_CONFIG, 1, GYRO_FS_250_DPS, 1, I2C_TIMEOUT);  
//             break;
//     }
// }
void IMU_INIT(imu_t* pIMU)
{
    uint8_t _addr = MPU9250_ADDRESS << 1;
    uint8_t data;
    
    if(CHECK_MPU9250(MPU9250_ADDRESS) == HAL_OK)
    {
        data = 0x00u;
        HAL_I2C_Mem_Write(&hi2c1, _addr, PWR_MGMT_1, 1, &data , 1,I2C_TIMEOUT);
        data = 0x00u;
        HAL_I2C_Mem_Write(&hi2c1, _addr, SMPLRT_DIV, 1, &data, 1,I2C_TIMEOUT);
        // ACCEL_SCALE(&I2Cx, _addr, AccelScale, &pIMU);
        // GYRO_SCALE(&I2Cx, _addr, GyroScale, &pIMU);
        data = GYRO_FS_250_DPS;
        HAL_I2C_Mem_Write(&hi2c1, _addr, GYRO_CONFIG, 1, &data, 1,I2C_TIMEOUT);
        data = ACCEL_FS_2G;
        HAL_I2C_Mem_Write(&hi2c1, _addr, ACCEL_CONFIG_1, 1, &data, 1,I2C_TIMEOUT);
    }
}

uint8_t IMU_read(imu_t *pIMU)
{
    uint8_t _addr = MPU9250_ADDRESS << 1;
    uint8_t Acceldata[6];
    HAL_I2C_Mem_Read(&hi2c1, _addr, ACCEL_OUT, 1, &Acceldata, 6, I2C_TIMEOUT);
    pIMU->RawData_t.Ax_RAW = (int16_t)((Acceldata[0] << 8) | Acceldata[1]);
    pIMU->RawData_t.Ay_RAW = (int16_t)((Acceldata[2] << 8) | Acceldata[3]);
    pIMU->RawData_t.Az_RAW = (int16_t)((Acceldata[4] << 8) | Acceldata[5]);

    uint8_t Gyrodata[6];
    HAL_I2C_Mem_Read(&hi2c1, _addr, GYRO_OUT, 1, &Gyrodata, 6, I2C_TIMEOUT);
    pIMU->RawData_t.Gx_RAW = (int16_t)((Gyrodata[0] << 8) | Gyrodata[1]);
    pIMU->RawData_t.Gy_RAW = (int16_t)((Gyrodata[2] << 8) | Gyrodata[3]);
    pIMU->RawData_t.Gz_RAW = (int16_t)((Gyrodata[4] << 8) | Gyrodata[5]);

    pIMU->Data_t.Ax = (float)pIMU->RawData_t.Ax_RAW/16384.0;
    pIMU->Data_t.Ay = (float)pIMU->RawData_t.Ay_RAW/16384.0;
    pIMU->Data_t.Az = (float)pIMU->RawData_t.Az_RAW/16384.0;

    pIMU->Data_t.Gx = (float)pIMU->RawData_t.Gx_RAW/131.0;
    pIMU->Data_t.Gy = (float)pIMU->RawData_t.Gy_RAW/131.0;
    pIMU->Data_t.Gz = (float)pIMU->RawData_t.Gz_RAW/131.0;
}
