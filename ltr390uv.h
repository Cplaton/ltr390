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

#ifndef LTR390_H_
#define LTR390_H_

/********************************************************/
/* header includes */
#include "ltr390uv_defs.h"


/********************************************************/

int8_t ltr390_get_regs(uint8_t reg_addr, uint8_t *reg_data, uint8_t len, struct ltr390_dev *dev);

int8_t ltr390_set_regs(uint8_t *reg_addr,  uint8_t *reg_data, uint8_t len, struct ltr390_dev *dev);

int8_t ltr390_init(struct ltr390_dev *dev);

int8_t ltr390_configure(struct ltr390_dev *dev);

int8_t ltr390_soft_reset( struct ltr390_dev *dev);

int8_t ltr390_set_mode(uint8_t mode,  struct ltr390_dev *dev);

int8_t ltr390_set_rate(uint8_t rate,  struct ltr390_dev *dev);

int8_t ltr390_set_resolution(uint8_t resolution,  struct ltr390_dev *dev);

int8_t ltr390_set_gain(uint8_t gain_range,  struct ltr390_dev *dev);

int8_t ltr390_set_int(uint8_t int_enabled,  struct ltr390_dev *dev);

int8_t ltr390_set_int_src(uint8_t int_src,  struct ltr390_dev *dev);

int8_t ltr390_set_int_pers(uint8_t int_pers,  struct ltr390_dev *dev);

int8_t ltr390_set_thresh_low(uint32_t int_thresh_low,  struct ltr390_dev *dev);

int8_t ltr390_set_thresh_up(uint32_t int_thresh_up,  struct ltr390_dev *dev);

int8_t ltr390_get_raw_data(uint32_t *data,  struct ltr390_dev *dev);

int8_t ltr390_computed_data(uint32_t raw_data, double *computed_data,  struct ltr390_dev *dev);

#endif /* LTR390_H_ */ 