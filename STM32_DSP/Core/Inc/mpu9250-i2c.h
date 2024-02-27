/*
 * mpu9250-spi.c
 *
 *  Created on: Feb 27, 2024
 *      Author: Triet
 */
/*
 * mpu9250.h
 *
 *  Created on: Feb 4, 2024
 *      Author: Triet
 */

#ifndef INC_MPU9250_H_
#define INC_MPU9250_H_
#include <stdint.h>
#include <stm32f446xx.h>
#include <math.h>
#include <main.h>

extern I2C_HandleTypeDef hi2c1;

/** MPU9250 REGISTERS USED IN THIS PROJECT MACROS **/
/** Register memory size Variable**/
#define _8_BIT_Register                                 0x8u
#define _16_BIT_Register                                0x10u
#define _32_BIT_Register                                0x20u
#define _64_BIT_Register                                0x40u

/* MACROS for initialize the IMU */
#define WHO_AM_I                                        0x75u
#define SMPLRT_DIV                                      0x19u
#define PWR_MGMT_1                                      0x6Bu
#define PWR_MGMT_2                                      0x6Cu
#define GYRO_CONFIG                                     0x1Bu
#define ACCEL_CONFIG_1                                  0x1Cu
#define ACCEL_CONFIG_2                                  0x1Du
#define MPU9250_IS_ON                                   0x71u
#define USER_CTRL                                       0x6Au                                  
/* MACROS for reading the IMU*/
#define GYRO_OUT                                        0x43u
#define ACCEL_OUT                                       0x3Bu

/* MACROS */

#define I2C_TIMEOUT                                     100          

/** STRUCT FOR READING IMU DATA **/
typedef struct 
{
    struct 
    {
        /* RAW data read from sensor */
        int16_t Gx_RAW;
        int16_t Gy_RAW;
        int16_t Gz_RAW; 
        int16_t Ax_RAW;
        int16_t Ay_RAW;
        int16_t Az_RAW;
    }RawData_t;
    

    struct 
    {
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
    }Data_t;
    
    struct
    {
    float GyroScaleFactor;
    float AccelerometerScaleFactor; 
    }ScaleData_t; 
}imu_t;

/* List of mode for configure the sensor */
typedef enum
{
    ACCEL_FS_2G,
    ACCEL_FS_4G,
    ACCEL_FS_8G,
    ACCEL_FS_16G
}AccelRange_t;

typedef enum
{
    GYRO_FS_250_DPS,
    GYRO_FS_500_DPS,
    GYRO_FS_1000_DPS,
    GYRO_FS_2000_DPS
}GyroRange_t;

typedef enum
{
    DLPF_CFG_250Hz,
    DLPF_CFG_184Hz,
    DLPF_CFG_92Hz,
    DLPF_CFG_41Hz,
    DLPF_CFG_20Hz,
    DLPF_CFG_10Hz,
    DLPF_CFG_5Hz,
}DLPFcfg_t;

uint8_t IMU_Init(imu_t* Data);
void IMU_Read(imu_t* data); 

#endif /* INC_MPU9250_H_ */
