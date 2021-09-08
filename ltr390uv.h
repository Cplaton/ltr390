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

int8_t ltr390_get_regs(uint8_t reg_addr, uint8_t *reg_data, uint16_t len, const struct ltr390_dev *dev);

int8_t ltr390_set_regs(uint8_t *reg_addr, const uint8_t *reg_data, uint8_t len, const struct ltr390_dev *dev);

int8_t ltr390_soft_reset(const struct ltr390_dev *dev);

#endif /* LTR390_H_ */ 