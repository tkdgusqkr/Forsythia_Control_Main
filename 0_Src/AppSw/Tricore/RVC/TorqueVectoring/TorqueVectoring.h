/*
 * TorqueVectoring.h
 * Created on 2019. 12. 10.
 * Author: Dua
 */

#ifndef TORQUEVECTORING_H_
#define TORQUEVECTORING_H_

/* Includes */
#include "RVC.h"
#include "RVC_privateDataStructure.h"

typedef enum
{
	RVC_TV_status_left = -1,
	RVC_TV_status_neutral = 0,
	RVC_TV_status_right = 1,
} RVC_TV_status;

typedef struct
{
	float32 cur_delta_rpm;

	float32 percent_rpm;

	float32 min_delta_rpm;
	float32 max_delta_rpm;

	int pre_tv_status;
	int cur_tv_status;
} RVC_TV_t;

IFX_EXTERN RVC_TV_t RVC_TV;

/* Functionc Prototypes */
IFX_EXTERN void RVC_TorqueVectoring_run_modeOpen(void);
IFX_EXTERN void RVC_TorqueVectoring_run_mode1(void);
#endif
