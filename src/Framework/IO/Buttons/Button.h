#include <Arduino.h>
#include "Framework/Data/TopicServer.h"


struct ButtonData
{
    const unsigned long longPressDuration = 1000; // Long press duration in milliseconds 
    const unsigned long IncPressDuration = 2000; // Long press duration in milliseconds 
    TopicServer* m_topicServer;
    const int rightButtonPin = 39; 
    const int middleButtonPin = 37;
    const int leftButtonPin = 38; 

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
};

// ISRs for button press
void IRAM_ATTR HandleLeftButton();

void IRAM_ATTR HandleMiddleButton();

void IRAM_ATTR HandleRightButton();

void ButtonInitialize(TopicServer& topicServer_);

void ButtonCheckIsPressed();