#include <avr/io.h>
#include <stdbool.h>

#define MOTOR_LEFT_ENA (1 << 5)  // PinB.5
#define MOTOR_RIGHT_ENB (1 << 7) // PinB.7
//-----------------------------------
#define MOTOR_LEFT_IN1A (1 << 1)  // PinA.1
#define MOTOR_LEFT_IN2A (1 << 3)  // PinA.3
#define MOTOR_RIGHT_IN1B (1 << 5) // PinA.5
#define MOTOR_RIGHT_IN2B (1 << 7) // PinA.7

#define MAX_OCR 40000
#define MIN_OCR 5000 // determined by experiment

#define OCR_LEFT OCR1A
#define OCR_RIGHT OCR1C

void dbMc_setSpeed(int8_t speed);

//----------------------------------------
/// @brief  increase and decrease OCR V
/// @param[in] tag left or rigt wheel
/// @param[in]  dataLeft v Left
/// @param[in]  dataRight v Right
//----------------------------------------
void setInstruction(uint8_t tag, uint8_t dataLeft, uint8_t dataRight);

void dbMc_init();