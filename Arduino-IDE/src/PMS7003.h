#ifndef PMS7003_H
#define PMS7003_H
#endif

#define PMS7003_I2C_ADDR 0x42
#define START_1 0x42  
#define START_2 0x4d  
#define DATA_LENGTH_H        0  
#define DATA_LENGTH_L        1  
#define PM1_0_ATMOSPHERE_H   8  
#define PM1_0_ATMOSPHERE_L   9  
#define PM2_5_ATMOSPHERE_H   10  
#define PM2_5_ATMOSPHERE_L   11  
#define PM10_ATMOSPHERE_H    12  
#define PM10_ATMOSPHERE_L    13  
#define PM2_5_PARTICLE_H   16  
#define PM2_5_PARTICLE_L   17  
#define VERSION              26  
#define ERROR_CODE           27  
#define CHECKSUM             29  

unsigned int GetPM_Data(unsigned char chrSrc[], byte bytHigh, byte bytLow);
void read_PMS7003(unsigned int data[]);
void main_PMS7003();

/*
#ifndef    MAX_FRAME_LEN
#define    MAX_FRAME_LEN   64
#endif

struct PMS7003_framestruct {
    byte  frameHeader[2];
    unsigned int  frameLen = MAX_FRAME_LEN;
    unsigned int  concPM1_0_CF1;
    unsigned int  concPM2_5_CF1;
    unsigned int  concPM10_0_CF1;
    unsigned int  checksum;
} PMS7003S;

bool pms7003_read() {
    bool packetReceived = false;
    unsigned int calcChecksum = 0;
    bool inFrame = false;
    int detectOff = 0;	
    char frameBuf[MAX_FRAME_LEN];
    int frameLen = MAX_FRAME_LEN;

    while (!packetReceived) {
        if (Serial.available() > 32) {
            int drain = Serial.available();
            for (int i = drain; i > 0; i--) {
                Serial.read();
            }
        }
        if (Serial.available() > 0) {
            int incomingByte = Serial.read();
            if (!inFrame) {
                if (incomingByte == 0x42 && detectOff == 0) {
                    frameBuf[detectOff] = incomingByte;
                    PMS7003S.frameHeader[0] = incomingByte;
                    calcChecksum = incomingByte; // Checksum init!
                    detectOff++;
                }
                else if (incomingByte == 0x4D && detectOff == 1) {
                    frameBuf[detectOff] = incomingByte;
                    PMS7003S.frameHeader[1] = incomingByte;
                    calcChecksum += incomingByte;
                    inFrame = true;
                    detectOff++;
                }
                else {

                }
            }
            else {
                frameBuf[detectOff] = incomingByte;
                calcChecksum += incomingByte;
                detectOff++;
                unsigned int val = (frameBuf[detectOff-1]&0xff)+(frameBuf[detectOff-2]<<8);
                switch (detectOff) {
                    case 4:
                        PMS7003S.frameLen = val;
                        frameLen = val + detectOff;
                        break;
                    case 6:
                        PMS7003S.concPM1_0_CF1 = val;
                        break;
                    case 8:
                        PMS7003S.concPM2_5_CF1 = val;
                        break;
                    case 10:
                        PMS7003S.concPM10_0_CF1 = val;
                        break;
                    case 32:
                        PMS7003S.checksum = val;
                        calcChecksum -= ((val>>8)+(val&0xFF));
                        break;
                    default:
                        break;
                }

                if (detectOff >= frameLen) {
                    packetReceived = true;
                    detectOff = 0;
                    inFrame = false;
                }
            }
        }
    }
    return (calcChecksum == PMS7003S.checksum);
}
*/