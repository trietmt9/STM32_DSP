/*
 * mpu9250.h
 *
 *  Created on: Feb 4, 2024
 *      Author: pc
 */

#ifndef INC_MPU9250_H_
#define INC_MPU9250_H_

/** MPU9250 REGISTERS USED IN THIS PROJECT MACROS **/

/* MACROS for initialize the IMU */
#define WHO_AM_I                                        0X68
#define SMPLRT_DIV                                      0X2
#define PWR_MGMT_1                                      0X6B
#define PWR_MGMT_2                                      0X6C
#define GYRO_CONFIG                                     0X1B
#define ACCEL_CONFIG                                    0X1C
#define ACCEL_CONFIG2                                   0X1D

/* MACROS for reading the IMU*/
#define GYRO_OUT                                        0X43
#define ACCEL_OUT                                       0X3B

#endif /* INC_MPU9250_H_ */
