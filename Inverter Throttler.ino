/**
 * @file solarSimV5.1.ino
 * @author Yashrif Hasib (yashrifhasib@gmail.com)
 * @brief Automatically throttle Solar Grid Tie Inverter DC Voltage to snap it out of its groove.
 * @version 5.3
 * @date 2022-06-28
 * 
 * GOAL: 1 - DEVELOP BASE CODE INCLUDING THE STAGES 0 TO 7 (BINARY).                                                                                         --CHECK
 * GOAL: 2 - INCORPORATE THE ABILITLY TO CONTROL TWO INVERTERS USING ONE SWITCHING PIN.                                                                      --CHECK
 * GOAL: 3 - ADDING ANOTHER IF CONDITION WHERE IF THE VOLTAGE IS TOO HIGH, IT WILL MOVE TO A HIGHER TIER IF CONDITION TO PUT MORE LOAD ON THE INVERTER.      --CHECK
 * GOAL: 4 - INCORPORATE THE ABILITY TO RESET THE INVERTER USING A RESETTING PIN.                                                                            --CHECK
 * 
 * @copyright Copyright (c) 2022
 */

// inverter voltage inputs pins
const int analogPin1 = A0;
const int analogPin2 = A1;
// load controlling pins
const int ledPin1 = 2;
const int ledPin2 = 3;
// const int ledPin3 = 4;
// inverter controlling pins
const int inverter1Reset = 5;
const int inverter2Reset = 6;
const int inverterSwitch = 7;
// load logic constants
const int threshold1 = 890;
const int threshold2 = 950;
// const int overvalue = 50;
// this is for delay time. Whenever there is an update (no change) this delay will be triggered to accomodate for RL timing.
const int highVoltageCheckTime = 5000; // standard is 5000
// since delay proves inefficient for the progra2m, highVoltageCheckTime is triggered in a loop in increments of voltageCheckTime
const int voltageCheckTime = 100;
// iterator is to make sure that I am looping each inverter exactly once. It is to make sure that I dont loop the same inverter twice.
bool iterator = false;
// resetCounter is used to reset the inverter after 1 iteration.
bool resetCounter = false;

/**
 * @brief void setup() is there to make sure everything is ready for what is going to happen in void loop()
 * Setting up all the pins to make sure they are ready to accept input or send output. 
 * NOTE: The Serial.begin(9600) is for testing purpose only and is not needed for practical use.
 */
void setup() {
  pinMode(analogPin1, INPUT); // Inverter 1 input pin
  pinMode(analogPin2, INPUT); // Inverter 2 input pin
  pinMode(ledPin1, OUTPUT); // Relay pin for turning on Light 1
  pinMode(ledPin2, OUTPUT); // Relay pin for turning on Light 2
  // pinMode(ledPin3, OUTPUT); // Relay pin for turning on Light 3
  pinMode(inverter1Reset, OUTPUT); // Pin that controls Inverter 1 power
  pinMode(inverter2Reset, OUTPUT); // Pin that controls Inverter 2 power
  pinMode(inverterSwitch, OUTPUT); // Pin that acts as a 2-way-switch between the two inverters. It is needed to put the load of the lights on the correct inverter since there is only one set of them.
  Serial.begin(9600); //  Serial is used for testing purposes only.
}

/**
 * @brief the main purpose of this function is to update the Serial in a given delayed time.
 * When delay(time) is used, the program sleeps the whole time without progressing through the code.
 * As a result, this function is used such that the serial is updated every 100ms.
 * @param timeDelayed This is actual time that is going to be delayed.
 * @param analogPin This is the analog pin where the program is going to be updating its values from.
 * NOTE: This function is also for testing only.
 */
void delayTime(int timeDelayed, int analogPin) {
  int counter = 0;
  while (counter < (timeDelayed / voltageCheckTime)) { // Dividing the total time by 100ms
    counter++;  // increasing the counter so the loop is not endless.
    int analogValue = analogRead(analogPin); // updating the voltage value
    Serial.println(analogValue); // printing the voltage value
    delay(voltageCheckTime); // delaying by small increments.
  }
}

/**
 * @brief The void loop() is an infinite loop that keeps the program running 24/7 
 * If conditions are sequences therefore switch/case is not used
 */
void loop() {
  int analogValue = 0;
  int analogPin = 0;
  int threshold = 0;
  int inverterReset = 0;
  if (iterator) { // deciding which inverter is going to be picked.
    analogPin = analogPin2; // picking the right analogPin
    threshold = threshold2; // picking the right threshold
    inverterReset = inverter2Reset; // picking the right inverter reset switch.
  }
  else {
    analogPin = analogPin1;
    threshold = threshold1;
    inverterReset = inverter1Reset;
  }
  iterator = !iterator; // toggling the iterator value
  analogValue = analogRead(analogPin);

  if (resetCounter && analogValue >= threshold)
    goto iteration_2;
  resetCounter = false;
  // This is stage 000. This means that either the inverter is running in way it should or the program is still looping through the code.
  if (digitalRead(ledPin1) == 0 || digitalRead(ledPin2) == 0 /*|| digitalRead(ledPin3)*/ == 0) {
    /*if (analogValue >= threshold) {
      if (iterator) { digitalWrite(inverterSwitch, LOW); } // this is here where the program decides if its going to be inv1 or inv 2.
      else { digitalWrite(inverterSwitch, HIGH); }
      if ((analogValue - threshold) >= overvalue) { // Stage 001 of ledPins.
        goto tierUpgrade; // label used to jump to a further condition.
      }
      digitalWrite(ledPin1, HIGH); // The ledPins are updated from 0 through 7 in binary mode.. 000, 001, 010, 011, 100, 101, 110, 111.
      delayTime(highVoltageCheckTime, analogPin); // delay can be used but the serial will not be updated Testing use only. Applies to delayTime(timeDelayed, analogPin) calls.
      analogValue = analogRead(analogPin); // updating the current voltage
      Serial.println(analogValue); // Testing only.. applies to Serials.
      if (analogValue >= threshold) { // Stage 010 of ledPins
        digitalWrite(ledPin2, HIGH);
        digitalWrite(ledPin1, LOW);
        delayTime(highVoltageCheckTime, analogPin);
        analogValue = analogRead(analogPin);
        Serial.println(analogValue);
        if (analogValue >= threshold) { // Stage 011 of ledPins
          digitalWrite(ledPin1, HIGH);
          delayTime(highVoltageCheckTime, analogPin);
          analogValue = analogRead(analogPin);
          Serial.println(analogValue);*/
          if (analogValue >= threshold) { // Stage 100 of ledPins
            if (iterator) { digitalWrite(inverterSwitch, LOW); } // this is here where the program decides if its going to be inv1 or inv 2.
            else { digitalWrite(inverterSwitch, HIGH); }
            //digitalWrite(ledPin3, HIGH);
            digitalWrite(ledPin1, HIGH);
            delayTime(highVoltageCheckTime, analogPin);
            analogValue = analogRead(analogPin);
            Serial.println(analogValue);
            if (analogValue >= threshold) { // Stage 101 of ledPins
              digitalWrite(ledPin1, LOW);
              digitalWrite(ledPin2, HIGH);
              delayTime(highVoltageCheckTime, analogPin);
              analogValue = analogRead(analogPin);
              Serial.println(analogValue);
              if (analogValue >= threshold) { // Stage 110 of ledPins
                digitalWrite(ledPin1, HIGH);
                delayTime(highVoltageCheckTime, analogPin);
                analogValue = analogRead(analogPin);
                Serial.println(analogValue);
                if (analogValue >= threshold) { // Stage 111 of ledPins.
                  iteration_2:
                    delayTime(highVoltageCheckTime, analogPin);
                    analogValue = analogRead(analogPin);
                    Serial.println(analogValue);
                    if (!resetCounter) {
                      resetCounter = true;
                      iterator = !iterator;
                      analogValue = analogRead(analogPin); // updating the value
                      Serial.println(analogValue); // printing the updated value
                      delay(voltageCheckTime); // give a 100ms delay before the next loop starts to incorporate real life conditions and timing. 
                      return;
                    }
                    resetCounter = false;
                  if (analogValue >= threshold) { // this is after the ledpins have reached 111 stage. Since this is the last stage, the next step is to restart the inverter.
                    // digitalWrite(ledPin3, LOW);
                    digitalWrite(ledPin1, LOW);
                    digitalWrite(ledPin2, LOW);
                    digitalWrite(inverterReset, HIGH); // inverterReset begin HIGH means that the switch to the inverter is open. 
                    delayTime(1000, analogPin); // Starting the inverter again after 1 second
                    digitalWrite(inverterReset, LOW); // inverterReset begin LOW means that the switch to the inverter is closed.
                    delayTime(10000, analogPin); // Giving the inverter 10 seconds to follow through with its starting process.
                  }
                }            
              }
            }
          }
        }

  if (analogValue < threshold) { // this condition is activated if the inverter is running perfectly. This is where stage 000 is activated and kept if voltage is below threshold. 
    digitalWrite(ledPin1, LOW);
    digitalWrite(ledPin2, LOW);
    // digitalWrite(ledPin3, LOW);
  }
}
