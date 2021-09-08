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

static void interleave_reg_addr(const uint8_t *reg_addr, uint8_t *temp_buff, const uint8_t *reg_data, uint8_t len);


/********************************************************/

int8_t ltr390_get_regs(uint8_t reg_addr, uint8_t *reg_data, uint16_t len, const struct ltr390_dev *dev)
{
	int8_t rslt;

	/* Check for null pointer in the device structure*/
	rslt = null_ptr_check(dev);
	/* Proceed if null check is fine */
	if (rslt == LTR390_OK) {
		/* Read the data  */
		rslt = dev->read(dev->part_id, reg_addr, reg_data, len);
		/* Check for communication error */
		if (rslt != LTR390_OK)
			rslt = LTR390_E_COMM_FAIL;
	}

	return rslt;
}


int8_t ltr390_set_regs(uint8_t *reg_addr, const uint8_t *reg_data, uint8_t len, const struct ltr390_dev *dev)
{
	int8_t rslt;
	uint8_t temp_buff[3]; /* Typically not to write more than 3 registers */

	if (len > 3)
		len = 3;

	uint16_t temp_len;
	uint8_t reg_addr_cnt;

	/* Check for null pointer in the device structure*/
	rslt = null_ptr_check(dev);
	/* Check for arguments validity */
	if ((rslt ==  LTR390_OK) && (reg_addr != NULL) && (reg_data != NULL)) {
		if (len != 0) {
			temp_buff[0] = reg_data[0];
			/* Burst write mode */
			if (len > 1) {
				/* Interleave register address w.r.t data for
				burst write*/
				interleave_reg_addr(reg_addr, temp_buff, reg_data, len);
				temp_len = ((len * 2) - 1);
			} else {
				temp_len = len;
			}
			rslt = dev->write(dev->part_id, reg_addr[0], temp_buff, temp_len);
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
    
    rslt = null_ptr_check(dev);
    uint8_t reg_data;

    rslt = ltr390_get_regs(&reg_addr,&reg_data,1,dev);

    if (rslt == LTR390_OK) {

        uint8_t soft_rst_cmd = LTR390_SET_BITS(reg_data,
                                            LTR390_POS_SOFT_RST,
                                            LTR390_MASK_SOFT_RST,
                                            LTR390_VAL_SOFT_RST_EN);

        if (rslt == LTR390_OK) {
            /* Write the soft reset command in the sensor */
            rslt = ltr390_set_regs(&reg_addr, &soft_rst_cmd, 1, dev);
            /* As per data sheet, startup time is 2 ms. */
        }
    }

	return rslt;
}

static void interleave_reg_addr(const uint8_t *reg_addr, uint8_t *temp_buff, const uint8_t *reg_data, uint8_t len)
{
	uint8_t index;

	for (index = 1; index < len; index++) {
		temp_buff[(index * 2) - 1] = reg_addr[index];
		temp_buff[index * 2] = reg_data[index];
	}
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
