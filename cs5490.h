#ifndef _CS5490_H
#define _CS5490_H

#include "mbed.h"

// typedef enum {
//   READ_CMD,
//   WRITE_CMD,
//   PAGE_SELECT_CMD,
//   INSTRUCTION_CMD,
//   NONE_CMD
// } last_cmd_t;

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

static const uint8_t BUFFER_SIZE = 3;

class CS5490 {
  public:
    CS5490(PinName tx, PinName rx, PinName reset, PinName digitalInOut);

    void init();

    /* INSTRUCTIONS */

    /* CALIBRATION */

    /* MEASUREMENTS */
    float getPower();

  protected:
    uint8_t data[BUFFER_SIZE];
    RawSerial _uart;
    DigitalOut _reset;
    DigitalInOut _digitalInOut;
    bool readMessage();
    bool readRegister(CsRegister_t);
    void writeRegister(CsRegister_t, uint8_t);
    void sendInstruction(uint8_t);
    void printMessage();
    float toFloat(uint8_t, uint8_t);
};

#endif