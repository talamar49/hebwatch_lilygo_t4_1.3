#include "Button.h"

ButtonData buttonData;

// ISRs for button press
void IRAM_ATTR HandleLeftButton() {
    if (digitalRead(buttonData.leftButtonPin) == LOW) { // Button pressed (assuming active LOW)
        buttonData.leftPressStartTime = millis(); // Record the time when the button was pressed
        buttonData.leftPressed = true;
        buttonData.leftLongPressHandled = false; // Reset long press handled flag
    } else { // Button released
        buttonData.leftPressed = false;
    }
}

void IRAM_ATTR HandleMiddleButton() {
    if (digitalRead(buttonData.middleButtonPin) == LOW) { // Button pressed (assuming active LOW)
        buttonData.middlePressStartTime = millis(); // Record the time when the button was pressed
        buttonData.middlePressed = true;
        buttonData.middleLongPressHandled = false; // Reset long press handled flag
    } else { // Button released
        buttonData.middlePressed = false;
    }
}

void IRAM_ATTR HandleRightButton() {
    if (digitalRead(buttonData.rightButtonPin) == LOW) { // Button pressed (assuming active LOW)
        buttonData.rightPressStartTime = millis(); // Record the time when the button was pressed
        buttonData.rightPressed = true;
        buttonData.rightLongPressHandled = false; // Reset long press handled flag
    } else { // Button released
        buttonData.rightPressed = false;
    }
}

void ButtonInitialize(TopicServer& topicServer_) {
    buttonData.m_topicServer = &topicServer_;
    pinMode(buttonData.leftButtonPin, INPUT_PULLUP);
    pinMode(buttonData.middleButtonPin, INPUT_PULLUP);
    pinMode(buttonData.rightButtonPin, INPUT_PULLUP);

    // Attach interrupts for both FALLING and RISING edges
    attachInterrupt(digitalPinToInterrupt(buttonData.leftButtonPin), HandleLeftButton, CHANGE);
    attachInterrupt(digitalPinToInterrupt(buttonData.middleButtonPin), HandleMiddleButton, CHANGE);
    attachInterrupt(digitalPinToInterrupt(buttonData.rightButtonPin), HandleRightButton, CHANGE);
}

void ButtonCheckIsPressed() {
    unsigned long currentTime = millis();

    // Check left button
    if (buttonData.leftPressed) {
        if (currentTime - buttonData.leftPressStartTime >= buttonData.longPressDuration && !buttonData.leftLongPressHandled) {
            buttonData.m_topicServer->Publish("LeftButtonLongPressTopic"); // Long press
            buttonData.leftLongPressHandled = true; // Mark long press as handled
        }

        if(currentTime - buttonData.leftPressStartTime >= buttonData.IncPressDuration)
        {
            buttonData.m_topicServer->Publish("LeftButtonIncTopic"); // Long press
        }
    } else if (buttonData.leftPressStartTime != 0 && !buttonData.leftLongPressHandled) {
        buttonData.m_topicServer->Publish("LeftButtonShortPressTopic"); // Short press
        buttonData.leftPressStartTime = 0; // Reset the timer
    }

    // Check middle button
    if (buttonData.middlePressed) {
        if (currentTime - buttonData.middlePressStartTime >= buttonData.longPressDuration && !buttonData.middleLongPressHandled) {
            buttonData.m_topicServer->Publish("MiddleButtonLongPressTopic"); // Long press
            buttonData.middleLongPressHandled = true; // Mark long press as handled
        }
    } else if (buttonData.middlePressStartTime != 0 && !buttonData.middleLongPressHandled) {
        buttonData.m_topicServer->Publish("MiddleButtonShortPressTopic"); // Short press
        buttonData.middlePressStartTime = 0; // Reset the timer
    }

    // Check right button
    if (buttonData.rightPressed) {
        
        if (currentTime - buttonData.rightPressStartTime >= buttonData.longPressDuration && !buttonData.rightLongPressHandled) {
            buttonData.m_topicServer->Publish("RightButtonLongPressTopic"); // Long press
            buttonData.rightLongPressHandled = true; // Mark long press as handled
        }
        
        if(currentTime - buttonData.rightPressStartTime >= buttonData.IncPressDuration)
        {
            buttonData.m_topicServer->Publish("RightButtonIncTopic"); // Long press
        }

    } else if (buttonData.rightPressStartTime != 0 && !buttonData.rightLongPressHandled) {
        buttonData.m_topicServer->Publish("RightButtonShortPressTopic"); // Short press
        buttonData.rightPressStartTime = 0; // Reset the timer
    }
}