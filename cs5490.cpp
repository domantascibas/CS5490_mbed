#include "mbed.h"
#include "cs5490.h"

static const uint16_t CS_SERIAL_BAUD = 600;

/* COMMANDS */
static const uint8_t COMMAND_READ_REGISTER = 0x00;
static const uint8_t COMMAND_WRITE_REGISTER = 0x40;
static const uint8_t COMMAND_PAGE_SELECT = 0x80;
static const uint8_t COMMAND_INSTRUCTION = 0xC0;

/* CONTROLS */
static const uint8_t CONTROLS_SOFTWARE_RESET = 0x01;
static const uint8_t CONTROLS_STANDBY = 0x02;
static const uint8_t CONTROLS_WAKEUP = 0x03;
static const uint8_t CONTROLS_SINGLE_CONVERSION = 0x14;
static const uint8_t CONTROLS_CONTINUOUS_CONVERSION = 0x15;
static const uint8_t CONTROLS_HALT_CONVERSION = 0x18;

/* CALIBRATION */
static const uint8_t CALIBRATION_DC_OFFSET = 0x20;
static const uint8_t CALIBRATION_AC_OFFSET = 0x30;
static const uint8_t CALIBRATION_GAIN = 0x38;
static const uint8_t CALIBRATION_I = 0x21;
static const uint8_t CALIBRATION_V = 0x22;
static const uint8_t CALIBRATION_V_AND_I = 0x26;

/* REGISTERS */
static const CsRegister_t CONFIGURATION_0 = {0, 0, ENABLED, 0xC02000};
static const CsRegister_t CONFIGURATION_1 = {0, 1, ENABLED, 0x00EEEE};
static const CsRegister_t INTERRUPT_MASK = {0, 3, ENABLED, 0x000000};
static const CsRegister_t PHASE_COMP_CONTROL = {0, 5, ENABLED, 0x000000};
static const CsRegister_t UART_CONTROL = {0, 7, ENABLED, 0x02004D};
static const CsRegister_t ENERGY_PULSE_WIDTH = {0, 8, ENABLED, 0x000001};
static const CsRegister_t ENERGY_PULSE_CONTROL = {0, 9, ENABLED, 0x000000};
static const CsRegister_t INTERRUPT_STATUS = {0, 23, DISABLED, 0x800000};
static const CsRegister_t CHIP_STATUS_1 = {0, 24, DISABLED, 0x801800};
static const CsRegister_t CHIP_STATUS_2 = {0, 25, DISABLED, 0x000000};
static const CsRegister_t REGISTER_LOCK_CONTROL = {0, 34, ENABLED, 0x000000};
static const CsRegister_t PEAK_VOLTAGE = {0, 36, DISABLED, 0x000000};
static const CsRegister_t PEAK_CURRENT = {0, 37, DISABLED, 0x000000};
static const CsRegister_t PHASE_SEQ_DETECTION = {0, 48, DISABLED, 0x000000};
static const CsRegister_t ZERO_CROSS_NUM = {0, 55, DISABLED, 0x000064};
// page 16
static const CsRegister_t CONFIGURATION_2 = {16, 0, ENABLED, 0x100200};
static const CsRegister_t REGISTER_CHECKSUM = {16, 1, DISABLED, 0x000000};
static const CsRegister_t INSTANT_CURRENT = {16, 2, DISABLED, 0x000000};
static const CsRegister_t INSTANT_VOLTAGE = {16, 3, DISABLED, 0x000000};
static const CsRegister_t INSTANT_ACTIVE_POWER = {16, 4, DISABLED, 0x000000};
static const CsRegister_t AVERAGE_ACTIVE_POWER = {16, 5, DISABLED, 0x000000};
static const CsRegister_t RMS_CURRENT = {16, 6, DISABLED, 0x000000};
static const CsRegister_t RMS_VOLTAGE = {16, 7, DISABLED, 0x000000};
static const CsRegister_t AVERAGE_REACTIVE_POWER = {16, 14, DISABLED, 0x000000};
static const CsRegister_t INSTANT_REACTIVE_POWER = {16, 15, DISABLED, 0x000000};
static const CsRegister_t INSTANT_APPARENT_POWER = {16, 20, DISABLED, 0x000000};
static const CsRegister_t POWER_FACTOR = {16, 21, DISABLED, 0x000000};
static const CsRegister_t TEMPERATURE = {16, 27, DISABLED, 0x000000};
static const CsRegister_t TOTAL_ACTIVE_POWER = {16, 29, DISABLED, 0x000000};
static const CsRegister_t TOTAL_APPARENT_POWER = {16, 30, DISABLED, 0x000000};
static const CsRegister_t TOTAL_REACTIVE_POWER = {16, 31, DISABLED, 0x000000};
static const CsRegister_t DC_CURRENT_OFFSET = {16, 32, ENABLED, 0x000000};
static const CsRegister_t DC_CURRENT_GAIN = {16, 33, ENABLED, 0x400000};
static const CsRegister_t DC_VOLTAGE_OFFSET = {16, 34, ENABLED, 0x000000};
static const CsRegister_t DC_VOLTAGE_GAIN = {16, 35, ENABLED, 0x400000};
static const CsRegister_t INSTANT_POWER_OFFSET = {16, 36, ENABLED, 0x000000};
static const CsRegister_t AC_CURRENT_OFFSET = {16, 37, ENABLED, 0x000000};
static const CsRegister_t LINE_TO_SAMPLE_FREQ = {16, 49, DISABLED, 0x01999A};
static const CsRegister_t SAMPLE_COUNT = {16, 51, DISABLED, 0x000FA0};
static const CsRegister_t TEMPERATURE_GAIN = {16, 54, ENABLED, 0x06B716};
static const CsRegister_t TEMPERATURE_OFFSET = {16, 55, ENABLED, 0xD53998};
static const CsRegister_t FILTER_SETTLE_TIME = {16, 57, DISABLED, 0x00001E};
static const CsRegister_t NO_LOAD_THRESHOLD = {16, 58, ENABLED, 0x000000};
static const CsRegister_t SYSTEM_GAIN = {16, 60, ENABLED, 0x500000};
static const CsRegister_t SYSTEM_TIME = {16, 61, DISABLED, 0x000000};
// page 17
static const CsRegister_t VOLTAGE_SAG_DURATION = {17, 0, ENABLED, 0x000000};
static const CsRegister_t VOLTAGE_SAG_LEVEL = {17, 1, ENABLED, 0x000000};
static const CsRegister_t OVERCURRENT_DURATION = {17, 4, ENABLED, 0x000000};
static const CsRegister_t OVERCURRENT_LEVEL = {17, 5, ENABLED, 0x7FFFFF};
// page 18
static const CsRegister_t I_CHANNEL_Z_CROSS_THRESHOLD = {18, 24, ENABLED, 0x100000};
static const CsRegister_t ENERGY_PULSE_RATE = {18, 28, ENABLED, 0x800000};
static const CsRegister_t ROGOWSKI_COIL_INTEGRATOR_GAIN = {18, 43, ENABLED, 0x143958};
static const CsRegister_t VOLTAGE_SWELL_DURATION = {18, 46, ENABLED, 0x000000};
static const CsRegister_t VOLTAGE_SWELL_LEVEL = {18, 47, ENABLED, 0x7FFFFF};
static const CsRegister_t V_CHANNEL_Z_CROSS_THRESHOLD = {18, 58, ENABLED, 0x100000};
static const CsRegister_t LINE_CYCLE_COUNT = {18, 62, DISABLED, 0x000064};
static const CsRegister_t I_CHANNEL_GAIN_CALIBRATION_SCALE = {18, 63, ENABLED, 0x4CCCCC};

CS5490::CS5490(PinName tx, PinName rx, PinName reset, PinName digitalInOut) :
  _uart(tx, rx, CS_SERIAL_BAUD), _reset(reset), _digitalInOut(digitalInOut) {
}

void CS5490::init() {
  _reset = 0;
  wait(0.5);
  _reset = 1;

  wait(0.5);
  sendInstruction(CONTROLS_WAKEUP);
  wait(0.5);
  sendInstruction(CONTROLS_CONTINUOUS_CONVERSION);
}

float CS5490::getPower() {
  float result;
  if(readRegister(AVERAGE_ACTIVE_POWER)) {
    printf("0x%02X%02X%02X\r\n", data[0], data[1], data[2]);
    result = toFloat(0,0);
    return result;
  }
  return 0.0;
}

bool CS5490::readMessage() {
  // TODO: set timeout, and return false if timeout
  uint8_t i = 0;
  for(i=0; i<sizeof(data); i++) {
    data[i] = _uart.getc();
    // printf("[%d] 0x%02X\r\n", i, data[i]);
  }
  return true;
}

bool CS5490::readRegister(CsRegister_t reg) {
  // printf("[read] 0x%X 0x%X\r\n", COMMAND_PAGE_SELECT | reg.page, COMMAND_READ_REGISTER | reg.address);
  _uart.putc(COMMAND_PAGE_SELECT | reg.page);
  _uart.putc(COMMAND_READ_REGISTER | reg.address);
  if (readMessage()) return true;
  return false;
}

void CS5490::sendInstruction(uint8_t instruction) {
  printf("[instruction] 0x%X\r\n", COMMAND_INSTRUCTION | instruction);
  _uart.putc(COMMAND_INSTRUCTION | instruction);
}

void CS5490::writeRegister(CsRegister_t reg, uint8_t data) {
}

float CS5490::toFloat(uint8_t lsbPower, uint8_t msbOption) {
  uint8_t i = 0;
  uint8_t msb_mask = 0x80;
  uint32_t buffer_mask = 0xFF000000;
  uint32_t buffer = 0;
  bool msb;
  float output;

  for(i=0; i<sizeof(data); i++) {
    buffer |= data[i] << ((sizeof(data)-1) - i) * 8;
    // printf("[%d 0x%02X] 0x%lX\r\n", ((sizeof(data) - 1) - i), data[i] << ((sizeof(data) - 1) - i) * 8, buffer);
  }

  // TODO: add cases for msbUnsigned, and msbNull
  // TODO: add lsbPower variable
  msb = data[0] & msb_mask;
  // printf("0x%lX\r\n", buffer);
  // output = (float)buffer;
  // output /= (pow(2, 24));

  if(msb) {
    buffer = ~buffer;
    printf("0x%lX\r\n", buffer);
    buffer &= ~(buffer_mask);
    printf("0x%lX\r\n", buffer);
    output = (float)buffer + 1.0;
    printf("%.2f\r\n", output);
    output /= -pow(2, 23);
  } else {
    output = (float)buffer;
    output /= (pow(2, 23)-1.0);
  }
  printf("%f voltage\r\n", output);
  return output;
}