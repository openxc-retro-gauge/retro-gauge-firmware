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
            if(c == ')' || c =='>'){
                parse_message(input);
                input = "";
            } else if(c == 'u') {
                motorStepUp();
            } else if(c == 'd') {
                motorStepDown();
            } else {
                input += c;
            }
        }
    }
}

void parse_message(String message) {
    if(message[0] == '(') {
        int digitVal = 10*(message[1]-'0') + (message[2]-'0');
        //Update Digits
        setDisplay(LED_CHAR_SET[digitVal/10],LED_CHAR_SET[digitVal%10]);
    }

    if(message[message.length()-1] == ')') {
        double motorVal = 10*(message[message.length()-3]-'0') + (message[message.length()-2]-'0');
        unsigned long motorPer = motorVal*1000/SPEED_SLOPE;
        setMotorTarget(STEPS * motorPer / 100);
    }

    if (message[0] == '<') {
        int rgbValue = 100*(message[i+1] -'0') + 10*(message[i+2]-'0') + (message[i+3]-'0');
        setLED(rgbValue, 255, 255);
    }
}
