#ifndef _CS5490_H
#define _CS5490_H

#include "mbed.h"

typedef enum {
  DISABLED,
  ENABLED
} prot_t;

typedef struct
{
  uint8_t page;
  uint8_t address;
  prot_t checksum;
  uint32_t default_val;
} CsRegister_t;

static const uint8_t BUF_LENGTH = 3;

class CS5490 {
  public:
    CS5490(PinName tx, PinName rx, PinName reset, PinName digitalInOut);

    void init();

    /* INSTRUCTIONS */

    /* CALIBRATION */

    /* MEASUREMENTS */
    void getReg();
    static void simpleCallback(void* object, uint8_t value);
    void testCallback(void (*cb)(void *, uint8_t), void *obj);
    void printMessage(uint8_t);

  protected:
    Serial _uart;
    DigitalOut _reset;
    DigitalInOut _digitalInOut;
    void onMessageReceived();
    void onByteReceived();
    void readByte(uint8_t);
    void readRegister(CsRegister_t);
    void writeRegister(CsRegister_t, uint8_t);
    void sendInstruction(uint8_t);
    void selectPage(uint8_t);
    uint8_t buf_pointer;
    uint8_t data[BUF_LENGTH];
};

#endif