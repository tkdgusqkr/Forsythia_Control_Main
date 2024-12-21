/*
 * TorqueVectoring.c
 * Created on 2019. 12. 10.
 * Author: Dua
 */


/* Includes */
#include "TorqueVectoring.h"
#include <math.h>

/* Macros */
#define TVOPEN_LSD_ON		FALSE
// #define TVOPEN_LSD_GAIN		1.0f

/* Global Variables */
IFX_EXTERN RVC_t RVC;

RVC_TV_t RVC_TV =
{
		.cur_delta_rpm = 0,

		.percent_rpm = 0,

		.min_delta_rpm = 500,
		.max_delta_rpm = 2500,

		.pre_tv_status = RVC_TV_status_neutral,
		.cur_tv_status = RVC_TV_status_neutral,
};
void RVC_TorqueVectoring_reset(void)
{
	RVC_TV.percent_rpm = 0;
}

void RVC_TorqueVectoring_set(void)
{
	float32 tmp_delta_rpm = (float32)(RVC.AmkMonitor.MotorVelocity.velocity_FR - RVC.AmkMonitor.MotorVelocity.velocity_FL);

	if(tmp_delta_rpm > RVC_TV.min_delta_rpm)
	{
		RVC_TV.cur_tv_status = RVC_TV_status_left;
		RVC_TV.cur_delta_rpm = tmp_delta_rpm;
	}
	else if(tmp_delta_rpm < -RVC_TV.min_delta_rpm)
	{
		RVC_TV.cur_tv_status = RVC_TV_status_right;
		RVC_TV.cur_delta_rpm = -tmp_delta_rpm;
	}
	else
	{
		RVC_TV.cur_tv_status = RVC_TV_status_neutral;
		RVC_TV.cur_delta_rpm = fabs(tmp_delta_rpm);
	}

	if((fabs(RVC_TV.cur_tv_status - RVC_TV.pre_tv_status) == 2) || RVC_TV.cur_delta_rpm > RVC_TV.max_delta_rpm)
	{
		RVC_TV.cur_tv_status = RVC_TV_status_neutral;
	}

	if(RVC_TV.cur_tv_status != RVC_TV_status_neutral)
	{
		RVC_TV.percent_rpm = (RVC_TV.cur_delta_rpm - RVC_TV.min_delta_rpm) / (RVC_TV.max_delta_rpm - RVC_TV.min_delta_rpm) * 100;
	}

	RVC_TV.pre_tv_status = RVC_TV.cur_tv_status;
}

/* Function Implementation */
void RVC_TorqueVectoring_run_modeOpen(void)
{
	// if(TVOPEN_LSD_ON == TRUE)
	// {
	// 	// RVC.slip.axle

	// }
	// else
	// {
	// 	float32 fd = 2*RVC.torque.frontDist;
	// 	RVC.torque.frontLeft = fd * RVC.torque.controlled;
	// 	RVC.torque.frontRight = fd * RVC.torque.controlled;
	// 	RVC.torque.rearLeft = RVC.torque.controlled;
	// 	RVC.torque.rearRight = RVC.torque.controlled;
	// }
	
	// float32 fd = 2 * RVC.torque.frontDist;
	// RVC.torque.frontLeft = fd * RVC.torque.controlled;
	// RVC.torque.frontRight = fd * RVC.torque.controlled;
	// RVC.torque.rearLeft = RVC.torque.controlled;
	// RVC.torque.rearRight = RVC.torque.controlled;

	RVC_TorqueVectoring_set();

	if(TVOPEN_LSD_ON == FALSE)
	{
		RVC.torque.frontLeft = RVC.torque.controlled;
		RVC.torque.frontRight = RVC.torque.controlled;
		RVC.torque.rearLeft = RVC.torque.controlled;
		RVC.torque.rearRight = RVC.torque.controlled;
	}
	else
	{
		if(RVC_TV.cur_tv_status == RVC_TV_status_left)
		{
			RVC.torque.frontLeft = RVC.torque.controlled;
			RVC.torque.frontRight = RVC.torque.controlled - RVC_TV.percent_rpm;
			RVC.torque.rearLeft = RVC.torque.controlled;
			RVC.torque.rearRight = RVC.torque.controlled - RVC_TV.percent_rpm;
		}
		else if(RVC_TV.cur_tv_status == RVC_TV_status_right)
		{
			RVC.torque.frontLeft = RVC.torque.controlled - RVC_TV.percent_rpm;
			RVC.torque.frontRight = RVC.torque.controlled;
			RVC.torque.rearLeft = RVC.torque.controlled - RVC_TV.percent_rpm;
			RVC.torque.rearRight = RVC.torque.controlled;
		}
		else
		{
			RVC.torque.frontLeft = RVC.torque.controlled;
			RVC.torque.frontRight = RVC.torque.controlled;
			RVC.torque.rearLeft = RVC.torque.controlled;
			RVC.torque.rearRight = RVC.torque.controlled;

			RVC_TorqueVectoring_reset();
		}
	}

	if(RVC.torque.frontLeft > 100)	RVC.torque.frontLeft = 100;
	if(RVC.torque.frontRight > 100)	RVC.torque.frontRight = 100;
	if(RVC.torque.rearLeft > 100)	RVC.torque.rearLeft = 100;
	if(RVC.torque.rearRight > 100)	RVC.torque.rearRight = 100;

	if(RVC.torque.frontLeft < 0)	RVC.torque.frontLeft = 0;
	if(RVC.torque.frontRight < 0)	RVC.torque.frontRight = 0;
	if(RVC.torque.rearLeft < 0)	RVC.torque.rearLeft = 0;
	if(RVC.torque.rearRight < 0)	RVC.torque.rearRight = 0;

	// if(TVOPEN_LSD_ON == TRUE)
	// {

	// }

	// if(TVOPEN_LSD_ON == TRUE && RVC.diff.error == FALSE &&
	//     SDP_WheelSpeed.velocity.chassis > RVC.lsd.speedLow) // TODO: Diff deadzone
	// {
	// 	if(RVC.diff.rear > 0)
	// 		RVC.lsd.faster = RVC_Lsd_Faster_left;
	// 	else if(RVC.diff.rear < 0)
	// 		RVC.lsd.faster = RVC_Lsd_Faster_right;
	// 	else
	// 		RVC.lsd.faster = RVC_Lsd_Faster_none;

	// 	float absDiffRear = fabs(RVC.diff.rear);
	// 	float slowerTorque = 0;
	// 	float fasterTorque = 0;

	// 	slowerTorque = RVC.torque.controlled * (1.0 + absDiffRear * RVC.lsd.kGain * RVC.lsd.mGain);
	// 	fasterTorque = RVC.torque.controlled * (1.0 - absDiffRear * RVC.lsd.kGain);

	// 	if(absDiffRear > RVC.lsd.diffLimit)
	// 	{
	// 		slowerTorque = RVC.torque.controlled * (1 + RVC.lsd.lGain * (absDiffRear - RVC.lsd.diffLimit));
	// 		fasterTorque = RVC.torque.controlled * (1 - RVC.lsd.lGain * (absDiffRear - RVC.lsd.diffLimit));
	// 	}

	// 	if(slowerTorque > 100)
	// 		slowerTorque = 100;
	// 	if(fasterTorque < 0)
	// 		fasterTorque = 0;

	// 	if(RVC.lsd.faster == RVC_Lsd_Faster_left)
	// 	{
	// 		RVC.torque.rearLeft = fasterTorque;
	// 		RVC.torque.rearRight = slowerTorque;
	// 	}
	// 	else if(RVC.lsd.faster == RVC_Lsd_Faster_right)
	// 	{
	// 		RVC.torque.rearLeft = slowerTorque;
	// 		RVC.torque.rearRight = fasterTorque;
	// 	}
	// 	else
	// 	{
	// 		RVC.torque.rearLeft = RVC.torque.controlled;
	// 		RVC.torque.rearRight = RVC.torque.controlled;
	// 	}
	// }
	// else
	// {
	// 	RVC.torque.rearLeft = RVC.torque.controlled;
	// 	RVC.torque.rearRight = RVC.torque.controlled;
	// }
}

void RVC_TorqueVectoring_run_mode1(void)
{
	// TODO: TV algorithm
	/*Default*/
	RVC_TorqueVectoring_run_modeOpen();
}
