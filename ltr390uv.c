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

/********************************************************/
/* header includes */
#include "ltr390uv.h"

static int8_t null_ptr_check(const struct ltr390_dev *dev);

static int8_t get_als_data(uint32_t *data, const struct ltr390_dev *dev);

static int8_t get_uvs_data(uint32_t *data, const struct ltr390_dev *dev);

/********************************************************/


int8_t ltr390_init(struct ltr390_dev *dev)
{
	int8_t rslt;
	/* chip id read try count */
	uint8_t try_count = 5;
	uint8_t part_id = 0;

	/* Check for null pointer in the device structure*/
	rslt = null_ptr_check(dev);
	/* Proceed if null check is fine */
	if (rslt ==  LTR390_OK) {
		while (try_count) {
			/* Read the part id of sensor */
			rslt = ltr390_get_regs(LTR390_REG_PART_ID, &part_id, 1, dev);
			/* Check for part id validity */
			if ((rslt == LTR390_OK) && (part_id == LTR390_PART_ID)) {
				dev->part_id = part_id;
				/* Reset the sensor */
				rslt = ltr390_soft_reset(dev);
				break;
			}
			--try_count;
		}
		/* Chip part id check failed */
		if (!try_count)
			rslt = LTR390_E_DEV_NOT_FOUND;
	}

	return rslt;
}


int8_t ltr390_configure(struct ltr390_dev *dev)
{
	int8_t rslt;
	/* set up UV or ALS mode  */
	rslt = ltr390_set_mode(dev->settings.mode, dev);

	/* default UV mode gain=18x, res=20b, rate>500ms */
	if(dev->settings.mode==LTR390_VAL_UVS_MODE_UVS)
	{
		if(dev->settings.rate<LTR390_VAL_MEAS_RATE_500_MS)
			dev->settings.rate=LTR390_VAL_MEAS_RATE_500_MS;
		
		dev->settings.resolution = LTR390_VAL_RES_20_BIT;

		dev->settings.gain_range = LTR390_VAL_GAIN_RANGE_18;
	}

	/* set up measure rate */
	rslt |= ltr390_set_rate(dev->settings.rate, dev);

	/* set up measure resolution 13-20b */
	rslt |= ltr390_set_resolution(dev->settings.resolution, dev);

	/* set up measure gain 3x-18x */
	rslt |= ltr390_set_gain(dev->settings.gain_range, dev);

	/* set up interrupt */
	rslt |= ltr390_set_int(dev->settings.int_enabled, dev);

	/* if interrupt enabled */
	if(dev->settings.int_enabled==TRUE)
	{
		/* set up interrupt source (UVS/ALS) */
		rslt |= ltr390_set_int_src(dev->settings.int_src, dev);

		/* set up interrupt persist */
		rslt |= ltr390_set_int_pers(dev->settings.int_pers, dev);

		/* set up threshold low */
		rslt |= ltr390_set_thresh_low(dev->settings.int_thresh_low, dev);

		/* set up threshold up */
		rslt |= ltr390_set_thresh_up(dev->settings.int_thresh_up, dev);
	}

	return LTR390_OK;
	
}


int8_t ltr390_get_regs(uint8_t reg_addr, uint8_t *reg_data, uint8_t len, struct ltr390_dev *dev)
{
	int8_t rslt;

	/* Check for null pointer in the device structure*/
	rslt = null_ptr_check(dev);
	/* Proceed if null check is fine */
	if (rslt == LTR390_OK) {
		/* Read the data  */
		rslt = dev->read((uint8_t)((dev->dev_id<<1)|0x01), reg_addr, reg_data, len);
		/* Check for communication error */
		if (rslt != LTR390_OK)
			rslt = LTR390_E_COMM_FAIL;
	}

	return rslt;
}


int8_t ltr390_set_regs(uint8_t *reg_addr, const uint8_t *reg_data, uint8_t len, struct ltr390_dev *dev)
{
	int8_t rslt;

	if (len > 3) /* Typically not to write more than 3 registers */
		len = 3;

	/* Check for null pointer in the device structure*/
	rslt = null_ptr_check(dev);
	/* Check for arguments validity */
	if ((rslt ==  LTR390_OK) && (reg_addr != NULL) && (reg_data != NULL)) {
		if (len > 0) {
			/* write data */
			rslt = dev->write((uint8_t)(dev->dev_id<<1), reg_addr[0], reg_data, len);
			/* Check for communication error */
			if (rslt != LTR390_OK)
				rslt = LTR390_E_COMM_FAIL;
		} else {
			rslt = LTR390_E_INVALID_LEN;
		}
	} else {
		rslt = LTR390_E_NULL_PTR;
	}

	return rslt;
}


int8_t ltr390_soft_reset(const struct ltr390_dev *dev) 
{
    int8_t rslt;
    uint8_t reg_addr = LTR390_REG_MAIN_CTRL;
	uint8_t reg_data;
    
	/* Check for null pointer in the device structure*/
    rslt = null_ptr_check(dev);
	if (rslt == LTR390_OK) {
		/* Get register value*/
		rslt = ltr390_get_regs(&reg_addr,&reg_data,1,dev);
		if (rslt == LTR390_OK) {
			/* prepare command to write */
			uint8_t soft_rst_cmd = LTR390_SET_BITS(reg_data,
												LTR390_POS_SOFT_RST,
												LTR390_MASK_SOFT_RST,
												LTR390_VAL_SOFT_RST_EN);
			if (rslt == LTR390_OK) {
				/* Write the soft reset command in the sensor */
				rslt = ltr390_set_regs(&reg_addr, &soft_rst_cmd, 1, dev);
			}
		}
	}

	return rslt;
}


int8_t ltr390_set_mode(uint8_t mode, const struct ltr390_dev *dev)
{
	int8_t rslt;
    uint8_t reg_addr = LTR390_REG_MAIN_CTRL;
	uint8_t reg_data;
    
	/* Control input value */
	switch (mode) 
	{
		case LTR390_VAL_UVS_MODE_ALS:
		case LTR390_VAL_UVS_MODE_UVS:
			/* Check for null pointer in the device structure*/
			rslt = null_ptr_check(dev);
			if (rslt == LTR390_OK) {
				/* Get register value*/
				rslt = ltr390_get_regs(&reg_addr,&reg_data,1,dev);
				if (rslt == LTR390_OK) {
					/* prepare command to write */
					uint8_t conf_mode = LTR390_SET_BITS(reg_data,
														LTR390_POS_UVS_MODE,
														LTR390_MASK_UVS_MODE,
														mode);
					/* Write mode in the sensor's register */
					rslt = ltr390_set_regs(&reg_addr, &conf_mode, 1, dev);
				}
			}
			break;
		default:
			rslt = LTR390_E_INVALID_VAL;
	} 

	return rslt;
}

int8_t ltr390_set_rate(uint8_t rate, const struct ltr390_dev *dev)
{
	int8_t rslt;
    uint8_t reg_addr = LTR390_REG_ALS_UVS_MEAS_RATE;
	uint8_t reg_data;
    
	/* Control input value */
	switch (rate) 
	{
		case LTR390_VAL_MEAS_RATE_25_MS:
		case LTR390_VAL_MEAS_RATE_50_MS:
		case LTR390_VAL_MEAS_RATE_100_MS:
		case LTR390_VAL_MEAS_RATE_200_MS:
		case LTR390_VAL_MEAS_RATE_500_MS:
		case LTR390_VAL_MEAS_RATE_1000_MS:
		case LTR390_VAL_MEAS_RATE_2000_MS:
			/* Check for null pointer in the device structure*/
			rslt = null_ptr_check(dev);
			if (rslt == LTR390_OK) {
				/* Get register value*/
				rslt = ltr390_get_regs(&reg_addr,&reg_data,1,dev);
				if (rslt == LTR390_OK) {
					/* prepare command to write */
					uint8_t conf_rate = LTR390_SET_BITS(reg_data,
														LTR390_POS_ALS_UVS_MEAS_RATE,
														LTR390_MASK_ALS_UVS_MEAS_RATE,
														rate);
					/* Write mode in the sensor's register */
					rslt = ltr390_set_regs(&reg_addr, &conf_rate, 1, dev);
				}
			}
			break;
		default:
			rslt = LTR390_E_INVALID_VAL;
	} 

	return rslt;
}

int8_t ltr390_set_resolution(uint8_t resolution, const struct ltr390_dev *dev)
{
	int8_t rslt;
    uint8_t reg_addr = LTR390_REG_ALS_UVS_MEAS_RATE;
	uint8_t reg_data;
    
	/* Control input value */
	switch (resolution) 
	{
		case LTR390_VAL_RES_13_BIT:
		case LTR390_VAL_RES_16_BIT:
		case LTR390_VAL_RES_17_BIT:
		case LTR390_VAL_RES_18_BIT:
		case LTR390_VAL_RES_19_BIT:
		case LTR390_VAL_RES_20_BIT:
			/* Check for null pointer in the device structure*/
			rslt = null_ptr_check(dev);
			if (rslt == LTR390_OK) {
				/* Get register value*/
				rslt = ltr390_get_regs(&reg_addr,&reg_data,1,dev);
				if (rslt == LTR390_OK) {
					/* prepare command to write */
					uint8_t conf_res = LTR390_SET_BITS(reg_data,
														LTR390_POS_ALS_UVS_RES,
														LTR390_MASK_ALS_UVS_RES,
														resolution);
					/* Write mode in the sensor's register */
					rslt = ltr390_set_regs(&reg_addr, &conf_res, 1, dev);
				}
			}
			break;
		default:
			rslt = LTR390_E_INVALID_VAL;
	} 

	return rslt;
}

int8_t ltr390_set_gain(uint8_t gain_range, const struct ltr390_dev *dev)
{
	int8_t rslt;
    uint8_t reg_addr = LTR390_POS_ALS_UVS_GAIN_RANGE;
	uint8_t reg_data;
    
	/* Control input value */
	switch (gain_range) 
	{
		case LTR390_VAL_GAIN_RANGE_1:
		case LTR390_VAL_GAIN_RANGE_3:
		case LTR390_VAL_GAIN_RANGE_6:
		case LTR390_VAL_GAIN_RANGE_9:
		case LTR390_VAL_GAIN_RANGE_18:
			/* Check for null pointer in the device structure*/
			rslt = null_ptr_check(dev);
			if (rslt == LTR390_OK) {
				/* Get register value*/
				rslt = ltr390_get_regs(&reg_addr,&reg_data,1,dev);
				if (rslt == LTR390_OK) {
					/* prepare command to write */
					uint8_t conf_gain = LTR390_SET_BITS(reg_data,
														LTR390_POS_ALS_UVS_GAIN_RANGE,
														LTR390_MASK_ALS_UVS_GAIN_RANGE,
														gain_range);
					/* Write mode in the sensor's register */
					rslt = ltr390_set_regs(&reg_addr, &conf_gain, 1, dev);
				}
			}
			break;
		default:
			rslt = LTR390_E_INVALID_VAL;
	} 

	return rslt;
}

int8_t ltr390_set_int(uint8_t int_enabled, const struct ltr390_dev *dev)
{
	int8_t rslt;
    uint8_t reg_addr = LTR390_REG_INT_CFG;
	uint8_t reg_data;
    
	/* Control input value */
	switch (int_enabled) 
	{
		case FALSE:
		case TRUE:
			/* Check for null pointer in the device structure*/
			rslt = null_ptr_check(dev);
			if (rslt == LTR390_OK) {
				/* Get register value*/
				rslt = ltr390_get_regs(&reg_addr,&reg_data,1,dev);
				if (rslt == LTR390_OK) {
					/* prepare command to write */
					uint8_t conf_int = LTR390_SET_BITS(reg_data,
														LTR390_POS_LS_INT_EN,
														LTR390_MASK_LS_INT_EN,
														(int_enabled==TRUE?LTR390_VAL_LS_INT_EN:LTR390_VAL_LS_INT_DIS));
					/* Write mode in the sensor's register */
					rslt = ltr390_set_regs(&reg_addr, &conf_int, 1, dev);
				}
			}
			break;
		default:
			rslt = LTR390_E_INVALID_VAL;
	} 

	return rslt;
}

int8_t ltr390_set_int_src(uint8_t int_src, const struct ltr390_dev *dev)
{
	int8_t rslt;
    uint8_t reg_addr = LTR390_REG_INT_CFG;
	uint8_t reg_data;
    
	/* Control input value */
	switch (int_src) 
	{
		case LTR390_VAL_LS_INT_SEL_ALS:
		case LTR390_VAL_LS_INT_SEL_UVS:
			/* Check for null pointer in the device structure*/
			rslt = null_ptr_check(dev);
			if (rslt == LTR390_OK) {
				/* Get register value*/
				rslt = ltr390_get_regs(&reg_addr,&reg_data,1,dev);
				if (rslt == LTR390_OK) {
					/* prepare command to write */
					uint8_t conf_int_src = LTR390_SET_BITS(reg_data,
														LTR390_POS_LS_INT_SEL,
														LTR390_MASK_LS_INT_SEL,
														int_src);
					/* Write mode in the sensor's register */
					rslt = ltr390_set_regs(&reg_addr, &conf_int_src, 1, dev);
				}
			}
			break;
		default:
			rslt = LTR390_E_INVALID_VAL;
	} 

	return rslt;
}

int8_t ltr390_set_int_pers(uint8_t int_pers, const struct ltr390_dev *dev)
{
	int8_t rslt;
    uint8_t reg_addr = LTR390_REG_INT_PST;
	uint8_t reg_data;
    
	/* Control input value */
	switch (int_pers) 
	{
		case LTR390_VAL_ALS_UVS_TRIG_INT_ALL:
		case LTR390_VAL_ALS_UVS_TRIG_INT_2_CONS:
		case LTR390_VAL_ALS_UVS_TRIG_INT_3_CONS:
		case LTR390_VAL_ALS_UVS_TRIG_INT_4_CONS:
		case LTR390_VAL_ALS_UVS_TRIG_INT_5_CONS:
		case LTR390_VAL_ALS_UVS_TRIG_INT_6_CONS:
		case LTR390_VAL_ALS_UVS_TRIG_INT_7_CONS:
		case LTR390_VAL_ALS_UVS_TRIG_INT_8_CONS:
		case LTR390_VAL_ALS_UVS_TRIG_INT_9_CONS:
		case LTR390_VAL_ALS_UVS_TRIG_INT_10_CONS:
		case LTR390_VAL_ALS_UVS_TRIG_INT_11_CONS:
		case LTR390_VAL_ALS_UVS_TRIG_INT_12_CONS:
		case LTR390_VAL_ALS_UVS_TRIG_INT_13_CONS:
		case LTR390_VAL_ALS_UVS_TRIG_INT_14_CONS:
		case LTR390_VAL_ALS_UVS_TRIG_INT_15_CONS:
		case LTR390_VAL_ALS_UVS_TRIG_INT_16_CONS:
			/* Check for null pointer in the device structure*/
			rslt = null_ptr_check(dev);
			if (rslt == LTR390_OK) {
				/* Get register value*/
				rslt = ltr390_get_regs(&reg_addr,&reg_data,1,dev);
				if (rslt == LTR390_OK) {
					/* prepare command to write */
					uint8_t conf_int_pers = LTR390_SET_BITS(reg_data,
														LTR390_POS_ALS_UVS_PERSIST,
														LTR390_MASK_ALS_UVS_PERSIST,
														int_pers);
					/* Write mode in the sensor's register */
					rslt = ltr390_set_regs(&reg_addr, &conf_int_pers, 1, dev);
				}
			}
			break;
		default:
			rslt = LTR390_E_INVALID_VAL;
	} 

	return rslt;
}

int8_t ltr390_set_thresh_low(uint32_t int_thresh_low, const struct ltr390_dev *dev)
{
	int8_t rslt;
    uint8_t reg_addr[3] = {LTR390_REG_ALS_UVS_THRES_LOW_0,
				LTR390_REG_ALS_UVS_THRES_LOW_1,
				LTR390_REG_ALS_UVS_THRES_LOW_2};
	uint8_t reg_data[3]={0};
	uint8_t conf_thresh_low[3];
    
	/* Check for null pointer in the device structure*/
    rslt = null_ptr_check(dev);
	if (rslt == LTR390_OK) {
		/* Get register value*/
		if (rslt == LTR390_OK) {
			/* prepare command to write */
			conf_thresh_low[0] = LTR390_SET_BITS(reg_data[0],
									LTR390_POS_ALS_UVS_THRES_LOW_0,
									LTR390_MASK_ALS_UVS_THRES_LOW_0,
									LTR390_GET_LSB(int_thresh_low));
			/* Write the soft reset command in the sensor */
			rslt = ltr390_set_regs(reg_addr, conf_thresh_low, 3, dev);

			conf_thresh_low[1] = LTR390_SET_BITS(reg_data[1],
									LTR390_POS_ALS_UVS_THRES_LOW_0,
									LTR390_MASK_ALS_UVS_THRES_LOW_0,
									LTR390_GET_MID(int_thresh_low));

			conf_thresh_low[2] = LTR390_SET_BITS(reg_data[2],
									LTR390_POS_ALS_UVS_THRES_LOW_0,
									LTR390_MASK_ALS_UVS_THRES_LOW_0,
									LTR390_GET_MSB(int_thresh_low));
			
		}
	}

	return rslt;
}

int8_t ltr390_set_thresh_up(uint32_t int_thresh_up, const struct ltr390_dev *dev)
{
	int8_t rslt;
    uint8_t reg_addr[3] = {LTR390_REG_ALS_UVS_THRES_UP_0,
				LTR390_REG_ALS_UVS_THRES_UP_1,
				LTR390_REG_ALS_UVS_THRES_UP_2};
	uint8_t reg_data[3]={0};
	uint8_t conf_thresh_up[3];
    
	/* Check for null pointer in the device structure*/
    rslt = null_ptr_check(dev);
	if (rslt == LTR390_OK) {
		if (rslt == LTR390_OK) {
			/* prepare command to write */
			conf_thresh_up[0] = LTR390_SET_BITS(reg_data[0],
									LTR390_POS_ALS_UVS_THRES_UP_0,
									LTR390_MASK_ALS_UVS_THRES_UP_0,
									LTR390_GET_LSB(int_thresh_up));

			conf_thresh_up[1] = LTR390_SET_BITS(reg_data[1],
									LTR390_POS_ALS_UVS_THRES_UP_0,
									LTR390_MASK_ALS_UVS_THRES_UP_0,
									LTR390_GET_MID(int_thresh_up));

			conf_thresh_up[2] = LTR390_SET_BITS(reg_data[2],
									LTR390_POS_ALS_UVS_THRES_UP_0,
									LTR390_MASK_ALS_UVS_THRES_UP_0,
									LTR390_GET_MSB(int_thresh_up));
			/* Write the soft reset command in the sensor */
			rslt = ltr390_set_regs(reg_addr, conf_thresh_up, 3, dev);
		}
	}

	return rslt;
}

int8_t ltr390_get_raw_data(uint32_t *data, const struct ltr390_dev *dev)
{
	int8_t rslt;

	switch (dev->settings.mode)
	{
		case LTR390_VAL_UVS_MODE_ALS:
			rslt=get_als_data(data, dev);
			break;

		case LTR390_VAL_UVS_MODE_UVS:
			rslt=get_uvs_data(*data, dev);
			break;
		
		default:
			rslt=LTR390_E_INVALID_VAL;
			break;
	}

	return rslt;
}

int8_t ltr390_computed_data(uint32_t raw_data, double *computed_data, const struct ltr390_dev *dev)
{
	int8_t rslt = LTR390_OK;

	switch (dev->settings.mode)
	{
		case LTR390_VAL_UVS_MODE_ALS:
			*computed_data = (double)((0.6*raw_data)/(a_gain[dev->settings.gain_range]*a_int[dev->settings.resolution]))*dev->settings.w_fac;
			break;

		case LTR390_VAL_UVS_MODE_UVS:
			*computed_data = (double)(raw_data/dev->settings.uv_sensitivity)*dev->settings.w_fac;
			break;
		
		default:
			rslt=LTR390_E_INVALID_VAL;
			break;
	}

	return rslt;
}

static int8_t get_als_data(uint32_t *data, const struct ltr390_dev *dev)
{
	int8_t rslt;
    uint8_t reg_addr[3] = {LTR390_REG_ALS_DATA_0,
				LTR390_REG_ALS_DATA_1,
				LTR390_REG_ALS_DATA_2};
	uint8_t reg_data[3]={0};

	/* Check for null pointer in the device structure*/
    rslt = null_ptr_check(dev);
	if (rslt == LTR390_OK) {
		/* Get register value*/
		rslt = ltr390_get_regs(reg_addr,&reg_data,3,dev);
		if (rslt == LTR390_OK) {
			switch(dev->settings.resolution)
			{
				case LTR390_VAL_RES_13_BIT:
					*data=(uint32_t)(((uint32_t)(reg_data[1]<<8)&0x1F00)|(uint32_t)reg_data[0]);
					break;
				case LTR390_VAL_RES_16_BIT:
					*data=(uint32_t)(((uint32_t)(reg_data[1]<<8))|(uint32_t)reg_data[0]);
					break;
				case LTR390_VAL_RES_17_BIT:
					*data=(uint32_t)(((uint32_t)(reg_data[2]<<16)&0x10000)|((uint32_t)(reg_data[1]<<8))|(uint32_t)reg_data[0]);
					break;
				case LTR390_VAL_RES_18_BIT:
					*data=(uint32_t)(((uint32_t)(reg_data[2]<<16)&0x30000)|((uint32_t)(reg_data[1]<<8))|(uint32_t)reg_data[0]);
					break;
				case LTR390_VAL_RES_19_BIT:
					*data=(uint32_t)(((uint32_t)(reg_data[2]<<16)&0x70000)|((uint32_t)(reg_data[1]<<8))|(uint32_t)reg_data[0]);
					break;
				default:
					data=(uint32_t)(((uint32_t)(reg_data[2]<<16)&0xF0000)|((uint32_t)(reg_data[1]<<8))|(uint32_t)reg_data[0]);
					break;
			}
		}
	}

	return rslt;
}

static int8_t get_uvs_data(uint32_t *data, const struct ltr390_dev *dev)
{
	int8_t rslt;
    uint8_t reg_addr[3] = {LTR390_REG_UVS_DATA_0,
				LTR390_REG_UVS_DATA_1,
				LTR390_REG_UVS_DATA_2};
	uint8_t reg_data[3]={0};

	/* Check for null pointer in the device structure*/
    rslt = null_ptr_check(dev);
	if (rslt == LTR390_OK) {
		/* Get register value*/
		rslt = ltr390_get_regs(reg_addr,&reg_data,3,dev);
		if (rslt == LTR390_OK) {
			switch(dev->settings.resolution)
			{
				case LTR390_VAL_RES_13_BIT:
					*data=(uint32_t)(((uint32_t)(reg_data[1]<<8)&0x1F00)|(uint32_t)reg_data[0]);
					break;
				case LTR390_VAL_RES_16_BIT:
					*data=(uint32_t)(((uint32_t)(reg_data[1]<<8))|(uint32_t)reg_data[0]);
					break;
				case LTR390_VAL_RES_17_BIT:
					*data=(uint32_t)(((uint32_t)(reg_data[2]<<16)&0x10000)|((uint32_t)(reg_data[1]<<8))|(uint32_t)reg_data[0]);
					break;
				case LTR390_VAL_RES_18_BIT:
					*data=(uint32_t)(((uint32_t)(reg_data[2]<<16)&0x30000)|((uint32_t)(reg_data[1]<<8))|(uint32_t)reg_data[0]);
					break;
				case LTR390_VAL_RES_19_BIT:
					*data=(uint32_t)(((uint32_t)(reg_data[2]<<16)&0x70000)|((uint32_t)(reg_data[1]<<8))|(uint32_t)reg_data[0]);
					break;
				default:
					*data=(uint32_t)(((uint32_t)(reg_data[2]<<16)&0xF0000)|((uint32_t)(reg_data[1]<<8))|(uint32_t)reg_data[0]);
					break;
			}
		}
	}

	return rslt;
}

static int8_t null_ptr_check(const struct ltr390_dev *dev)
{
	int8_t rslt;

	if ((dev == NULL) || (dev->read == NULL) || (dev->write == NULL)) {
		/* Device structure pointer is not valid */
		rslt = LTR390_E_NULL_PTR;
	} else {
		/* Device structure is fine */
		rslt = LTR390_OK;
	}

	return rslt;
}


/*FOR DEBUG ONLY*/
int main()
{
    return LTR390_OK;
}