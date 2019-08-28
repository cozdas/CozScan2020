#include "avr/io.h"
#include "avr/pgmspace.h"

const byte clockPIN=2;
const byte strobePIN=3;
const byte dataPIN=4;

union State
{
  uint32_t state;
/*  
  struct
  {
    unsigned int
    :8,
    r7: 1,
    s8: 1, r8: 1,
    s9: 1, r9: 1,
    
    r10: 1, s10: 1, g10: 1,
    r5 : 1, s5 : 1, g5 : 1,
    
    outb_b: 1,
    outb_a: 1,
    
    s6: 1, r6: 1,
    s7: 1, 
    s1: 1, r1: 1, 
    s2: 1, r2: 1,
    s3: 1, r3: 1,
    s4: 1, r4: 1;
  };
*/
/*
  struct
  {
    unsigned int a0:1 ,a1:1, a2:1, a3:1, a4:1, a5:1, a6:1, a7:1;
    unsigned int b0:1 ,b1:1, b2:1, b3:1, b4:1, b5:1, b6:1, b7:1;
    unsigned int c0:1 ,c1:1, c2:1, c3:1, c4:1, c5:1, c6:1, c7:1;
  };
*/
};

volatile uint32_t st = 0;

void clockISR()
{
  //digitalWrite(13, HIGH);
  //PORTB |= (1<<PB5); //B5: D13
  
  //int in = digitalRead(dataPIN);
  //st = (st<<1) | ((in==HIGH)?1:0);

  st = (st<<1) | ((PORTD>>PD4)&1);
 
  //digitalWrite(13, LOW);
  PORTB ^= (1<<PB5); //toggle led
}

const uint32_t R7 = 1UL<<0;

const uint32_t S8 = 1UL<<1;
const uint32_t R8 = 1UL<<2;

const uint32_t S9 = 1UL<<3;
const uint32_t R9 = 1UL<<4;

const uint32_t R10= 1UL<<5;
const uint32_t S10= 1UL<<6;
const uint32_t G10= 1UL<<7;

const uint32_t R5= 1UL<<8;
const uint32_t S5= 1UL<<9;
const uint32_t G5= 1UL<<10;

const uint32_t OUTB_B = 1UL<<11;
const uint32_t OUTB_A = 1UL<<12;

const uint32_t S6 = 1UL<<13;
const uint32_t R6 = 1UL<<14;

const uint32_t S7 = 1UL<<15;

const uint32_t S1 = 1UL<<16;
const uint32_t R1 = 1UL<<17;

const uint32_t S2 = 1UL<<18;
const uint32_t R2 = 1UL<<19;

const uint32_t S3 = 1UL<<20;
const uint32_t R3 = 1UL<<21;

const uint32_t S4 = 1UL<<22;
const uint32_t R4 = 1UL<<23;

void strobeISR()
{
  /*
  if(st.r1)      digitalWrite(5,LOW);
  else if(st.s1) digitalWrite(5,HIGH);

  if(st.r2)      digitalWrite(6,LOW);
  else if(st.s2) digitalWrite(6,HIGH);

  if(st.r3)      digitalWrite(7,LOW);
  else if(st.s3) digitalWrite(7,HIGH);

  if(st.r4)      digitalWrite(8,LOW);
  else if(st.s4) digitalWrite(8,HIGH);

  if(st.r5)      digitalWrite(9,LOW);
  else if(st.s5) digitalWrite(9,HIGH);

  if(st.r6)      digitalWrite(10,LOW);
  else if(st.s6) digitalWrite(10,HIGH);

  if(st.r7)      digitalWrite(11,LOW);
  else if(st.s7) digitalWrite(11,HIGH);

  if(st.r8)      digitalWrite(12,LOW);
  else if(st.s8) digitalWrite(12,HIGH);

  if(st.r9)      digitalWrite(A1,LOW);
  else if(st.s9) digitalWrite(A1,HIGH);

  if(st.r10)      digitalWrite(A2,LOW);
  else if(st.s10) digitalWrite(A2,HIGH);

  if(st.outb_a)
  {
    digitalWrite(A4,LOW);
    digitalWrite(A3,HIGH);
  }
  else if(st.outb_b)
  {
    digitalWrite(A3,LOW);
    digitalWrite(A4,HIGH);
  }
  */
  digitalWrite(5, (st&S1) ? HIGH : LOW);
  digitalWrite(6, (st&R1) ? HIGH : LOW);
  digitalWrite(7, (st&S2) ? HIGH : LOW); 
  digitalWrite(8, (st&R2) ? HIGH : LOW);
  
  digitalWrite(13, LOW);
}

#define cbi(sfr, bit) (_SFR_BYTE(sfr) &= ~_BV(bit))
void setup() 
{
  //disable timer interrupt so that we can respond to clock ASAP
  cbi (TIMSK0, TOIE0); // disable Timer0
  
  pinMode(clockPIN, INPUT_PULLUP);
  pinMode(strobePIN, INPUT_PULLUP);
  pinMode(dataPIN, INPUT_PULLUP);

  attachInterrupt(digitalPinToInterrupt(clockPIN), clockISR, RISING);
  attachInterrupt(digitalPinToInterrupt(strobePIN), strobeISR, RISING);

  pinMode(5, OUTPUT);   //CH1
  pinMode(6, OUTPUT);   //CH2
  pinMode(7, OUTPUT);   //CH3
  pinMode(8, OUTPUT);   //CH4
  pinMode(9, OUTPUT);   //CH5
  pinMode(10, OUTPUT);  //CH6
  pinMode(11, OUTPUT);  //CH7
  pinMode(12, OUTPUT);  //CH8
  pinMode(A1, OUTPUT);  //CH9
  pinMode(A2, OUTPUT);  //CH10
  pinMode(A3, OUTPUT);  //OUTB->A
  pinMode(A4, OUTPUT);  //OUTB->B
}

void loop() 
{
    //PORTB = B00100000; //B5: D13
    //delay(500);
    //PORTB = B00000000; //B5: D13
    //delay(500);
}
