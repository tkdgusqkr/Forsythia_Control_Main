#include "HLD.h"
#include <Port/Io/IfxPort_Io.h>
#include "Configuration.h"


IFX_EXTERN void SDP_DashBoardLed_init();
IFX_EXTERN void SDP_TEMP1_LED_ON();
IFX_EXTERN void SDP_TEMP1_LED_OFF();
IFX_EXTERN void SDP_TEMP2_LED_ON();
IFX_EXTERN void SDP_TEMP2_LED_OFF();
IFX_EXTERN void SDP_RTD_LED_ON();
IFX_EXTERN void SDP_RTD_LED_OFF();
IFX_EXTERN void SDP_SDC_LED_ON();
IFX_EXTERN void SDP_SDC_LED_OFF();
IFX_EXTERN void SDP_ECU_LED_ON();
IFX_EXTERN void SDP_ECU_LED_OFF();
IFX_EXTERN void SDP_BSPD_LED_ON();
IFX_EXTERN void SDP_BSPD_LED_OFF();
IFX_EXTERN void SDP_IMD_LED_ON();
IFX_EXTERN void SDP_IMD_LED_OFF();
IFX_EXTERN void SDP_AMS_LED_ON();
IFX_EXTERN void SDP_AMS_LED_OFF();

typedef struct{
    Ifx_P module;
    uint8 port;
    boolean val;
}indicator_t;

typedef struct{
    indicator_t TEMP1;
    indicator_t TEMP2;
    indicator_t RTD;
    indicator_t SDC;
    indicator_t ECU;
    indicator_t BSPD;
    indicator_t IMD;
    indicator_t AMS;
}DashBoardLed_t;

IFX_EXTERN DashBoardLed_t DashBoardLed;