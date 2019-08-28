volatile uint32_t st = 0;

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


void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  Serial.println("Hello");

  st = (st<<1) | 1;
  st = (st<<1) | 0;
  st = (st<<1) | 1;
  st = (st<<1) | 0;
  st = (st<<1) | 1;
  st = (st<<1) | 0;
  st = (st<<1) | 1;
  st = (st<<1) | 0;

  st = (st<<1) | 1;
  st = (st<<1) | 0;
  st = (st<<1) | 1;
  st = (st<<1) | 0;
  st = (st<<1) | 1;
  st = (st<<1) | 0;
  st = (st<<1) | 1;
  st = (st<<1) | 0;

  st = (st<<1) | 1;
  st = (st<<1) | 0;
  st = (st<<1) | 1;
  st = (st<<1) | 0;
  st = (st<<1) | 1;
  st = (st<<1) | 0;
  st = (st<<1) | 1;
  st = (st<<1) | 0;



  Serial.println(st);

  Serial.println((st&S1) ? HIGH : LOW);
  Serial.println((st&R1) ? HIGH : LOW);
  Serial.println((st&S2) ? HIGH : LOW); 
  Serial.println((st&R2) ? HIGH : LOW);

}


void loop() {
  // put your main code here, to run repeatedly:

}
