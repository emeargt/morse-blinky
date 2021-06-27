#include <Arduino.h>

#define MSG_LENGTH 56

/** message in morse code
 *  binary sequence: 1=ON, 0=OFF
 *  message is read from right to left and must fit in 64 bits
 *  encoding
 *    - dot = 1 bit, ON
 *    - dash = 3 bits, ON
 *    - space between morse characters = 1 bit, OFF
 *    - space between letters of message = 3 bits, OFF
 *    - space between words = 7 bits, OFF
 *  bit length is determined by timer0
 */
static const uint64_t msg = 0x000e3ab8abb88ea8; // VE7XT, left zero padded by 8 bits

ISR(TIMER1_COMPA_vect) {
  static unsigned int id = 0;
  // shift through message and mask out relevant bit
  if((msg >> id) & 1) PORTA |= 0x04; // turn on led
  else PORTA &= 0xfb; // turn off led
  id = (id + 1) % MSG_LENGTH;
}

void setup() {
  // turn off adc
  ADCSRA = 0;
  // disable analog comparator
  ACSR = 0x80; 
  
  // power reduction
  PRR = 0x07; // disable clocks to timer0, USI, and ADC

  // init PA2 as output
  DDRA |= 0x04;
  PORTA &= 0xfb; // turn leds off

  // setup timer
  TCCR1A = 0x00;
  TCCR1B = 0x0a; // CTC mode, prescale timer clock by 8
  OCR1A = 3200-1; // 3200*8/128k = 200 ms
  TIMSK1 |= (1 << OCIE1A); // enable compare A interrupts
}

void loop() {
  // don't need to do anything here
  // morse blink is handled in interrupt
}