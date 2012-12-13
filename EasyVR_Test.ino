/****************************************************
** // call-aid
** This is sample code that turns on the Arduino's
** onboard LED if any of the trained voice calls 
** are recognized.
****************************************************/

#if defined(ARDUINO) && ARDUINO >= 100
  #include "Arduino.h"
  #include "SoftwareSerial.h"
  SoftwareSerial port(12,13);
#else // Arduino 0022 - use modified NewSoftSerial
  #include "WProgram.h"
  #include "NewSoftSerial.h"
  NewSoftSerial port(12,13);
#endif

#include "EasyVR.h"
EasyVR easyvr(port);

//Groups and Commands
enum Groups
{
  GROUP_0  = 0,
  GROUP_1  = 1,
  GROUP_2  = 2,
};

enum Group0 
{
  G0_ARDUINO_TEST_1 = 0,
};

enum Group1 
{
  G1_NURSE_CALL_TEST_1 = 0,
  G1_YES_TEST_1 = 1,
  G1_NO_TEST_1 = 2,
  G1_CLAP_ON_1 = 3,
};

enum Group2 
{
  G2_CALL_NURSE_1 = 0,
  G2_CALL_NURSE_2 = 1,
};


EasyVRBridge bridge;

int8_t group, idx;

void setup()
{
  // bridge mode?
  if (bridge.check())
  {
    cli();
    bridge.loop(0, 1, 12, 13);
  }
  // run normally
  Serial.begin(9600);
  port.begin(9600);

  if (!easyvr.detect())
  {
    Serial.println("EasyVR not detected!");
    for (;;);
  }

  easyvr.setPinOutput(EasyVR::IO1, LOW);
  Serial.println("EasyVR detected!");
  easyvr.setTimeout(5);
  easyvr.setLanguage(0);

  // call-aid: Change the group below to point to the group
  // with the commands that you've trained
  group = 2;
}

void action();

void loop()
{
  // call-aid
  // Comment out the line below so the onboard LED is off
  // easyvr.setPinOutput(EasyVR::IO1, HIGH); // LED on (listening)

  Serial.print("Say a command in Group ");
  Serial.println(group);
  easyvr.recognizeCommand(group);

  do
  {
    // can do some processing while waiting for a spoken command
  }
  while (!easyvr.hasFinished());
 
  // call-aid: comment out the line beloew
  //  easyvr.setPinOutput(EasyVR::IO1, LOW); // LED off

  idx = easyvr.getWord();
  if (idx >= 0)
  {
    // built-in trigger (ROBOT)
    // group = GROUP_X; <-- jump to another group X
    group = 2; //  call-aid: set to your training group
    return;
  }
  idx = easyvr.getCommand();
  if (idx >= 0)
  {
    // print debug message
    uint8_t train = 0;
    char name[32];
    Serial.print("Command: ");
    Serial.print(idx);
    if (easyvr.dumpCommand(group, idx, name, train))
    {
      Serial.print(" = ");
      Serial.println(name);
    }
    else
      Serial.println();
    easyvr.playSound(0, EasyVR::VOL_FULL);
    // perform some action
    action();
  }
  else // errors or timeout
  {
    if (easyvr.isTimeout())
      Serial.println("Timed out, try again...");
    int16_t err = easyvr.getError();
    if (err >= 0)
    {
      Serial.print("Error ");
      Serial.println(err, HEX);
    }
  }
}

void action()
{
    switch (group)
    {
    case GROUP_0:
      switch (idx)
      {
      case G0_ARDUINO_TEST_1:
        // write your action code here
        // group = GROUP_X; <-- or jump to another group X for composite commands
        break;
      }
      break;
    case GROUP_1:
      switch (idx)
      {
      case G1_NURSE_CALL_TEST_1:
        // write your action code here
        // group = GROUP_X; <-- or jump to another group X for composite commands
        break;
      case G1_YES_TEST_1:
        // write your action code here
        // group = GROUP_X; <-- or jump to another group X for composite commands
        break;
      case G1_NO_TEST_1:
        // write your action code here
        // group = GROUP_X; <-- or jump to another group X for composite commands
        break;
      case G1_CLAP_ON_1:
        // write your action code here
        // group = GROUP_X; <-- or jump to another group X for composite commands
        break;
      }
      break;
    case GROUP_2:
      switch (idx)
      // call-aid: Each case below is a matched voice call
      {
      case G2_CALL_NURSE_1:
        Serial.println("Nurse Call 1 Activated");  //  call-aid
        easyvr.setPinOutput(EasyVR::IO1, HIGH); // call-aid: Turn on LED to show voice was recognized
        // write your action code here
        // group = GROUP_X; <-- or jump to another group X for composite commands
        break;
      case G2_CALL_NURSE_2:
        Serial.println("Nurse Call 2 Activated");  //  call-aid
        easyvr.setPinOutput(EasyVR::IO1, HIGH); // call-aid: Turn on LED to show voice was recognized
        // write your action code here
        // group = GROUP_X; <-- or jump to another group X for composite commands
        break;
      }
      break;
    }
}
