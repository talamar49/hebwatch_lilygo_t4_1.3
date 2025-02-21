#include <Arduino.h>
#include "Framework/Data/TopicServer.h"

TopicServer* m_topicServer;
const int rightButtonPin = 39; // Change to your pin
const int middleButtonPin = 37; // Change to your pin
const int leftButtonPin = 38; // Change to your pin

// Button state and timing variables
bool leftPressed = false;
bool rightPressed = false;
bool middlePressed = false;
unsigned long leftPressStartTime = 0;
unsigned long rightPressStartTime = 0;
unsigned long middlePressStartTime = 0;
bool leftLongPressHandled = false; // Track if long press has been handled
bool rightLongPressHandled = false; // Track if long press has been handled
bool middleLongPressHandled = false; // Track if long press has been handled
const unsigned long longPressDuration = 1000; // Long press duration in milliseconds (e.g., 1 second)

// ISRs for button press
void IRAM_ATTR HandleLeftButton() {
    if (digitalRead(leftButtonPin) == LOW) { // Button pressed (assuming active LOW)
        leftPressStartTime = millis(); // Record the time when the button was pressed
        leftPressed = true;
        leftLongPressHandled = false; // Reset long press handled flag
    } else { // Button released
        leftPressed = false;
    }
}

void IRAM_ATTR HandleMiddleButton() {
    if (digitalRead(middleButtonPin) == LOW) { // Button pressed (assuming active LOW)
        middlePressStartTime = millis(); // Record the time when the button was pressed
        middlePressed = true;
        middleLongPressHandled = false; // Reset long press handled flag
    } else { // Button released
        middlePressed = false;
    }
}

void IRAM_ATTR HandleRightButton() {
    if (digitalRead(rightButtonPin) == LOW) { // Button pressed (assuming active LOW)
        rightPressStartTime = millis(); // Record the time when the button was pressed
        rightPressed = true;
        rightLongPressHandled = false; // Reset long press handled flag
    } else { // Button released
        rightPressed = false;
    }
}

void ButtonInitialize(TopicServer& topicServer_) {
    m_topicServer = &topicServer_;
    pinMode(leftButtonPin, INPUT_PULLUP);
    pinMode(middleButtonPin, INPUT_PULLUP);
    pinMode(rightButtonPin, INPUT_PULLUP);

    // Attach interrupts for both FALLING and RISING edges
    attachInterrupt(digitalPinToInterrupt(leftButtonPin), HandleLeftButton, CHANGE);
    attachInterrupt(digitalPinToInterrupt(middleButtonPin), HandleMiddleButton, CHANGE);
    attachInterrupt(digitalPinToInterrupt(rightButtonPin), HandleRightButton, CHANGE);
}

void ButtonCheckIsPressed() {
    unsigned long currentTime = millis();

    // Check left button
    if (leftPressed) {
        if (currentTime - leftPressStartTime >= longPressDuration && !leftLongPressHandled) {
            m_topicServer->Publish("LeftButtonLongPressTopic"); // Long press
            leftLongPressHandled = true; // Mark long press as handled
        }
    } else if (leftPressStartTime != 0 && !leftLongPressHandled) {
        m_topicServer->Publish("LeftButtonShortPressTopic"); // Short press
        leftPressStartTime = 0; // Reset the timer
    }

    // Check middle button
    if (middlePressed) {
        if (currentTime - middlePressStartTime >= longPressDuration && !middleLongPressHandled) {
            m_topicServer->Publish("MiddleButtonLongPressTopic"); // Long press
            middleLongPressHandled = true; // Mark long press as handled
        }
    } else if (middlePressStartTime != 0 && !middleLongPressHandled) {
        m_topicServer->Publish("MiddleButtonShortPressTopic"); // Short press
        middlePressStartTime = 0; // Reset the timer
    }

    // Check right button
    if (rightPressed) {
        if (currentTime - rightPressStartTime >= longPressDuration && !rightLongPressHandled) {
            m_topicServer->Publish("RightButtonLongPressTopic"); // Long press
            rightLongPressHandled = true; // Mark long press as handled
        }
    } else if (rightPressStartTime != 0 && !rightLongPressHandled) {
        m_topicServer->Publish("RightButtonShortPressTopic"); // Short press
        rightPressStartTime = 0; // Reset the timer
    }
}