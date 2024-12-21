/*
 * ShockValue.h
 *
 *  Created on: 2021. 5. 13.
 *      Author: Suprhimp
 */

#ifndef SRC_APPSW_TRICORE_SDP_SHOCKVALUE_SHOCKVALUE_H_
#define SRC_APPSW_TRICORE_SDP_SHOCKVALUE_SHOCKVALUE_H_

/******************************************************************************/
/*----------------------------------Includes----------------------------------*/
/******************************************************************************/
#include "SDP.h"
#include "CanCommunication.h"
#include "HLD.h"

typedef union
{
	struct{
		unsigned int AngleR : 16;
		unsigned int AngleL : 16;
		unsigned int Roll : 16;
		unsigned int Heave : 16;
    }S;
    uint32 RecievedData[2];
}ShockCanMsg_data_t;

typedef struct{
    float frontRoll;
    float frontHeave;
    float rearRoll;
    float rearHeave;
}ShockValue_t;

typedef struct
{
	float32 start;
	float32 end;
	float32 deadzone;
	boolean reversed;

	float32 center;
	float32 opposite;
	float32 len;


	float32 dzstart;
	float32 dzend;
	float32 dzlen;

}SDP_ShockValue_sensorConfig_t;

typedef struct
{
	SDP_ShockValue_sensorConfig_t	config;
	float32						pedalPercent;
	boolean						isValueOk;
}SDP_ShockValue_sensor_t;

typedef struct
{
	SDP_ShockValue_sensor_t		shock0;
	SDP_ShockValue_sensor_t		shock1;
}SDP_ShockValue_pps_t;

typedef struct
{
	float32 pps;
	boolean isValueOk;
}SDP_ShockValue_struct_t;

typedef struct
{
	SDP_ShockValue_struct_t			shock0;
	SDP_ShockValue_struct_t			shock1;
}SDP_ShockValue_t;

/*
typedef union
{
	uint32 TransmitData[2];
		struct{
			unsigned int AngleR : 16;
			unsigned int AngleL : 16;
			unsigned int Roll : 16;
			unsigned int Heave : 16;
		}S;
}ShockCanMsg_data_log_t;
*/

IFX_EXTERN SDP_ShockValue_t		SDP_ShockValue;
IFX_EXTERN ShockCanMsg_data_t ShockCanMsgFront;
IFX_EXTERN ShockCanMsg_data_t ShockCanMsgRear;

IFX_EXTERN AdcSensor SHOCK0;
IFX_EXTERN AdcSensor SHOCK1;

// IFX_EXTERN ShockCanMsg_data_log_t ShockCanMsgFront_log;
// IFX_EXTERN ShockCanMsg_data_log_t ShockCanMsgRear_log;


IFX_EXTERN void SDP_ShockValue_init(void);
IFX_EXTERN void SDP_ShockValue_run_1ms(void);

#endif /* SHOCKVALUE_H */
