#include <avr/io.h>
#include "motor.h"

void uart0_registerReceptionCallback(void (*pCallback)(uint8_t data));
