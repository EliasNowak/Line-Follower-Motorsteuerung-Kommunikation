#include "motor.h"
#include <avr/io.h>
#include <stdbool.h>
#include <uart.h>

void dbMc_setSpeed(int8_t speed)
{
    if (speed == 0)
    {
        PORTA |= (MOTOR_LEFT_IN1A | MOTOR_LEFT_IN2A | MOTOR_RIGHT_IN1B | MOTOR_RIGHT_IN2B);
        OCR_LEFT = OCR_RIGHT = 0;
        return;
    }
    else if (speed > 0) // drive forwards
    {
        PORTA &= ~MOTOR_LEFT_IN2A;
        PORTA |= MOTOR_LEFT_IN1A;
        PORTA &= ~MOTOR_RIGHT_IN1B;
        PORTA |= MOTOR_RIGHT_IN2B;
    }
    else if (speed < 0) // drive backwards
    {
        PORTA |= MOTOR_LEFT_IN2A;
        PORTA &= ~MOTOR_LEFT_IN1A;
        PORTA |= MOTOR_RIGHT_IN1B;
        PORTA &= ~MOTOR_RIGHT_IN2B;
        speed = -speed;
    }
    // left wheel
    if (speed >= 100)
        OCR_LEFT = MAX_OCR - 1;
    else
        OCR_LEFT = (MAX_OCR - MIN_OCR) / (100 - 1) * (speed - 1) + MIN_OCR;

    // right wheel
    if (speed >= 100)
        OCR_RIGHT = MAX_OCR - 1;
    else
        OCR_RIGHT = (MAX_OCR - MIN_OCR) / (100 - 1) * (speed - 1) + MIN_OCR;
}

void setInstruction(uint8_t tag, uint8_t dataLeft, uint8_t dataRight)
{
    if (tag == 0x00) // STOP
    {
        OCR_LEFT = OCR_RIGHT = 0;
    }
    else if (tag == 0x01) // DIR
    {
        PORTA &= ~MOTOR_LEFT_IN2A;
        PORTA |= MOTOR_LEFT_IN1A;
        PORTA &= ~MOTOR_RIGHT_IN1B;
        PORTA |= MOTOR_RIGHT_IN2B;
        // forward
        if (dataLeft >= 100)
            OCR_LEFT = MAX_OCR - 1;
        else
            OCR_LEFT = (MAX_OCR - MIN_OCR) / (100 - 1) * (dataLeft - 1) + MIN_OCR;

        // right wheel
        if (dataRight >= 100)
            OCR_RIGHT = MAX_OCR - 1;
        else
            OCR_RIGHT = (MAX_OCR - MIN_OCR) / (100 - 1) * (dataRight - 1) + MIN_OCR;
    }
    else if (tag == 0x02) // BACK
    {
        PORTA |= MOTOR_LEFT_IN2A;
        PORTA &= ~MOTOR_LEFT_IN1A;
        PORTA |= MOTOR_RIGHT_IN1B;
        PORTA &= ~MOTOR_RIGHT_IN2B;
        // backwards
        if (dataLeft >= 100)
            OCR_LEFT = MAX_OCR - 1;
        else
            OCR_LEFT = (MAX_OCR - MIN_OCR) / (100 - 1) * (dataLeft - 1) + MIN_OCR;

        // right wheel
        if (dataRight >= 100)
            OCR_RIGHT = MAX_OCR - 1;
        else
            OCR_RIGHT = (MAX_OCR - MIN_OCR) / (100 - 1) * (dataRight - 1) + MIN_OCR;
    }
    else if (tag == 0x03) // FORWARD
    {
        PORTA &= ~MOTOR_LEFT_IN2A;
        PORTA |= MOTOR_LEFT_IN1A;
        PORTA &= ~MOTOR_RIGHT_IN1B;
        PORTA |= MOTOR_RIGHT_IN2B;
        // forward
        if (dataLeft >= 100)
            OCR_LEFT = MAX_OCR - 1;
        else
            OCR_LEFT = (MAX_OCR - MIN_OCR) / (100 - 1) * (dataLeft - 1) + MIN_OCR;

        // right wheel
        if (dataRight >= 100)
            OCR_RIGHT = MAX_OCR - 1;
        else
            OCR_RIGHT = (MAX_OCR - MIN_OCR) / (100 - 1) * (dataRight - 1) + MIN_OCR;
    }
}

void dbMc_init()
{
    // the pins to control the h-bridge must be outputs
    DDRA |= (MOTOR_LEFT_IN1A | MOTOR_LEFT_IN2A |
             MOTOR_RIGHT_IN1B | MOTOR_RIGHT_IN2B);

    DDRB |= (MOTOR_LEFT_ENA | MOTOR_RIGHT_ENB);

    TCCR1A = (1 << WGM11); // fast PWM mode
    TCCR1B = ((1 << WGM13) | (1 << WGM12));

    TIMSK1 = 0;               // disable all interrupts
    OCR_LEFT = OCR_RIGHT = 0; // stop the motors

    // 62.5ns * 320.000 = 20ms
    // 62.5ns * 8(PS) * 40000(ICR) = 20ms
    ICR1 = MAX_OCR - 1;

    // to let the hardware create the PWM signal at OC1A and OC1C
    TCCR1A |= ((1 << COM1A1) | (1 << COM1C1));
    TCCR1B |= (1 << CS11); // start with PS=8
}
