#include "Framework/IO/Buttons/Button.h"


// Static instance for ISR access
ButtonHandler* ButtonHandler::instance() {
    static ButtonHandler instance(38, 37, 39); // Default pins; change as necessary
    return &instance;
}

ButtonHandler::ButtonHandler(int leftPin, int middlePin, int rightPin)
    : leftButtonPin(leftPin), middleButtonPin(middlePin), rightButtonPin(rightPin),
      leftPressed(false), middlePressed(false), rightPressed(false) {}

void ButtonHandler::Initialize(TopicServer topicServer_) {
    m_topicServer = topicServer_;
    pinMode(leftButtonPin, INPUT_PULLUP);
    pinMode(middleButtonPin, INPUT_PULLUP);
    pinMode(rightButtonPin, INPUT_PULLUP);

    attachInterrupt(digitalPinToInterrupt(leftButtonPin), HandleLeftButton, RISING);
    attachInterrupt(digitalPinToInterrupt(middleButtonPin), HandleMiddleButton, RISING);
    attachInterrupt(digitalPinToInterrupt(rightButtonPin), HandleRightButton, RISING);
}

void ButtonHandler::CheckIsPressed() {
    if (leftPressed) {
        m_topicServer.Publish("LeftButtonTopic");
        leftPressed = false;
    } 
    else if (rightPressed) {
        m_topicServer.Publish("RightButtonTopic");
        rightPressed = false;
    } 
    else if (middlePressed) {
        m_topicServer.Publish("MiddleButtonTopic");
        middlePressed = false;
    }
}

void IRAM_ATTR ButtonHandler::HandleLeftButton() {
    ButtonHandler::instance()->leftPressed = true;
}

void IRAM_ATTR ButtonHandler::HandleMiddleButton() {
    ButtonHandler::instance()->middlePressed = true;
}

void IRAM_ATTR ButtonHandler::HandleRightButton() {
    ButtonHandler::instance()->rightPressed = true;
}