
char flag;
void setup() 
{
pinMode(13, OUTPUT);
digitalWrite(13, LOW);

Serial.begin(9600);
}

void loop() {
  if (Serial.available() > 0)
  {
  flag = Serial.read();
  //Serial.print(flag);
  if(flag == '1'){
    digitalWrite(13, HIGH);
  }
  if (flag == '0'){
    digitalWrite(13, LOW);
  }
  }
}
