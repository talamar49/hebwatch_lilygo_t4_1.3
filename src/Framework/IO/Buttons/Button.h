#include <Arduino.h>

const int rightButtonPin = 39; // Change to your pin
const int middleButtonPin = 37; // Change to your pin
const int leftButtonPin = 38; // Change to your pin

// Function prototypes
void IRAM_ATTR HandleLeftButton();
void IRAM_ATTR HandleMiddleButton();
void IRAM_ATTR HandleRightButton();

// Configure button pins as inputs with pull-up resistors
pinMode(rightButtonPin, INPUT_PULLUP);
pinMode(middleButtonPin, INPUT_PULLUP);
pinMode(leftButtonPin, INPUT_PULLUP);

// Attach interrupts to the buttons
attachInterrupt(digitalPinToInterrupt(leftButtonPin), HandleLeftButton, RISING);
attachInterrupt(digitalPinToInterrupt(middleButtonPin), HandleMiddleButton, RISING);
attachInterrupt(digitalPinToInterrupt(rightButtonPin), HandleRightButton, RISING);

bool leftPressed;
bool rightPressed;
bool middlePressed;

void IRAM_ATTR HandleLeftButton()
{
leftPressed = true;
}

void IRAM_ATTR HandleMiddleButton()
{
middlePressed = true;
}

void IRAM_ATTR HandleRightButton()
{
rightPressed = true;
}

void CheckIsPressed(bool& left, bool& right, bool& middle)
{
if(left) {
g_topicServer.Publish("LeftButtonTopic");
left = false;
}
else if(right) {
g_topicServer.Publish("RightButtonTopic");
right = false;
}
else if(middle){
g_topicServer.Publish("MiddleButtonTopic");
middle = false;
}
}