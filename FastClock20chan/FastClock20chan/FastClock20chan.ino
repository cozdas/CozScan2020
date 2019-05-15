//TODO: we can add sanity check to make sure no two channels are connected to the same output at the same time
//need to make sure we have enough clock budget left for that though

//using interrupts was too slow, the clock ISR was
//totally missing the data. Actively waiting the rising clock
//edge is much faster, now the data is properly sampled before the falling edge
//of the clock


//Arduino DATA pins on the Micro board
//D0 = pin 20  RX
//D1 = pin 21  TX
//D2 = pin 24  D2
//D3 = pin 25  D3
//D4 = pin 26  D4
//D5 = pin 27  D5
//D6 = pin 28  D6
//D7 = pin 29  D7
//D8 = pin 30  D8
//D9 = pin 31  D9
//D10 = pin 32 D10
//D11 = pin 33 D11
//D12 = pin 34 D12
//D13 = pin 1  D13
//D14 = pin16  MISO 
//D15 = pin17  SCK
//D16 = pin18  MOSI
//D17 = pin19  SS (RX LED)
//D18 = pin4   A0
//D19 = pin5   A1
//D20 = pin6   A2
//D21 = pin7   A3
//D22 = pin8   A4
//D23 = pin9   A5
//D24 = pin26  D4 ***duplicate***
//D25 = pin28  D6 ***duplicate***
//D26 = pin30  D8 ***duplicate***
//D27 = pin31  D9 ***duplicate***
//D28 = pin32  D10 ***duplicate***
//D29 = pin34  D12 ***duplicate***
//D30 = TXLED  --

/* SCAN CHANNEL TO PIN MAPPING
chan DPin   NAME  PORT    BIT   MicroPin
0     --, 
1     15    PB1   PORTB   1     17
2     14    PB3   PORTB   3     16
3     16    PB2   PORTB   2     18
4     17    PB0   PORTB   0     19
5     1     PD3   PORTD   3     21
6     0     PD2   PORTD   2     20
7     2     PD1   PORTD   1     24
8     3     PD0   PORTD   0     25
9     4     PD4   PORTD   4     26
10    5     PC6   PORTC   6     27
11    21    PF4   PORTF   4     7
12    22    PF1   PORTF   1     8
13    23    PF0   PORTF   0     9
14    12    PD6   PORTD   6     34
15    11    PB7   PORTB   7     33
16    10    PB6   PORTB   6     32
17    9     PB5   PORTB   5     31
18    8     PB4   PORTB   4     30
19    7     PE6   PORTE   6     29
20    6     PD7   PORTD   7     28
21    13    PC7   PORTC   7     1
22    30    PD5   PORTD   5     (11: wire from TX led)
*/

/* CONTROL BIS TO CHANNEL AND PIN MAPPINGS
//first sent bit is MSB (bit48), last sent bit is LSB (bit0)
//even bits: reset coil
//odd bits: set coil
//ctrl bit, chan, Dpin, PORTBIT,  PORT,   BIT
0           11    21    PF4       PORTF   4
2           12    22    PF1       PORTF   1
4           13    23    PF0       PORTF   0
6           14    12    PD6       PORTD   6
8           15    11    PB7       PORTB   7
10          16    10    PB6       PORTB   6
12          17    9     PB5       PORTB   5
14          18    8     PB4       PORTB   4
16          19    7     PE6       PORTE   6
18          20    6     PD7       PORTD   7
20          1     15    PB1       PORTB   1
22          2     14    PB3       PORTB   3 
24          3     16    PB2       PORTB   2
26          4     17    PB0       PORTB   0
28          5     1     PD3       PORTD   3
30          6     0     PD2       PORTD   2
32          7     2     PD1       PORTD   1
34          8     3     PD0       PORTD   0
36          9     4     PD4       PORTD   4
38          10    5     PC6       PORTC   6
40          21    13    PC7       PORTC   7 //4WIRE
*/

//temp variables to hold register set/clear bits
uint8_t _PB, _PC, _PD, _PE, _PF;

//mapping from scanner control bits to MCU registers (2 bits per entry, first bit clear, second set)
uint8_t* controlbit2port[]    = {&_PF,&_PF,&_PF,&_PD,&_PB,&_PB,&_PB,&_PB,&_PE,&_PD,&_PB,&_PB,&_PB,&_PB,&_PD,&_PD,&_PD,&_PD,&_PD,&_PC,&_PC};
uint8_t  controlbit2portbit[] = {   4,   1,   0,   6,   7,   6,   5,   4,   6,   7,   1,   3,   2,   0,   3,   2,   1,   0,   4,   6,   7};

//channel number to Arduino digital pin mapping. Note the scanner card starts from chan1 not 0
uint8_t Chan2Pin[]            = {99 ,15 ,14 ,16 ,17 ,1  ,0  ,2  ,3  ,4  ,5  ,21 ,22 ,23 ,12 ,11 ,10 ,9  ,8  ,7  ,6  ,13 ,30};

//control bits are shifted into this 64bit variable
uint64_t  state = 0;
uint8_t   numBitsCollected = 0;


void TestRun()
{
  for(;;)
  {
    //reset chan 21, this will connect bus_b to intput
    state = 1ll<<40;
    numBitsCollected = 48;
    strobe();

    //connect all channels to input one by one
    for(int i=0;i<20;++i)
    {
      //close relay
      state = 1ll<<(2*i+1);
      numBitsCollected = 48;
      strobe();
      delay(250);

      //open relay
      state = 1ll<<(2*i);
      numBitsCollected = 48;
      strobe();
      delay(50);
    }
  }
}



//we process the bits in two passes to guarantee the "break before connect" ordering.
//pass1: read the odd bits, which are reset coil commands, and clear the corresponding port bits for those channels
//pass2: read the even bits, which are set coil commands, and set the corresponding port bits for those channels
//(heavy bit manipulation for fastest write)
void strobe()
{
  //we can safely ignore the coil deenergizing events
  if(state==0)
    return;

  //we are out of sync if strobe comes not after exactly 48 bits. 
  //Disconnect everything to be on the safer side
  /*
  if(numBitsCollected!=48)
  {
    PORTB = PORTC = PORTD = PORTE = PORTF =0; 
    return;
  }
  */


  //Pass0: process the reset bits (even bits)
  {
    _PB = _PC = _PD = _PE = _PF = 0;
    uint64_t st = state;
    //uint32_t st = states[0];
    for(int i=0; i<21; ++i, st>>=2)
    {
      (*controlbit2port[i]) |= (st&1)<<(controlbit2portbit[i]);
    }

    PORTB &= ~_PB;
    PORTC &= ~_PC;
    PORTD &= ~_PD;
    PORTE &= ~_PE;
    PORTF &= ~_PF;  

    //also if the SENSE connection is set, break the INPUT connection now
    if(state&(1ll<<41))
      PORTD &= ~(1<<5);
  }

  //Pass1:  process the set bits (odd bits)
  {

    _PB = _PC = _PD = _PE = _PF = 0;
    uint64_t st = state>>1;
    //uint32_t st = states[1];
    for(int i=0; i<21; ++i, st>>=2)
    {
      (*controlbit2port[i]) |= (st&1)<<(controlbit2portbit[i]);
    } 

    PORTB |= _PB;
    PORTC |= _PC;
    PORTD |= _PD;
    PORTE |= _PE;
    PORTF |= _PF;
  } 

  //also if the SENSE connection was broken, sat the INPUT connection now
  if(state&(1ll<<40))
    PORTD |= (1<<5);

  state = 0;  
  numBitsCollected = 0;
}

void setup() 
{
  //Serial.begin(115200);
  //set all channel pins to output
  for(int i=1; i<23; ++i)
  {
    pinMode(Chan2Pin[i], OUTPUT);
    digitalWrite(Chan2Pin[i], LOW);
  }

  //set the input pins
  pinMode(18, INPUT); //A0: PF7: DATA
  pinMode(19, INPUT); //A1: PF6: CLOCK
  pinMode(20, INPUT); //A2: PF5: STROBE

  state = 0;  
  numBitsCollected = 0;

  //  TestRun();
}

//active waiting the pin states is much faster than interrupt driven system
//busy waiting the clocks and strobe there in a very thight loop
void loop() 
{
  //cli(); //disabling interrupts right away causes the Arduino IDE fail to upload the sketch.

  while(1)
  {
    //wait rising edge of the clock
    while(!(PINF&(1<<6)))
    {
      //while waiting check strobe rising edge
      if(PINF&(1<<5)) 
      {
        strobe();
        //don't need to wait strobe signal's falling edge, even if strobe retriggers, since state will be 0, it'll return immediately 
      }
    }

    //we delay disabling interrupts because if this executes right away the 
    //uploader can't upload (probably verify) the sketch thus giving error during upload.
    //I'm guessing that the sketch runs before the verify begins and when interrupts are disabled
    //USB doesn't work.
    cli();  
    
    //sample
    uint8_t b = ((PINF>>7)&1);
    state = (state<<1) | b;
    numBitsCollected++;
    
    //wait falling clock edge
    while(PINF&(1<<6));
  }
}
