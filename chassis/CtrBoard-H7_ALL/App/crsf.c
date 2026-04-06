#include "crsf.h"

uint16_t rc_channels[CRSF_MAX_CHANNEL];

// 简化的包解析状态机
void CRSF_ParseByte(uint8_t byte) {
    static uint8_t state = 0;
    static uint8_t frame_len = 0;
    static uint8_t frame_type = 0;
    static uint8_t buffer[64];
    static uint8_t p = 0;

    switch(state) {
        case 0: // Sync
             if (byte == 0xC8 || byte == 0xEE || byte == 0xEA) {
                 buffer[0] = byte;
                 p = 1;
                 state = 1;
             }
             break;
        case 1: // Length
             frame_len = byte;
             buffer[p++] = byte;
             if (frame_len > 62) { state = 0; break; }
             state = 2;
             break;
        case 2: // Type
             frame_type = byte;
             buffer[p++] = byte;
             state = 3;
             break;
        case 3: // Payload and CRC
             buffer[p++] = byte;
             if (p == frame_len + 2) {
                 // Validate CRC8 in real implementation
                 if (frame_type == 0x16) { // RC Channels packed
                     // 11 bits per channel
                     rc_channels[0] = (uint16_t)((buffer[3] | buffer[4] << 8) & 0x07FF);
                     rc_channels[1] = (uint16_t)((buffer[4] >> 3 | buffer[5] << 5) & 0x07FF);
                     rc_channels[2] = (uint16_t)((buffer[5] >> 6 | buffer[6] << 2 | buffer[7] << 10) & 0x07FF);
                     rc_channels[3] = (uint16_t)((buffer[7] >> 1 | buffer[8] << 7) & 0x07FF);
                     rc_channels[4] = (uint16_t)((buffer[8] >> 4 | buffer[9] << 4) & 0x07FF);
                     rc_channels[5] = (uint16_t)((buffer[9] >> 7 | buffer[10] << 1 | buffer[11] << 9) & 0x07FF);
                     rc_channels[6] = (uint16_t)((buffer[11] >> 2 | buffer[12] << 6) & 0x07FF);
                     rc_channels[7] = (uint16_t)((buffer[12] >> 5 | buffer[13] << 3) & 0x07FF);
                     rc_channels[8] = (uint16_t)((buffer[14] | buffer[15] << 8) & 0x07FF);
                     rc_channels[9] = (uint16_t)((buffer[15] >> 3 | buffer[16] << 5) & 0x07FF);
                     rc_channels[10] = (uint16_t)((buffer[16] >> 6 | buffer[17] << 2 | buffer[18] << 10) & 0x07FF);
                     rc_channels[11] = (uint16_t)((buffer[18] >> 1 | buffer[19] << 7) & 0x07FF);
                     rc_channels[12] = (uint16_t)((buffer[19] >> 4 | buffer[20] << 4) & 0x07FF);
                     rc_channels[13] = (uint16_t)((buffer[20] >> 7 | buffer[21] << 1 | buffer[22] << 9) & 0x07FF);
                     rc_channels[14] = (uint16_t)((buffer[22] >> 2 | buffer[23] << 6) & 0x07FF);
                     rc_channels[15] = (uint16_t)((buffer[23] >> 5 | buffer[24] << 3) & 0x07FF);
                 }
                 state = 0;
             }
             break;
    }
}
