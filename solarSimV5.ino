
const byte analogPins[] ={A0, A1, A2, A3, A4, A5};
const byte panelSets[] = {5, 6, 7, 8, 9, 10};
const int number_of_panelSets = 2;
int iterator = 0;
const int ledPin1 = 2;
const int ledPin2 = 3;
const int ledPin3 = 4;
const int threshold = 900;
const int highVoltageCheckTime = 5000;
const int voltageCheckTime = 200;

void setup() {
  pinMode(analogPins[0], INPUT);
  pinMode(analogPins[1], INPUT);
  pinMode(analogPins[2], INPUT);
  pinMode(analogPins[3], INPUT);
  pinMode(analogPins[4], INPUT);
  pinMode(analogPins[5], INPUT);
  pinMode(panelSets[0], OUTPUT);
  pinMode(panelSets[1], OUTPUT);
  pinMode(panelSets[2], OUTPUT);
  pinMode(panelSets[3], OUTPUT);
  pinMode(panelSets[4], OUTPUT);
  pinMode(panelSets[5], OUTPUT);
  pinMode(ledPin1, OUTPUT);
  pinMode(ledPin2, OUTPUT);
  pinMode(ledPin3, OUTPUT);
  Serial.begin(9600);
}

void loop() {
  int analogPin = analogPins[iterator];
  int panelRelay = panelSets[iterator];
  digitalWrite(panelRelay, HIGH);
  int analogValue = analogRead(analogPin);

  if (digitalRead(ledPin1) == 0 || digitalRead(ledPin2) == 0 || digitalRead(ledPin3) == 0) {
    if (analogValue >= threshold) {
      digitalWrite(ledPin1, HIGH);
      delayTime(highVoltageCheckTime, analogPin);
      analogValue = analogRead(analogPin);
      if (analogValue >= threshold) {
        digitalWrite(ledPin2, HIGH);
        digitalWrite(ledPin1, LOW);
        delayTime(highVoltageCheckTime, analogPin);
        analogValue = analogRead(analogPin);
        if (analogValue >= threshold) {
          digitalWrite(ledPin1, HIGH);
          delayTime(highVoltageCheckTime, analogPin);
          analogValue = analogRead(analogPin);
          if (analogValue >= threshold) {
            digitalWrite(ledPin3, HIGH);
            digitalWrite(ledPin1, LOW);
            digitalWrite(ledPin2, LOW);
            delayTime(highVoltageCheckTime, analogPin);
            analogValue = analogRead(analogPin);
            if (analogValue >= threshold) {
              digitalWrite(ledPin3, HIGH);
              digitalWrite(ledPin1, HIGH);
              digitalWrite(ledPin2, LOW);
              delayTime(highVoltageCheckTime, analogPin);
              analogValue = analogRead(analogPin);
              if (analogValue >= threshold) {
                digitalWrite(ledPin3, HIGH);
                digitalWrite(ledPin1, LOW);
                digitalWrite(ledPin2, HIGH);
                delayTime(highVoltageCheckTime, analogPin);
                analogValue = analogRead(analogPin);
                if (analogValue >= threshold) {
                  digitalWrite(ledPin3, HIGH);
                  digitalWrite(ledPin1, HIGH);
                  digitalWrite(ledPin2, HIGH);
                  delayTime(highVoltageCheckTime, analogPin);
                  analogValue = analogRead(analogPin);
                }            
              }
            }
          }
        }
      }
    }
  }
  
  if (analogValue < threshold) {
    digitalWrite(ledPin1, LOW);
    digitalWrite(ledPin2, LOW);
    digitalWrite(ledPin3, LOW);
    digitalWrite(panelRelay, LOW);
    iterator++;
  }

  if (iterator == number_of_panelSets) {
    iterator = 0;
  }
  
  Serial.println(analogValue);
  delay(voltageCheckTime);        
}

void delayTime(int number_of_milliseconds, int analogPin) {
  int counter = 0;
  while (counter < (number_of_milliseconds / voltageCheckTime)) {
    int analogValue = analogRead(analogPin);
    Serial.println(analogValue);
    counter++;
    delay(voltageCheckTime);
  }
}
