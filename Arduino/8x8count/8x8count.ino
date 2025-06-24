#define R8 13
#define R7 A3
#define R6 2
#define R5 10
#define R4 9
#define R3 3
#define R2 8
#define R1 5

#define C1 A2
#define C2 7
#define C3 6
#define C4 12
#define C5 4
#define C6 11
#define C7 A4
#define C8 A5


int timer = 0;
int decade, unos;
int count;


void setup() {               
  pinMode(R1, OUTPUT);
  pinMode(R2, OUTPUT);
  pinMode(R3, OUTPUT);
  pinMode(R4, OUTPUT);
  pinMode(R5, OUTPUT);
  pinMode(R6, OUTPUT);
  pinMode(R7, OUTPUT);
  pinMode(R8, OUTPUT);

  pinMode(C1, OUTPUT);
  pinMode(C2, OUTPUT);
  pinMode(C3, OUTPUT);
  pinMode(C4, OUTPUT);
  pinMode(C5, OUTPUT);
  pinMode(C6, OUTPUT);
  pinMode(C7, OUTPUT);
  pinMode(C8, OUTPUT);

  

}

void SelectRow (int row){
  if (row == 1) digitalWrite(R1, HIGH); else digitalWrite(R1,LOW);
  if (row == 2) digitalWrite(R2, HIGH); else digitalWrite(R2,LOW);
  if (row == 3) digitalWrite(R3, HIGH); else digitalWrite(R3,LOW);
  if (row == 4) digitalWrite(R4, HIGH); else digitalWrite(R4,LOW);
  if (row == 5) digitalWrite(R5, HIGH); else digitalWrite(R5,LOW);
  if (row == 6) digitalWrite(R6, HIGH); else digitalWrite(R6,LOW);
  if (row == 7) digitalWrite(R7, HIGH); else digitalWrite(R7,LOW);
  if (row == 8) digitalWrite(R8, HIGH); else digitalWrite(R8,LOW);
  
}

void SelectColumn(int column){
  if (column == 1) digitalWrite(C1, LOW); else digitalWrite(C1,HIGH);
  if (column == 2) digitalWrite(C2, LOW); else digitalWrite(C2,HIGH);
  if (column == 3) digitalWrite(C3, LOW); else digitalWrite(C3,HIGH);
  if (column == 4) digitalWrite(C4, LOW); else digitalWrite(C4,HIGH);
  if (column == 5) digitalWrite(C5, LOW); else digitalWrite(C5,HIGH);
  if (column == 6) digitalWrite(C6, LOW); else digitalWrite(C6,HIGH);
  if (column == 7) digitalWrite(C7, LOW); else digitalWrite(C7,HIGH);
  if (column == 8) digitalWrite(C8, LOW); else digitalWrite(C8,HIGH);
  
}

void SelectDot(int row, int column){
SelectRow(row);
SelectColumn(column);
}

void Num(int n, int pos){
  int shift=0;
if (pos == 1){
  shift = 4;
}
else {
  shift = 0;
}
int i;
if (n==0){
  for(i=0; i<=30; i++){
SelectDot(1 + shift ,2);
delay(1);
SelectDot(1 + shift,3);
delay(1);
SelectDot(1 + shift,4);
delay(1);
SelectDot(1 + shift,5);
delay(1);
SelectDot(1 + shift,6);
delay(1);
SelectDot(2 + shift,2);
delay(1);
SelectDot(2 + shift,6);
delay(1);
SelectDot(3 + shift,2);
delay(1);
SelectDot(3 + shift ,3);
delay(1);
SelectDot(3 + shift,4);
delay(1);
SelectDot(3 + shift,5);
delay(1);
SelectDot(3 + shift,6);
delay(1);
  }
}

if(n == 1){
  for(i=0;i<=30;i++){
SelectDot(3 + shift,2);
delay(1);
SelectDot(3 + shift,3);
delay(1);
SelectDot(3 + shift,4);
delay(1);
SelectDot(3 + shift,5);
delay(1);
SelectDot(3 + shift,6);
delay(1);
  }
}

if(n==2){
  for (i = 0; i<=10; i++){
  SelectDot(1 + shift,2);
  delay(1);
  SelectDot(1 + shift,4);
  delay(1);
  SelectDot(1 + shift,5);
  delay(1);
  SelectDot(1 + shift,6);
  delay(1);
  SelectDot(2 + shift,2);
  delay(1);
  SelectDot(2 + shift,4);
  delay(1);
  SelectDot(2 + shift,6);
  delay(1);
  SelectDot(3 + shift,2);
  delay(1);
  SelectDot(3 + shift,3);
  delay(1);
  SelectDot(3 + shift,4);
  delay(1);
  SelectDot(3 + shift,6);
  delay(1);
  }
}

if(n == 3){
  for(i=0; i<=10; i++){
  SelectDot(1 + shift,2);
  delay(1);
  SelectDot(1 + shift,4);
  delay(1);
  SelectDot(1 + shift,6);
  delay(1);
  SelectDot(2 + shift,2);
  delay(1);
  SelectDot(2 + shift,4);
  delay(1);
  SelectDot(2 + shift,6);
  delay(1);
  SelectDot(3 + shift,2);
  delay(1);
  SelectDot(3 + shift,3);
  delay(1);
  SelectDot(3 + shift,4);
  delay(1);
  SelectDot(3 + shift,5);
  delay(1);
  SelectDot(3 + shift,6);
  delay(1);
  }
}
if (n==4){
  for(i=0; i<=10; i++){
  SelectDot(1 + shift,2);
  delay(1);
  SelectDot(1 + shift,3);
  delay(1);
  SelectDot(1 + shift,4);
  delay(1);
  SelectDot(2 + shift,4);
  delay(1);
  SelectDot(3 + shift,2);
  delay(1);
  SelectDot(3 + shift,3);
  delay(1);
  SelectDot(3 + shift,4);
  delay(1);
  SelectDot(3 + shift,5);
  delay(1);
  SelectDot(3 + shift,6);
  delay(1);
  }
}
if(n == 5){
  for(i=0;i<=10;i++){
  SelectDot(1 + shift, 2);
  delay(1);
  SelectDot(1 + shift, 3);
  delay(1);
  SelectDot(1 + shift, 4);
  delay(1);
  SelectDot(1 + shift, 6);
  delay(1);
  SelectDot(2 + shift, 2);;
  delay(1);
  SelectDot(2 + shift, 4);;
  delay(1);
  SelectDot(2 + shift, 6);;
  delay(1);
  SelectDot(3 + shift, 2);;
  delay(1);
  SelectDot(3 + shift, 4);;
  delay(1);
  SelectDot(3 + shift, 5);;
  delay(1);
  SelectDot(3 + shift, 6);;
  delay(1);
  }
}
if (n == 6){
  for(i=0;i<=10;i++){
  SelectDot(1 + shift, 2);
  delay(1);
  SelectDot(1 + shift, 3);
  delay(1);
  SelectDot(1 + shift, 4);
  delay(1);
  SelectDot(1 + shift, 5);
  delay(1);
  SelectDot(1 + shift, 6);
  delay(1);
  SelectDot(2 + shift, 2);;
  delay(1);
  SelectDot(2 + shift, 4);;
  delay(1);
  SelectDot(2 + shift, 6);;
  delay(1);
  SelectDot(3 + shift, 2);;
  delay(1);
  SelectDot(3 + shift, 4);;
  delay(1);
  SelectDot(3 + shift, 5);;
  delay(1);
  SelectDot(3 + shift, 6);;
  delay(1);
  }
}
if (n == 7){
  for(i=0; i<=10; i++){
  SelectDot(1 + shift, 2);
  delay(1);
  SelectDot(2 + shift, 2);
  delay(1);
  SelectDot(3 + shift, 2);
  delay(1);
  SelectDot(3 + shift, 3);
  delay(1);
  SelectDot(3 + shift, 4);
  delay(1);
  SelectDot(3 + shift, 5);
  delay(1);
  SelectDot(3 + shift, 6);
  delay(1);
  }
}
if (n == 8){
  for(i=0;i<=10;i++){
SelectDot(1 + shift,2);
delay(1);
SelectDot(1 + shift,3);
delay(1);
SelectDot(1 + shift,4);
delay(1);
SelectDot(1 + shift,5);
delay(1);
SelectDot(1 + shift,6);
delay(1);
SelectDot(2 + shift,2);
delay(1);
SelectDot(2 + shift,4);
delay(1);
SelectDot(2 + shift,6);
delay(1);
SelectDot(3 + shift,2);
delay(1);
SelectDot(3 + shift,3);
delay(1);
SelectDot(3 + shift,4);
delay(1);
SelectDot(3 + shift,5);
delay(1);
SelectDot(3 + shift,6);
delay(1);
  }
}
if (n == 9){
  for(i=0; i<=10; i++){
SelectDot(1 + shift,2);
delay(1);
SelectDot(1 + shift,3);
delay(1);
SelectDot(1 + shift,4);
delay(1);
SelectDot(1 + shift,6);
delay(1);
SelectDot(2 + shift,2);
delay(1);
SelectDot(2 + shift,4);
delay(1);
SelectDot(2 + shift,6);
delay(1);
SelectDot(3 + shift,2);
delay(1);
SelectDot(3 + shift,3);
delay(1);
SelectDot(3 + shift,4);
delay(1);
SelectDot(3 + shift,5);
delay(1);
SelectDot(3 + shift,6);
delay(1);
  }
}

}

void loop() {
  if (timer > 9){
    timer = 0;
  }
decade = timer/10;
unos = timer%10;

Num(unos, 1);
delay(10);
timer++;
delay(1000);

}