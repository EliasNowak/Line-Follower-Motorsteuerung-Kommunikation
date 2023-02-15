
#include <avr/io.h>
#include <avr/interrupt.h>
#include <uart.h>
#include <stddef.h>
#include "motor.h"

// command set                 TAG   LENGTH   VALUES
#define STOP 0x00    // 0x00  0x01 0x01
#define LEFT 0x01    // 0x01  0x01     <number of the speed to turn on>
#define RIGHT 0x02   // 0x02  0x01     <number of the speed to turn on>
#define BACK 0x03    // 0x03  0x01     <number of the speed to turn off>
#define FORWARD 0x04 // 0x03  0x01     <number of the speed to turn off>
// function pointer to the function to be called once data are received
void (*pReceptionCallback)(uint8_t data) = NULL;

void uart0_registerReceptionCallback(void (*pCallback)(uint8_t data))
{
  // uart0_init(9600, UART_M_TRANSCEIVE, UART_P_NONE);
  pReceptionCallback = pCallback; // remember the function to be called

  if (pCallback != NULL)
  {
    UCSR0B |= (1 << RXCIE0); // enable reception interrupt
  }
  else
  {
    UCSR0B &= ~(1 << RXCIE0); // disable reception interrupt
  }
}

// the interrupt routine is called whenever data are received
ISR(USART0_RX_vect)
{
  //   uart0_putc(UDR0);
  //   check if the function pointer points to a valid function
  if (pReceptionCallback != NULL)
  {
    (*pReceptionCallback)(UDR0);
  }
}

// gets called whenever a complete frame was received
void frameReceived(uint8_t tag, uint8_t len, uint8_t value)
{
  switch (tag) // execute the depending on the tag
  {
  case STOP: // if the command CMD_LEDS_ON was received
  {
    uart0_puts("STOP");
    setInstruction(STOP, 0);
    break;
  }
  case LEFT:
  {
    uart0_puts("LEFT");
    setInstruction(LEFT, value);
    break;
  }
  case RIGHT:
  {
    uart0_puts("RIGHT");
    setInstruction(RIGHT, value);
    break;
  }
  case BACK:
  {
    uart0_puts("BACK");
    setInstruction(BACK, value);
    break;
  }
  case FORWARD:
  {
    uart0_puts("FORWARD");
    setInstruction(FORWARD, value);
    break;
  }
  }
}

void byteReceived(uint8_t data) // gets called whenever UARD0 received a byte
{
  static uint8_t receptionBuffer[3] = {0x01, 0x01, 0x01}; // buffer to store the received bytes
  static uint16_t bytesReceived = 0;                      // counts how many bytes were received

  receptionBuffer[bytesReceived] = data; // copy the received by into the reception buffer
  bytesReceived++;                       // and increase the bytes received counter

  // if the transmittet len of the TLV Frame isn't 0x01,
  // it means something went wrong, so stop the roboter and return ERROR
  if (receptionBuffer[1] != 0x01)
  {
    setInstruction(STOP, 0);
    uart0_puts("Sync Error");
  }

  if ((bytesReceived >= 2) && (bytesReceived == 1 + 1 + receptionBuffer[1]))
  {
    bytesReceived = 0; // reset the bytes received counter for the
                       // the next frame
    // when a complete frame was received, call the function frameReceived
    frameReceived(receptionBuffer[0], receptionBuffer[1], receptionBuffer[2]);
  }
}

int main(void)
{
  dbMc_init();
  dbMc_setSpeed(80);
  uart0_init(9600, UART_M_TRANSCEIVE, UART_P_NONE); // initialize UART0
  uart0_registerReceptionCallback(&byteReceived);
  sei();

  while (1)
  {
  }
}