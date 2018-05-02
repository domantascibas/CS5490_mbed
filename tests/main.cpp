#include "mbed.h"

DigitalOut led(LED1);

int main(void) {
  led = 0;

  while(1) {
    led = !led;
    wait(0.5);
  }
  return 0;
}