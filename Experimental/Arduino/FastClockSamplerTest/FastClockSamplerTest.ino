const byte clockPIN =2;
const byte strobePIN=3;
const byte dataPIN  =4;

union State
{
  uint32_t state;
  
  struct //r:reset, s:set, g:gate for ch5 & ch10 SSRs (in some models)
  {
    unsigned int
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
};

volatile State st;


#define cbi(sfr, bit) (_SFR_BYTE(sfr) &= ~_BV(bit))
void setup() 
{
  st.state = 0;
  //inputs
  pinMode(clockPIN, INPUT);
  pinMode(strobePIN, INPUT);
  pinMode(dataPIN, INPUT);

  //led
  pinMode(13, OUTPUT);

  //outputs
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
  
  //disable timer interrupt so that we can respond to clock 
  //as quickly as possible.
  cbi (TIMSK0, TOIE0); // disable Timer0
}

//this sets/resets the outputs depending on the collected bits
//This function can be optimized by direct port access which will
//both be faster and will let parallel writes
void strobe()
{
  uint32_t s = st.state&0x0fff;
  while(s)
  {
     PORTB = (PORTB&~(1<<PB5)) | ((s&1)<<PB5);
     s>>=1;
  }

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
  
}

//using interrupts was too slow, the clock ISR was
//totally missing the data. Actively waiting the rising clock
//edge is much faster, data is sampled before the falling edge
//of the clock

void loop() 
{
  //wait rising edge
  while(!(PIND&(1<<PD2)))
  {
    if(PIND&(1<<PD3)) //check strobe
    {
      strobe();
    }
  }  
  
  //sample
  uint8_t b = ((PIND>>PD4)&1);
  st.state =(st.state<<1) | b;

  //show bit on LED
  PORTB |= (b<<PB5);

  //wait falling edge
  while(PIND&(1<<PD2));

  //turn LED off
  PORTB &= ~(1<<PB5);
}
