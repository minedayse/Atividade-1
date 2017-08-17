const int ledPin = LED_BUILTIN;
float tempF = 30.0;
char oper;
String setPointS = "";
float setPointF = 0.0;

void setup() {
  Serial.begin(9600);
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, LOW);
}

void loop() {
  // put your main code here, to run repeatedly:
  while(!Serial.available());
  digitalWrite(ledPin1, LOW);
  oper = Serial.read();
  if (oper == 'r'){
    Serial.print(tempF);
    Serial.print('\n');
    tempF += 0.1;
  }
  else if(oper == 'w'){
    setPointS = Serial.readStringUntil('\n');
    setPointF = setPointS.toFloat();
  }
  if (tempF > setPointF){
    digitalWrite(ledPin3, HIGH);
  }
  else{
    digitalWrite(ledPin, LOW);
  }
}


// PARTE DA YASMIN


// which analog pin to connect
#define THERMISTORPIN A0
//turn reading on
#define ONVOLT 6         
// voltage at 0 degrees C
#define VINIT 2.08      
// voltage at 35 degrees C
#define V35 0.5   
// which pin to send signal (actuator)
#define ACT 9
#define NUMSAMPLES 5
 
uint16_t samples[NUMSAMPLES];

float UPTEMP = 40;
float DOWNTEMP = 20;

String inputString = "";

void setup(void) {
  Serial.begin(9600);
  pinMode(THERMISTORPIN,INPUT);
  pinMode(ONVOLT,OUTPUT);
  digitalWrite(ONVOLT,LOW);
  pinMode(ACT,OUTPUT);
  digitalWrite(ACT,LOW);
}
 
void loop(void) {
  

  while (Serial.available() > 0) {
      // read the incoming byte:
      char inChar = (char)Serial.read();
      inputString += inChar;
  }

  if (inputString[1]=='1') { //read temperature
      uint8_t i;
      float average=0;
      // take N samples in a row, with a slight delay
      for (i=0; i< NUMSAMPLES; i++) {
      digitalWrite(ONVOLT,HIGH); 
      samples[i] = analogRead(THERMISTORPIN);
      average += samples[i];
      delay(10);
      }
      digitalWrite(ONVOLT,LOW);
      average /= NUMSAMPLES; //value in number
      average = 5*average/1023; //value in voltage
      // convert the value to temperature
      average = (VINIT - average)*35/(VINIT - V35);
      Serial.println(average);
      if (average < DOWNTEMP) {
        digitalWrite(ACT,HIGH);
      } else if (average > UPTEMP) {
        digitalWrite(ACT,LOW);
      }
  } else if (inputString[1]=='0'){ //read limits
      if (inputString[2]=='1'){ //upper limit
          UPTEMP = inputString[3];  //TRANSFORM TO VOID
      } else if (inputString[2]=='0'){ //lower limit
          DOWNTEMP = inputString[3];  
      }
  }
}
