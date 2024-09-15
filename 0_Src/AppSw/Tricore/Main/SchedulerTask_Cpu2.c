/*
 * SchedulerTask_Cpu2.c
 *
 *  Created on: 2019. 10. 30.
 *      Author: Dua
 */


/******************************************************************************/
/*----------------------------------Includes----------------------------------*/
/******************************************************************************/
#include "SchedulerTask_Cpu2.h"
#include "AmkInverter_can.h"
#include "OrionBms2.h"
#include "SteeringWheel.h"
#include "AdcSensor.h"
#include "PedalBox.h"
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
Task_cpu2 Task_core2 =
{
	.flag = FALSE,
};
uint64 stm_buf_c2 = 0;
uint64 stm_buf_c2_delay = 0;
uint64 ticToc_1ms_c2 = 0;
uint64 delay_1ms_c2 = 0;
uint16_t task2_10ms_counter = 0;

// extern AdcSensor APPS0;
float32 tpsFl;
float32 tpsFr;
float32 tpsRl;
float32 tpsRr;

boolean brakeOn = FALSE;

sint16 valueFl;
sint16 valueFr;
sint16 valueRl;
sint16 valueRr;

int value = 0;
// boolean RTD_flag;
/******************************************************************************/
/*-------------------------Function Prototypes--------------------------------*/
/******************************************************************************/
IFX_STATIC void Task_core2_10ms_slot0(void);
IFX_STATIC void Task_core2_10ms_slot1(void);

/******************************************************************************/
/*------------------------Private Variables/Constants-------------------------*/
/******************************************************************************/


/******************************************************************************/
/*-------------------------Function Implementations---------------------------*/
/******************************************************************************/
void Task_core2_primaryService(void)
{
	;
}

void Task_core2_1ms(void)
{
	stm_buf_c2_delay = IfxStm_get(&MODULE_STM0);

	delay_1ms_c2 = (IfxStm_get(&MODULE_STM0) - stm_buf_c2_delay) * 1000000 / (IfxStm_getFrequency(&MODULE_STM0));
	stm_buf_c2 = IfxStm_get(&MODULE_STM0);

	SDP_PedalBox_run_1ms();
	SDP_SteeringAngleAdc_run();
	SDP_Accumulator_run_10ms();

	AmkInverter_can_Run();

	OrionBms2_run_1ms_c2();
	
	task2_10ms_counter+=1;
	
	while(IfxCpu_acquireMutex(&AmkInverterPublic.mutex));	//Wait for the mutex
	{
		tpsFl = AmkInverterPublic.fl;
		tpsFr = AmkInverterPublic.fr;
		tpsRl = AmkInverterPublic.rl;
		tpsRr = AmkInverterPublic.rr;

		brakeOn = AmkInverterPublic.brakeOn;

		IfxCpu_releaseMutex(&AmkInverterPublic.mutex);
	}

	valueFl = ((float32)AMK_TORQUE_LIM / (100.0f) * tpsFl);
	valueFr = ((float32)AMK_TORQUE_LIM / (100.0f) * tpsFr);
	valueRl = ((float32)AMK_TORQUE_LIM / (100.0f) * tpsRl);
	valueRr = ((float32)AMK_TORQUE_LIM / (100.0f) * tpsRr);


	// AmkInverter_writeMessage(value,value);
	// AmkInverter_writeMessage2(value,value);

	if(valueFl < 0)	valueFl = 0;
	if(valueFr < 0)	valueFr = 0;
	if(valueRl < 0)	valueRl = 0;
	if(valueRr < 0)	valueRr = 0;


	AmkInverter_writeMessage(valueFl,valueFr);
	AmkInverter_writeMessage2(valueRl,valueRr);
	
	// else if (task2_10ms_counter ==15)
	// SDP_DashBoardCan_run_10ms();

	AmkInverter_Start(RTD_flag);

	if(task2_10ms_counter == 10)
	{
		task2_10ms_counter = 0;
	}
	if(task2_10ms_counter == 0)
	{
		Task_core2_10ms_slot0();
	}
	if(task2_10ms_counter == 1)
	{
		Task_core2_10ms_slot1();
	}
	ticToc_1ms_c2 = (IfxStm_get(&MODULE_STM0) - stm_buf_c2) * 1000000 / (IfxStm_getFrequency(&MODULE_STM0));
}

IFX_STATIC void Task_core2_10ms_slot0(void)
{
	SDP_DashBoardCan_run_10ms();
	// SteeringWheel_run_xms_c2();
	/*
	FIXME:
	tempPedal to target torque value
	target torque value unit : 0.1 % of nominal torque 9.8Nm
	1000-> 100% of nominal torque
	2143 -> 214.3% of nominal torque 21Nm
	maby 21Nm is max by datasheet graph
	*/

}

IFX_STATIC void Task_core2_10ms_slot1(void)
{
	SteeringWheel_run_xms_c2();
}

void Task_core2_backgroundService(void)
{

}
