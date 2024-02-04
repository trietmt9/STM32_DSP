/*
 * mpu9250.h
 *
 *  Created on: Feb 4, 2024
 *      Author: pc
 */

#ifndef INC_MPU9250_H_
#define INC_MPU9250_H_
#include <stdint.h>
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
}imu;

#endif /* INC_MPU9250_H_ */
