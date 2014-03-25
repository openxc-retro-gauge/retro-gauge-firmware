#include "communication.h"
#include "motor.h"
#include "display.h"
#include "led.h"

String input = "";
const int SPEED_SLOPE = 1739;

void receiveCommands() {
    if(Serial.available()) {
        while(Serial.available() > 0){
            char c = Serial.read();
            input += c;
            if(c == ')' || c =='>'){
                parse_message(input);
                input = "";
            } else if(c == 'u') {
                motorStepUp();
                input = "";
            } else if(c == 'd') {
                motorStepDown();
                input = "";
            }
        }
    }
}

void parse_message(String message) {
    if(message[0] == '(') {
        int digitVal = 10*(message[1]-'0') + (message[2]-'0');
        Serial.print("Setting display to: ");
        Serial.print(message[1]);
        Serial.println(message[2]);
        setDisplay(digitVal / 10, digitVal % 10);
    }

    Serial.print("message is: ");
    Serial.println(message);
    if(message[message.length() - 1] == ')') {
        double motorVal = 10*(message[message.length()-3]-'0') + (message[message.length()-2]-'0');
        unsigned long motorPer = motorVal * 1000/SPEED_SLOPE;
        unsigned long target = STEPS * motorPer / 100;
        Serial.print("Setting motor target to: ");
        Serial.println(target);
        setMotorTarget(target);
    }

    if (message[0] == '<') {
        int rgbValue = 100*(message[1] -'0') + 10*(message[2]-'0') + (message[3]-'0');
        setLED(rgbValue, 255, 255);
    }
}
