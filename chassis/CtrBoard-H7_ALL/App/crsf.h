#ifndef __CRSF_H__
#define __CRSF_H__

#include <stdint.h>

#define CRSF_MAX_CHANNEL  16
#define CRSF_FRAME_SIZE   26 // Typical channel frame

extern uint16_t rc_channels[CRSF_MAX_CHANNEL];

void CRSF_ParseByte(uint8_t byte);

#endif
