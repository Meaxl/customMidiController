//* https://raw.githubusercontent.com/silveirago/DIY-Midi-Controller-full/master/Code%20-%20codigo/en-DIY_midi_controller-full-4051/en-DIY_midi_controller-full-4051.ino
/////////////////////////////////////////////
// Choosing your board
// Define your board, choose:
// "ATMEGA328" if using ATmega328 - Uno, Mega, Nano...
// "ATMEGA32U4" if using with ATmega32U4 - Micro, Pro Micro, Leonardo...
// "TEENSY" if using a Teensy board
// "DEBUG" if you just want to debug the code in the serial monitor
#define TEENSY 1 //* put here the uC you are using, like in the lines above followed by "1", like "ATMEGA328 1", "DEBUG 1", etc.

// MULTIPLEXER USING?
#define USING_MUX_4051 1 //* comment if not using a multiplexer 4051, uncomment if using it.

// LIBRARIES (4051 multiplexer, Threads)
#include "analogmuxdemux.h" // https://github.com/ajfisher/arduino-analog-multiplexer
#include <Thread.h> // Threads library >> https://github.com/ivanseidel/ArduinoThread
#include <ThreadController.h> // Same as above

// MULTIPLEXERS
#define N_MUX 5 //* number of multiplexers
//* Define s0, s1, s2, s3, and x pins
#define s0 2
#define s1 3
#define s2 4
#define x1 A0 // analog pin of the first mux
#define x2 A1 // analog pin of the second mux
#define x3 A2 // analog pin of the thrid mux
#define x4 A3 // analog pin of the fourth mux
#define x5 A4 // analog pin of the fifth mux

// Initializes the multiplexer
AnalogMux mux[N_MUX] = {
  AnalogMux(s0, s1, s2, x1), //*
  AnalogMux(s0, s1, s2, x2), //*
  AnalogMux(s0, s1, s2, x3), //*
  AnalogMux(s0, s1, s2, x4), //*
  AnalogMux(s0, s1, s2, x5) //*
  // ...
};

// POTENTIOMETERS
const int N_POTS = 0 + 8 + 8 + 8 + 8 + 8; //* total numbers of pots (slide & rotary). Number of pots in the Arduino + number of pots on multiplexer 1 + number of pots on multiplexer 2...
const int N_POTS_ARDUINO = 0; //* number of pots connected straight to the Arduino
const int POT_ARDUINO_PIN[N_POTS_ARDUINO] = {}; //* pins of each pot connected straight to the Arduino

/*#define USING_CUSTOM_CC_N 1 //* comment if not using CUSTOM CC NUMBERS, uncomment if using it.
#ifdef USING_CUSTOM_CC_N
int POT_CC_N[N_POTS] = {10, 15}; // Add the CC NUMBER of each pot you want
#endif*/

const int N_POTS_PER_MUX[N_MUX] = {8, 8, 8, 8, 8}; //* number of pots in each multiplexer (in order)
const int POT_MUX_PIN[N_MUX][8] = { //* pins of each pot of each mux in the order you want them to be
{0, 1, 2, 3, 4, 5, 6, 7, 8}, // pins of the first mux
{0, 1, 2, 3, 4, 5, 6, 7, 8}, // pins of the second mux
{0, 1, 2, 3, 4, 5, 6, 7, 8}, // pins of the thrid mux
{0, 1, 2, 3, 4, 5, 6, 7, 8}, // pins of the fourth mux
{0, 1, 2, 3, 4, 5, 6, 7, 8}  // pins of the fifth mux
};

int potCState[N_POTS] = {0}; // Current state of the pot
int potPState[N_POTS] = {0}; // Previous state of the pot
int potVar = 0; // Difference between the current and previous state of the pot

int potMidiCState[N_POTS] = {0}; // Current state of the midi value
int potMidiPState[N_POTS] = {0}; // Previous state of the midi value

const int TIMEOUT = 300; //* Amount of time the potentiometer will be read after it exceeds the varThreshold
const int varThreshold = 10; //* Threshold for the potentiometer signal variation
boolean potMoving = true; // If the potentiometer is moving
unsigned long PTime[N_POTS] = {0}; // Previously stored time
unsigned long timer[N_POTS] = {0}; // Stores the time that has elapsed since the timer was reset

// THREADS
ThreadController cpu; //thread master, where the other threads will be added
Thread threadPotentiometers; // thread to control the pots

void setup() {
  // Baud Rate
  // 31250 for MIDI class compliant | 115200 for Hairless MIDI
  Serial.begin(115200); //*


  // MULTIPLEXERS - Set each X pin as input_pullup (avoid floating behavior)
  pinMode(x1, INPUT_PULLUP);
  pinMode(x2, INPUT_PULLUP);
  pinMode(x3, INPUT_PULLUP);
  pinMode(x4, INPUT_PULLUP);
  pinMode(x5, INPUT_PULLUP);

  // THREADS
  threadPotentiometers.setInterval(10); // every how many millisiconds
  threadPotentiometers.onRun(potentiometers); // the function that will be added to the thread
  cpu.add(&threadPotentiometers); // add every thread here
}

void loop() {
    cpu.run(); // for threads
}


/////////////////////////////////////////////
// POTENTIOMETERS
/////////////////////////////////////////////
void potentiometers() {
    //It will happen if using a mux
    int nPotsPerMuxSum = N_POTS_ARDUINO; //offsets the buttonCState at every mux reading

    // reads the pins from every mux
    for (int j = 0; j < N_MUX; j++) {
      for (int i = 0; i < N_POTS_PER_MUX[j]; i++) {
        mux[j].SelectPin(POT_MUX_PIN[j][i]);
        potCState[i + nPotsPerMuxSum] = mux[j].AnalogRead();
      }
      nPotsPerMuxSum += N_POTS_PER_MUX[j];
    }

    for (int i = 0; i < N_POTS; i++) { // Loops through all the potentiometers
      potMidiCState[i] = map(potCState[i], 0, 1023, 0, 127); // Maps the reading of the potCState to a value usable in midi
      potVar = abs(potCState[i] - potPState[i]); // Calculates the absolute value between the difference between the current and previous state of the pot
      if (potVar > varThreshold) { // Opens the gate if the potentiometer variation is greater than the threshold
        PTime[i] = millis(); // Stores the previous time
      }

      timer[i] = millis() - PTime[i]; // Resets the timer 11000 - 11000 = 0ms
      if (timer[i] < TIMEOUT) { // If the timer is less than the maximum allowed time it means that the potentiometer is still moving
        potMoving = true;
      }
      else {
        potMoving = false;
      }

      if (potMoving == true) { // If the potentiometer is still moving, send the change control
        if (potMidiPState[i] != potMidiCState[i]) {
          // Sends the MIDI CC accordingly to the chosen board
          #ifdef TEENSY
          usbMIDI.sendControlChange(CC + i, potMidiCState[i], MIDI_CH); // CC number, CC value, midi channel
          #endif

          #ifdef USING_CUSTOM_CC_N
          usbMIDI.sendControlChange(POT_CC_N[i], potMidiCState[i], MIDI_CH); // CC number, CC value, midi channel
          #else
          usbMIDI.sendControlChange(CC + i, potMidiCState[i], MIDI_CH); // CC number, CC value, midi channel
          #endif


          Serial.print("Pot: ");
          Serial.print(i);
          Serial.print("  |  ch: ");
          Serial.print(MIDI_CH);
          Serial.print("  |  cc: ");
          #ifdef USING_CUSTOM_NN
          Serial.print(POT_CC_N[i]);
          #else
          Serial.print(CC + i);
          #endif
          Serial.print("  |  value: ");
          Serial.println(potMidiCState[i]);

          potPState[i] = potCState[i]; // Stores the current reading of the potentiometer to compare with the next
          potMidiPState[i] = potMidiCState[i];
        }
      }
    }
}