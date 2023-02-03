#define MOSFET_SWITCH 23
#define DEAD_IN 22
// sensor reads within 5mm distance
// 0 at closed
// 1 at open

uint doorstate = 0; // initiate with 0
void readSensor(){
  int data = digitalRead(DEAD_IN);
  if(doorstate != data){
    doorstate = data;
  }
}

void closeDoor(){
  delay(1000);
  digitalWrite(MOSFET_SWITCH, HIGH);
}

void openDoor(){
  delay(100);
  digitalWrite(MOSFET_SWITCH, LOW);
}

void checkSignal(){
  if(doorstate == 0){
    closeDoor();
  }else{
    openDoor();
  }
}

void main(){
  readSensor();
  delay(100);
  checkSignal();
  delay(100);
}

void setup() {
  Serial.begin(115200);
  Serial.println("Serial begin");
  pinMode(MOSFET_SWITCH, OUTPUT);
  pinMode(DEAD_IN, INPUT_PULLUP);
  digitalWrite(MOSFET_SWITCH,LOW);

  doorstate = digitalRead(DEAD_IN);
}

void loop() {
  main();
}
