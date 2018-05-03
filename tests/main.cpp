#include "mbed.h"
#include "cs5490.h"

DigitalOut led(LED1);
CS5490 cs5490(PA_9, PA_10, PA_11, PA_12);
Serial pc(USBTX, USBRX, 115200);
Ticker vTick;

void init(void) {
  led = 0;
  printf("\r\nloading...\r\n");
}

// void readV(void) {
// }

int main(void) {
  init();

  cs5490.init();
  while(1) {
    led = !led;
    wait(0.5);
    cs5490.read();
  }
  return 0;
}