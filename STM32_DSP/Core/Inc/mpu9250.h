/*
 * mpu9250.h
 *
 *  Created on: Feb 4, 2024
 *      Author: pc
 */

#ifndef INC_MPU9250_H_
#define INC_MPU9250_H_
#include <stdint.h>
#include <stm32f446xx.h>
#include <math.h>
#include <main.h>

extern SPI_HandleTypeDef hspi1;

/** MPU9250 REGISTERS USED IN THIS PROJECT MACROS **/

/* MACROS for initialize the IMU */
#define WHO_AM_I                                        0X68
#define SMPLRT_DIV                                      0X19
#define PWR_MGMT_1                                      0X6B
#define PWR_MGMT_2                                      0X6C
#define GYRO_CONFIG                                     0X1B
#define ACCEL_CONFIG                                    0X1C
#define ACCEL_CONFIG2                                   0X1D

/* MACROS for reading the IMU*/
#define GYRO_OUT                                        0X43
#define ACCEL_OUT                                       0X3B


/** STRUCT FOR READING IMU DATA **/
typedef struct 
{
/* RAW data read from sensor */
    int16_t Gx_RAW;
    int16_t Gy_RAW;
    int16_t Gz_RAW; 
    int16_t Ax_RAW;
    int16_t Ay_RAW;
    int16_t Az_RAW;

/* Variables for calculations */

    float Gx;
    float Gy;
    float Gz;
    float Gx_Callib;
    float Gy_Callib;
    float Gz_Callib;
    float Ax;
    float Ay;
    float Az;
    float Roll;
    float Pitch;
}imu_t;

/* List of mode for configure the sensor */
typedef enum
{
    ACCEL_FS_2G     = 0,
    ACCEL_FS_4G     = 1,
    ACCEL_FS_8G     = 2,
    ACCEL_FS_16G    = 3
}AccelRange_t;

typedef enum
{
    GYRO_FS_250_DPS     = 0,
    GYRO_FS_500_DPS     = 1,
    GYRO_FS_1000_DPS    = 2,
    GYRO_FS_2000_DPS    = 3
}GyroRange_t;

void IMU_Init();
void IMU_Read(imu_t* data); 
void WriteRegister(uint8_t address, uint8_t data);
void ReadRegister(uint8_t address, uint8_t size, uint8_t buffer);

#endif /* INC_MPU9250_H_ */
