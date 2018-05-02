#ifndef _CS5490_H
#define _CS5490_H

#include "mbed.h"

class CS5490 {
  public:
    CS5490(PinName CS_TX, PinName CS_RX, PinName CS_RST, PinName CS_DIO);

    void initialize();
    void write();

    /* INSTRUCTIONS */
    void reset();
    void standby();
    void wakeup();

    /* CALIBRATION */

    /* MEASURE */
    

  protected:
    void read();
};

#endif