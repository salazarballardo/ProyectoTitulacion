/*************************************************************************
* Description: Libreria i2c para el ATMEGA328P
* Author:      Ing. Misael Salazar
* File:        i2cmaster.h
* Target:      ATMEGA328P
**************************************************************************/

#ifndef I2CMASTER_H_
#define I2CMASTER_H_

#include <avr/io.h>
#include <inttypes.h>
#include <compat/twi.h>
#include "../config_cpu.h"
#include <util/delay.h>


/* I2C clock in Hz */
#define SCL_CLOCK  100000L

/** defines the data direction (reading from I2C device) in i2c_start(),i2c_rep_start() */
#define I2C_READ    1

/** defines the data direction (writing to I2C device) in i2c_start(),i2c_rep_start() */
#define I2C_WRITE   0

/**
 @brief initialize the I2C master interace. Need to be called only once 
 @param  void
 @return none
 */
void i2c_init(void);

/** 
 @brief Terminates the data transfer and releases the I2C bus 
 @param void
 @return none
 */
void i2c_stop(void);

/** 
 @brief Issues a start condition and sends address and transfer direction 
  
 @param    addr address and transfer direction of I2C device
 @retval   0   device accessible 
 @retval   1   failed to access device 
 */
unsigned char i2c_start(unsigned char addr);

/**
 @brief Issues a repeated start condition and sends address and transfer direction 
 @param   addr address and transfer direction of I2C device
 @retval  0 device accessible
 @retval  1 failed to access device
 */
unsigned char i2c_rep_start(unsigned char addr);

/**
 @brief Issues a start condition and sends address and transfer direction 
   
 If device is busy, use ack polling to wait until device ready 
 @param    addr address and transfer direction of I2C device
 @return   none
 */
void i2c_start_wait(unsigned char addr);

/**
 @brief Send one byte to I2C device
 @param    data  byte to be transfered
 @retval   0 write successful
 @retval   1 write failed
 */
unsigned char i2c_write(unsigned char data);

/**
 @brief    read one byte from the I2C device, request more data from device 
 @return   byte read from I2C device
 */
unsigned char i2c_readAck(void);

/**
 @brief    read one byte from the I2C device, read is followed by a stop condition 
 @return   byte read from I2C device
 */
unsigned char i2c_readNak(void);

#endif /* I2CMASTER_H_ */

