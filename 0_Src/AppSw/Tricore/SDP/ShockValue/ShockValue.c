#include "ShockValue.h"

#define SHOCK_CAN_MSG_0     0x405A
#define SHOCK_CAN_MSG_1     0x405B

#define A0STT		(1.15f)
#define A0END		(3.20f)

/*
#define SHOCK_CAN_MSG_0_log     0x275F01
#define SHOCK_CAN_MSG_1_log     0x275F02
*/

ShockCanMsg_data_t ShockCanMsgFront;
ShockCanMsg_data_t ShockCanMsgRear;
ShockValue_t shockValue;

AdcSensor SHOCK0;
AdcSensor SHOCK1;
SDP_ShockValue_pps_t	SDP_ShockValue_pps;
SDP_ShockValue_t		SDP_ShockValue;

/*
ShockCanMsg_data_log_t ShockCanMsgFront_log;
ShockCanMsg_data_log_t ShockCanMsgRear_log;
*/

CanCommunication_Message ShockCanMsg0;
CanCommunication_Message ShockCanMsg1;

/*
CanCommunication_Message ShockCanMsg0_log;
CanCommunication_Message ShockCanMsg1_log;
*/

void SDP_ShockValue_init(void);
void SDP_ShockValue_run_1ms(void);

void SDP_ShockValue_init(void){
	AdcSensor_Config config_adc;
	// SHOCK0
	config_adc.adcConfig.lpf.config.cutOffFrequency = 10000 / (2.0 * IFX_PI * 0.05); // FIXME: Adjust time constant
	config_adc.adcConfig.lpf.config.gain = 1;
	config_adc.adcConfig.lpf.config.samplingTime = 0.001;
	config_adc.adcConfig.lpf.activated = TRUE;

	config_adc.adcConfig.channelIn = &HLD_Vadc_P10_7_G3CH0_AD5;
	config_adc.tfConfig.a = 100.0f / (A0END - A0STT);
	config_adc.tfConfig.b = config_adc.tfConfig.a * (-A0STT);

	config_adc.isOvervoltageProtected = TRUE;

	AdcSensor_initSensor(&SHOCK0, &config_adc);
	HLD_AdcForceStart(SHOCK0.adcChannel.channel.group);

	// SHOCK1
	config_adc.adcConfig.channelIn = &HLD_Vadc_P33_10_G5CH4_DA0;
	config_adc.tfConfig.a = 100.0f / (A0END - A0STT);
	config_adc.tfConfig.b = config_adc.tfConfig.a * (-A0STT);

	config_adc.isOvervoltageProtected = TRUE;

	AdcSensor_initSensor(&SHOCK1, &config_adc);
	HLD_AdcForceStart(SHOCK1.adcChannel.channel.group);
}

IFX_STATIC void SDP_ShockValue_updatePPS_AN(SDP_ShockValue_sensor_t *data_out, AdcSensor *data_in)
{
	AdcSensor_getData(data_in);
	data_out->pedalPercent = data_out->config.reversed
			?(float32)100.0 - data_in->value : data_in->value;
}

IFX_STATIC void SDP_ShockValue_updatePPS(void)
{
	SDP_ShockValue_updatePPS_AN(&SDP_ShockValue_pps.shock0, &SHOCK0);
	SDP_ShockValue_updatePPS_AN(&SDP_ShockValue_pps.shock1, &SHOCK1);

	if(0.5 <= SDP_ShockValue_pps.shock0.pedalPercent && SDP_ShockValue_pps.shock0.pedalPercent <= 4.5)
	{
		SDP_ShockValue.shock0.pps = SDP_ShockValue_pps.shock0.pedalPercent;
		SDP_ShockValue.shock0.isValueOk = TRUE;
	}
	else
	{
		SDP_ShockValue.shock0.pps = 0;
		SDP_ShockValue.shock0.isValueOk = FALSE;
	}

	if(0.5 <= SDP_ShockValue_pps.shock1.pedalPercent && SDP_ShockValue_pps.shock1.pedalPercent <= 4.5)
	{
		SDP_ShockValue.shock1.pps = SDP_ShockValue_pps.shock1.pedalPercent;
		SDP_ShockValue.shock1.isValueOk = TRUE;
	}
	else
	{
		SDP_ShockValue.shock1.pps = 0;
		SDP_ShockValue.shock1.isValueOk = FALSE;
	}

}

void SDP_ShockValue_run_1ms(void){
	SDP_ShockValue_updatePPS();
}

/*
void SDP_ShockValue_log_init(void){
    // CAN message init 
	{
        CanCommunication_Message_Config config;
        config.messageId		=	SHOCK_CAN_MSG_0_log;
        config.frameType		=	IfxMultican_Frame_transmit;
        config.dataLen			=	IfxMultican_DataLengthCode_8;
        config.node				=	&CanCommunication_canNode0;
        CanCommunication_initMessage(&ShockCanMsg0_log, &config);
	}
	{
		CanCommunication_Message_Config config;
        config.messageId		=	SHOCK_CAN_MSG_1_log;
        config.frameType		=	IfxMultican_Frame_transmit;
        config.dataLen			=	IfxMultican_DataLengthCode_8;
        config.node				=	&CanCommunication_canNode0;
        CanCommunication_initMessage(&ShockCanMsg1_log, &config);
	}
}

void SDP_ShockValue_run_log(void){
    ShockCanMsgFront_log.S.AngleR = ShockCanMsgFront.S.AngleR;
    ShockCanMsgFront_log.S.AngleL = ShockCanMsgFront.S.AngleL;
    ShockCanMsgFront_log.S.Roll = ShockCanMsgFront.S.Roll;
    ShockCanMsgFront_log.S.Heave = ShockCanMsgFront.S.Heave;

    ShockCanMsgRear_log.S.AngleR = ShockCanMsgRear.S.AngleR;
    ShockCanMsgRear_log.S.AngleL = ShockCanMsgRear.S.AngleL;
    ShockCanMsgRear_log.S.Roll = ShockCanMsgRear.S.Roll;
    ShockCanMsgRear_log.S.Heave = ShockCanMsgRear.S.Heave;

    CanCommunication_setMessageData(ShockCanMsgRear_log.TransmitData[0], ShockCanMsgFront_log.TransmitData[1], &ShockCanMsg0_log);
    CanCommunication_setMessageData(ShockCanMsgRear_log.TransmitData[0], ShockCanMsgRear_log.TransmitData[1], &ShockCanMsg1_log);

    CanCommunication_transmitMessage(&ShockCanMsg0_log);
    CanCommunication_transmitMessage(&ShockCanMsg1_log);
}
*/
