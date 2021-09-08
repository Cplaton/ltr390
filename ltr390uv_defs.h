/* 
 * This file is part of the LTR-390-UV-01 library (https://github.com/Cplaton/ltr390).
 * Copyright (c) 2021 Clement Platon.
 * 
 * This program is free software: you can redistribute it and/or modify  
 * it under the terms of the GNU General Public License as published by  
 * the Free Software Foundation, version 3.
 *
 * This program is distributed in the hope that it will be useful, but 
 * WITHOUT ANY WARRANTY; without even the implied warranty of 
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU 
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License 
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef LTR390_DEFS_H_
#define LTR390_DEFS_H_

/********************************************************/
/* header includes */
#include <stdint.h>
#include <stddef.h>


/********************************************************/
/*                      Macros                          */
/********************************************************/


/* C standard macros */
#ifndef NULL
#ifdef __cplusplus
#define NULL   0
#else
#define NULL   ((void *) 0)
#endif
#endif

#ifndef TRUE
#define TRUE                1
#endif
#ifndef FALSE
#define FALSE               0
#endif

/********************************************************/
/*                      Consts                          */
/********************************************************/

#define LTR390_I2C_ADDR_BASE                    0x53

#define LTR390_I2C_ADDR_WRITE                   0xA6
#define LTR390_I2C_ADDR_READ                    0xA7

/* Registers Addresses */
#define LTR390_REG_MAIN_CTRL                    0x00
#define LTR390_REG_ALS_UVS_MEAS_RATE            0x04
#define LTR390_REG_ALS_UVS_GAIN                 0x05
#define LTR390_REG_PART_ID                      0x06
#define LTR390_REG_MAIN_STATUS                  0x07
#define LTR390_REG_ALS_DATA_0                   0x0D
#define LTR390_REG_ALS_DATA_1                   0x0E
#define LTR390_REG_ALS_DATA_2                   0x0F
#define LTR390_REG_UVS_DATA_0                   0x10
#define LTR390_REG_UVS_DATA_1                   0x11
#define LTR390_REG_UVS_DATA_2                   0x12
#define LTR390_REG_INT_CFG                      0x19
#define LTR390_REG_INT_PST                      0x1A
#define LTR390_REG_ALS_UVS_THRES_UP_0           0x21
#define LTR390_REG_ALS_UVS_THRES_UP_1           0x22
#define LTR390_REG_ALS_UVS_THRES_UP_2           0x23
#define LTR390_REG_ALS_UVS_THRES_LOW_0          0x24
#define LTR390_REG_ALS_UVS_THRES_LOW_1          0x25
#define LTR390_REG_ALS_UVS_THRES_LOW_2          0x26



/* Masks */
#define LTR390_MASK_ALS_UVS_EN                  0x01
#define LTR390_MASK_UVS_MODE                    0x04
#define LTR390_MASK_SOFT_RST                    0x08

#define LTR390_MASK_ALS_UVS_MEAS_RATE           0x07
#define LTR390_MASK_ALS_UVS_RES                 0x70

#define LTR390_MASK_ALS_UVS_GAIN_RANGE      	0x07

#define LTR390_MASK_REV_ID                  	0x0F
#define LTR390_MASK_PART_ID                 	0xF0

#define LTR390_MASK_ALS_UVS_DATA_STAT       	0x04
#define LTR390_MASK_ALS_UVS_INT_STAT        	0x08
#define LTR390_MASK_ALS_UVS_PWR_ON_STAT     	0x10

#define LTR390_MASK_ALS_DATA_0              	0xFF
#define LTR390_MASK_ALS_DATA_1              	0xFF
#define LTR390_MASK_ALS_DATA_2              	0x0F

#define LTR390_MASK_UVS_DATA_0              	0xFF
#define LTR390_MASK_UVS_DATA_1              	0xFF
#define LTR390_MASK_UVS_DATA_2              	0x0F

#define LTR390_MASK_LS_INT_EN               	0x02
#define LTR390_MASK_LS_INT_SEL              	0x30

#define LTR390_MASK_ALS_UVS_PERSIST         	0xF0

#define LTR390_MASK_ALS_UVS_THRES_LOW_0         0xFF
#define LTR390_MASK_ALS_UVS_THRES_LOW_1         0xFF
#define LTR390_MASK_ALS_UVS_THRES_LOW_2         0x0F



/* Values */
#define LTR390_VAL_ALS_UVS_STANDBY          	0x00
#define LTR390_VAL_ALS_UVS_ACTIVE           	0x01

#define LTR390_VAL_UVS_MODE_ALS             	0x00
#define LTR390_VAL_UVS_MODE_UVS             	0x01

#define LTR390_VAL_SOFT_RST_DIS             	0x00
#define LTR390_VAL_SOFT_RST_EN              	0x01

#define LTR390_VAL_MEAS_RATE_25_MS          	0x00
#define LTR390_VAL_MEAS_RATE_50_MS          	0x01
#define LTR390_VAL_MEAS_RATE_100_MS         	0x02
#define LTR390_VAL_MEAS_RATE_200_MS         	0x03
#define LTR390_VAL_MEAS_RATE_500_MS         	0x04
#define LTR390_VAL_MEAS_RATE_1000_MS        	0x05
#define LTR390_VAL_MEAS_RATE_2000_MS        	0x06

#define LTR390_VAL_RES_20_BIT               	0x00
#define LTR390_VAL_RES_19_BIT               	0x01
#define LTR390_VAL_RES_18_BIT               	0x02
#define LTR390_VAL_RES_17_BIT               	0x03
#define LTR390_VAL_RES_16_BIT               	0x04
#define LTR390_VAL_RES_13_BIT               	0x05

#define LTR390_VAL_GAIN_RANGE_1             	0x00
#define LTR390_VAL_GAIN_RANGE_3             	0x01
#define LTR390_VAL_GAIN_RANGE_6             	0x02
#define LTR390_VAL_GAIN_RANGE_9             	0x03
#define LTR390_VAL_GAIN_RANGE_18            	0x04

#define LTR390_VAL_ALS_UVS_DATA_OLD         	0x00
#define LTR390_VAL_ALS_UVS_DATA_NEW         	0x01

#define LTR390_VAL_ALS_UVS_INT_NOT_TRIG     	0x00
#define LTR390_VAL_ALS_UVS_INT_TRIG         	0x01

#define LTR390_VAL_PWR_ON_EVT_CLR           	0x00
#define LTR390_VAL_PWR_ON_EVT_SET           	0x01

#define LTR390_VAL_LS_INT_DIS               	0x00
#define LTR390_VAL_LS_INT_EN                	0x01

#define LTR390_VAL_LS_INT_SEL_ALS           	0x01
#define LTR390_VAL_LS_INT_SEL_UVS           	0x03

#define LTR390_VAL_ALS_UVS_TRIG_INT_ALL     	0x00
#define LTR390_VAL_ALS_UVS_TRIG_INT_2_CONS  	0x01
#define LTR390_VAL_ALS_UVS_TRIG_INT_3_CONS  	0x02
#define LTR390_VAL_ALS_UVS_TRIG_INT_4_CONS  	0x03
#define LTR390_VAL_ALS_UVS_TRIG_INT_5_CONS  	0x04
#define LTR390_VAL_ALS_UVS_TRIG_INT_6_CONS  	0x05
#define LTR390_VAL_ALS_UVS_TRIG_INT_7_CONS  	0x06
#define LTR390_VAL_ALS_UVS_TRIG_INT_8_CONS  	0x07
#define LTR390_VAL_ALS_UVS_TRIG_INT_9_CONS  	0x08
#define LTR390_VAL_ALS_UVS_TRIG_INT_10_CONS 	0x09
#define LTR390_VAL_ALS_UVS_TRIG_INT_11_CONS 	0x0A
#define LTR390_VAL_ALS_UVS_TRIG_INT_12_CONS 	0x0B
#define LTR390_VAL_ALS_UVS_TRIG_INT_13_CONS 	0x0C
#define LTR390_VAL_ALS_UVS_TRIG_INT_14_CONS 	0x0D
#define LTR390_VAL_ALS_UVS_TRIG_INT_15_CONS 	0x0E
#define LTR390_VAL_ALS_UVS_TRIG_INT_16_CONS 	0x0F



/* settings */


/* Type definitions */
typedef int8_t (*ltr390_com_fptr_t)(uint8_t dev_id, uint8_t reg_addr, uint8_t *data, uint16_t len);


/* ltr390 settings structure */
struct ltr390_settings {
    /* ALS/UVS */
    uint8_t mode;
    /* Measures rate */
    uint8_t rate;
    /* Measures resolution */
    uint8_t resolution;
    /* Gain Range */
    uint8_t gain_range;

};

/* ltr390 device structure */
struct ltr390_dev {
    /* Chip Id */
    uint8_t chip_id;
    /* Device Id */
    uint8_t dev_id;
    /* Read function pointer */
    ltr390_com_fptr_t read;
    /* Write function pointer */
    ltr390_com_fptr_t write;
    /* Sensor settings */
<<<<<<< HEAD
    struct ltr390_settings settings;
=======
    struct bme280_settings settings;
>>>>>>> e26b1db8342f130e12330c33d7b7282e99f8f0dc
};

#endif /* LTR390_DEFS_H_ */