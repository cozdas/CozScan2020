void setup() 
{
  pinMode(0, OUTPUT);   //CH1
  pinMode(1, OUTPUT);   //CH2
  pinMode(2, OUTPUT);   //CH1
  pinMode(3, OUTPUT);   //CH2
  pinMode(4, OUTPUT);   //CH1
  pinMode(5, OUTPUT);   //CH1
  pinMode(6, OUTPUT);   //CH2
  pinMode(7, OUTPUT);   //CH3
  pinMode(8, OUTPUT);   //CH4
  pinMode(9, OUTPUT);   //CH5
  pinMode(10, OUTPUT);  //CH6
  pinMode(11, OUTPUT);  //CH7
  pinMode(12, OUTPUT);  //CH8
  pinMode(13, OUTPUT);  //CH8
  
  pinMode(A0, OUTPUT);  //CH9
  pinMode(A1, OUTPUT);  //CH9
  pinMode(A2, OUTPUT);  //CH10
  pinMode(A3, OUTPUT);  //OUTB->A
  pinMode(A4, OUTPUT);  //OUTB->B
  pinMode(A5, OUTPUT);  //OUTB->B
  pinMode(A6, OUTPUT);  //OUTB->B
  pinMode(A7, OUTPUT);  //OUTB->B
}

void loop() 
{
  digitalWrite(0, HIGH);
  digitalWrite(1, HIGH);
  delay(250);
  digitalWrite(0, LOW);
  digitalWrite(1, LOW);
  delay(250);

}
