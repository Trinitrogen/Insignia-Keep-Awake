/*
 *  Insignia Keep Awake
 *  I recieved an Insignia 2.1 Channel soundbard and wireless subwoofer
 *  Model NS-SBAR21F20. It unfortunately is configured to shutoff after 
 *  15 minutes of inactivity requireing me to use the remote to power it
 *  back up.
 *  
 *  Using the microcontroller of your choice (NodeMCU in my case) and a 
 *  940nm IR LED, this code will increase the volume by 1 and then decrease
 *  by on a set schedule which is enough to keep it alive.
 
 *
 *  Any really good code you see in here primarily comes from 
 *  from the SimpleSender example in https://github.com/Arduino-IRremote/Arduino-IRremote.
 *  
 *  This code can be adapted to any device of your choosing by  
 *  modifying the Address and Command. Use an IR reciever along
 *  with the SimpleReciever example in the IRremote library 
 *  to determine the correct address and command. 
 *
 *  MIT License
 */
#include <Arduino.h>

//#define SEND_PWM_BY_TIMER         // Disable carrier PWM generation in software and use (restricted) hardware PWM.
//#define USE_NO_SEND_PWM           // Use no carrier PWM, just simulate an active low receiver signal. Overrides SEND_PWM_BY_TIMER definition

#include "PinDefinitionsAndMore.h" // Define macros for input and output pin etc.
#include <IRremote.hpp>

void setup() {
    pinMode(LED_BUILTIN, OUTPUT);

    Serial.begin(115200);

    // Just to know which program is running on my Arduino
    Serial.println(F("START " __FILE__ " from " __DATE__ "\r\nUsing library version " VERSION_IRREMOTE));

    /*
     * The IR library setup. That's all!
     */
    IrSender.begin(); // Start with IR_SEND_PIN as send pin and if NO_LED_FEEDBACK_CODE is NOT defined, enable feedback LED at default feedback LED pin

    Serial.print(F("Ready to send IR signals at pin "));
    Serial.println(IR_SEND_PIN);
}

/*
 * Set up the data to be sent.
 * For most protocols, the data is build up with a constant 8 (or 16 byte) address
 * and a variable 8 bit command.
 * There are exceptions like Sony and Denon, which have 5 bit address.
 */
uint16_t sAddress = 0xED85;
uint8_t upCommand = 0x61;
uint8_t downCommand = 0x60;
uint8_t sRepeats = 0;
int KeepAliveCycle = 420000; //Delay in miliseconds before the the arduino sends another keepalive
//int KeepAliveCycle = 5000;

void loop() {


    // Send commands
    Serial.println();
    Serial.print(F("Sending DOWN command: address=0x"));
    Serial.print(sAddress, HEX);
    Serial.print(F(" command=0x"));
    Serial.print(upCommand, HEX);
    Serial.print(F(" repeats="));
    Serial.print(sRepeats);
    Serial.println();
    IrSender.sendNEC(sAddress, downCommand, sRepeats);

    delay(1000);

    Serial.println();
    Serial.print(F("Sending UP command: address=0x"));
    Serial.print(sAddress, HEX);
    Serial.print(F(" command=0x"));
    Serial.print(upCommand, HEX);
    Serial.print(F(" repeats="));
    Serial.print(sRepeats);
    Serial.println();
    IrSender.sendNEC(sAddress, upCommand, sRepeats);


    Serial.println(F("Send NEC with 16 bit address"));
    Serial.flush();

    delay(KeepAliveCycle);
}
