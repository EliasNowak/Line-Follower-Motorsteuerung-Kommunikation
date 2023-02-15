#include <avr/io.h>
#include <stdbool.h>

#define MOTOR_LEFT_ENA (1 << 3)  // PinL.3
#define MOTOR_RIGHT_ENB (1 << 4) // PinL.4
//-----------------------------------
#define MOTOR_LEFT_IN1A (1 << 0)  // PinL.5
#define MOTOR_LEFT_IN2A (1 << 5)  // PinL.1
#define MOTOR_RIGHT_IN1B (1 << 2) // PinL.0
#define MOTOR_RIGHT_IN2B (1 << 1) // PinL.2

#define MAX_OCR 40000
#define MIN_OCR 5000 // determined by experiment

#define OCR_LEFT OCR5A
#define OCR_RIGHT OCR5B

void dbMc_setSpeed(int8_t speed);

//----------------------------------------
/// @brief  increase and decrease OCR V
/// @param[in] lor left or rigt wheel
/// @param[in]  data data will be multiplyed to your
/// @param[in]  stepszie multiplication factor
//----------------------------------------
void setInstruction(uint8_t tag, uint8_t data);

void dbMc_init();