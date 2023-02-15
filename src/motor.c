#include "motor.h"
#include <avr/io.h>
#include <stdbool.h>
#include <uart.h>

void dbMc_setSpeed(int8_t speed)
{
    if (speed == 0)
    {
        PORTL |= (MOTOR_LEFT_IN1A | MOTOR_LEFT_IN2A | MOTOR_RIGHT_IN1B | MOTOR_RIGHT_IN2B);
        OCR_LEFT = OCR_RIGHT = 0;
        return;
    }
    else if (speed > 0) // drive forwards
    {
        PORTL |= MOTOR_LEFT_IN2A;
        PORTL &= ~MOTOR_LEFT_IN1A;
        PORTL |= MOTOR_RIGHT_IN1B;
        PORTL &= ~MOTOR_RIGHT_IN2B;
    }
    else if (speed < 0) // drive backwards
    {
        PORTL &= ~MOTOR_LEFT_IN2A;
        PORTL |= MOTOR_LEFT_IN1A;
        PORTL &= ~MOTOR_RIGHT_IN1B;
        PORTL |= MOTOR_RIGHT_IN2B;
        speed = -speed;
    }

    // linear equation (=Geradengleichung)
    // OCR     = k * speed + d
    // k       = (MAX_OCR-MIN_OCR)/(100-1)
    // when speed = 1 -> OCR = MIN_OCR ==>
    // MIN_OCR = k * 1     + d
    // d       = MIN_OCR - k
    // OCR     = k * speed + MIN_OCR - k;
    // OCR     = k * (speed - 1) + MIN_OCR
    // OCR     = (MAX_OCR-MIN_OCR)/(100-1) * (speed-1) + MIN_OCR;

    // left wheel
    if (speed >= 100)
        OCR_LEFT = MAX_OCR - 1;
    else
        OCR_LEFT = (MAX_OCR - MIN_OCR) / (100 - 1) * (speed - 1) + MIN_OCR;

    // right wheel
    if (speed >= 100)
        OCR_RIGHT = MAX_OCR;
    else
        OCR_RIGHT = (MAX_OCR - MIN_OCR) / (100 - 1) * (speed - 1) + MIN_OCR;
}

void setInstruction(uint8_t tag, uint8_t data)
{
    if (tag == 0x00) // STOP
    {
        OCR_LEFT = OCR_RIGHT = 0;
    }
    else if (tag == 0x01) // LEFT
    {

        OCR_LEFT = 0;
        OCR_RIGHT = MAX_OCR;
        if (OCR_LEFT >= 40000)
        {
            OCR_LEFT = MAX_OCR;
        }
        else if (OCR_RIGHT < 6000)
        {
            OCR_RIGHT = 6000;
        }
        else
        {
            OCR_LEFT += 1000;
            OCR_RIGHT -= 1000;
        }
    }
    else if (tag == 0x02) // RIGHT
    {
        OCR_RIGHT = 0;
        OCR_LEFT = MAX_OCR;
        if (OCR_RIGHT >= 40000)
        {
            OCR_LEFT = MAX_OCR;
        }
        else if (OCR_LEFT < 6000)
        {
            OCR_RIGHT = 6000;
        }
        else
        {
            OCR_LEFT -= 1000;
            OCR_RIGHT += 1000;
        }
    }
    else if (tag == 0x03)
    {
        PORTL &= ~MOTOR_LEFT_IN2A;
        PORTL |= MOTOR_LEFT_IN1A;
        PORTL &= ~MOTOR_RIGHT_IN1B;
        PORTL |= MOTOR_RIGHT_IN2B;
        // backwards
        if (data >= 100)
            OCR_LEFT = MAX_OCR - 1;
        else
            OCR_LEFT = (MAX_OCR - MIN_OCR) / (100 - 1) * (data - 1) + MIN_OCR;

        // right wheel
        if (data >= 100)
            OCR_RIGHT = MAX_OCR;
        else
            OCR_RIGHT = (MAX_OCR - MIN_OCR) / (100 - 1) * (data - 1) + MIN_OCR;
    }
    else if (tag == 0x04)
    {
        PORTL |= MOTOR_LEFT_IN2A;
        PORTL &= ~MOTOR_LEFT_IN1A;
        PORTL |= MOTOR_RIGHT_IN1B;
        PORTL &= ~MOTOR_RIGHT_IN2B;
        // forward
        if (data >= 100)
            OCR_LEFT = MAX_OCR - 1;
        else
            OCR_LEFT = (MAX_OCR - MIN_OCR) / (100 - 1) * (data - 1) + MIN_OCR;

        // right wheel
        if (data >= 100)
            OCR_RIGHT = MAX_OCR;
        else
            OCR_RIGHT = (MAX_OCR - MIN_OCR) / (100 - 1) * (data - 1) + MIN_OCR;
    }
}

void dbMc_init()
{
    // the pins to control the h-bridge must be outputs
    DDRL |= (MOTOR_LEFT_ENA | MOTOR_LEFT_IN1A | MOTOR_LEFT_IN2A |
             MOTOR_RIGHT_ENB | MOTOR_RIGHT_IN1B | MOTOR_RIGHT_IN2B);

    TCCR5A = (1 << WGM51); // fast PWM mode
    TCCR5B = ((1 << WGM53) | (1 << WGM52));

    TIMSK5 = 0;               // disable all interrupts
    OCR_LEFT = OCR_RIGHT = 0; // stop the motors

    // 62.5ns * 320.000 = 20ms
    // 62.5ns * 8(PS) * 40000(ICR) = 20ms
    ICR5 = MAX_OCR - 1;

    // to let the hardware create the PWM signal at OC5A and OC5B
    TCCR5A |= ((1 << COM5A1) | (1 << COM5B1));
    TCCR5B |= (1 << CS51); // start with PS=8

    OCR_LEFT = OCR_RIGHT = MAX_OCR; // set the OCR Value max
}
