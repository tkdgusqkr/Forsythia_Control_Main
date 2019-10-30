/*
 * SchedulerTask_Cpu1.c
 *
 *  Created on: 2019. 10. 16.
 *      Author: Dua
 */


/******************************************************************************/
/*----------------------------------Includes----------------------------------*/
/******************************************************************************/
#include "SchedulerTask_Cpu1.h"
/******************************************************************************/
/*-----------------------------------Macros-----------------------------------*/
/******************************************************************************/

/******************************************************************************/
/*--------------------------------Enumerations--------------------------------*/
/******************************************************************************/


/******************************************************************************/
/*-----------------------------Data Structures--------------------------------*/
/******************************************************************************/


/******************************************************************************/
/*------------------------------Global variables------------------------------*/
/******************************************************************************/
Task_cpu1 Task_core1 =
{
		.flag = FALSE,
};
uint64 stm_buf_c1 = 0;
uint64 stm_buf_c1_delay = 0;
uint64 ticToc_1ms_c1 = 0;
uint64 delay_1ms_c1 = 0;
/******************************************************************************/
/*-------------------------Function Prototypes--------------------------------*/
/******************************************************************************/


/******************************************************************************/
/*------------------------Private Variables/Constants-------------------------*/
/******************************************************************************/


/******************************************************************************/
/*-------------------------Function Implementations---------------------------*/
/******************************************************************************/
void Task_core1_1ms (void)
{
	stm_buf_c1_delay = IfxStm_get(&MODULE_STM0);

	waitTime(TimeConst_100us*6+TimeConst_10us*3);	//wait 630us

	delay_1ms_c1 = (IfxStm_get(&MODULE_STM0) - stm_buf_c1_delay)*1000000/(IfxStm_getFrequency(&MODULE_STM0));
	stm_buf_c1 = IfxStm_get(&MODULE_STM0);

	HLD_Imu_run_1ms_c1();

	ticToc_1ms_c1 = (IfxStm_get(&MODULE_STM0) - stm_buf_c1)*1000000/(IfxStm_getFrequency(&MODULE_STM0));
}



