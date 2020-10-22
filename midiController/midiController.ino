#include <analogmuxdemux.h>
#include <StaticThreadController.h>
#include <Thread.h>
#include <ThreadController.h>


#define N_MUX 5 //* number of multiplexers
#define s0 2 //* select pins of multiplexers
#define s1 3 //*      "----"
#define s2 4 //*      "----"
#define x1 A0 //* analog pin of the first mux
#define x2 A1 //* analog pin of the second mux
#define x3 A2 //* analog pin of the thrid mux
#define x4 A3 //* analog pin of the fourth mux
#define x5 A4 //* analog pin of the fifth mux

byte MIDI_CH = 1; //* MIDI channel to be used
byte NOTE = 36; //* Lowest NOTE to be used
byte CC = 1; //* Lowest MIDI CC to be used (176)

// Initializes the multiplexer
AnalogMux mux[N_MUX] = {
  AnalogMux(s0, s1, s2, x1), 
  AnalogMux(s0, s1, s2, x2),
  AnalogMux(s0, s1, s2, x3),
  AnalogMux(s0, s1, s2, x4),
  AnalogMux(s0, s1, s2, x5)
};

const int N_POTS = 8 + 8 + 8 + 8 + 8; //* total numbers of pots

const int N_POTS_PER_MUX[N_MUX] = {8, 8, 8, 8, 8}; //* number of pots in each multiplexer (in order)
const int POT_MUX_PIN[N_MUX][8] = { //* pins of each pot of each mux in the order you want them to be
{0, 1, 2, 3, 4, 5, 6, 7}, //* pins of the first mux
{0, 1, 2, 3, 4, 5, 6, 7},
{0, 1, 2, 3, 4, 5, 6, 7},
{0, 1, 2, 3, 4, 5, 6, 7},
{0, 1, 2, 3, 4, 5, 6, 7}
};

int potCState[N_POTS] = {0}; //* Current state of the pot
int potPState[N_POTS] = {0}; //* Previous state of the pot
int potVar = 0; //* Difference between the current and previous state of the pot

int potMidiCState[N_POTS] = {0}; //* Current state of the midi value
int potMidiPState[N_POTS] = {0}; //* Previous state of the midi value

const int TIMEOUT = 300; //* Amount of time the potentiometer will be read after it exceeds the varThreshold
const int varThreshold = 30; //* Threshold for the potentiometer signal variation
boolean potMoving = true; // If the potentiometer is moving
unsigned long PTime[N_POTS] = {0}; // Previously stored time
unsigned long timer[N_POTS] = {0}; // Stores the time that has elapsed since the timer was reset

ThreadController cpu; //* thread master, where the other threads will be added
Thread threadPotentiometers; //* thread to control the pots

void setup() {
  pinMode(x1, INPUT_PULLUP); //* set each pin at input_pullup - avoid floating value
  pinMode(x2, INPUT_PULLUP); //* analog inputs of multiplexers
  pinMode(x3, INPUT_PULLUP); //* analog inputs of multiplexers
  pinMode(x4, INPUT_PULLUP); //* analog inputs of multiplexers
  pinMode(x5, INPUT_PULLUP); //* analog inputs of multiplexers

  /*pinMode(s0, OUTPUT);
  pinMode(s1, OUTPUT);
  pinMode(s2, OUTPUT);*/
  pinMode(13, OUTPUT); //* Teensy LED

  Serial.begin(31250); //* 31250 Standard Midi Baud Rate | 115200 for Hairless MIDI

  // THREADS
  threadPotentiometers.setInterval(10); //* every how many millisiconds
  threadPotentiometers.onRun(potentiometers); //* the function that will be added to the thread
  cpu.add(&threadPotentiometers); //* add every thread here
}

void loop() {
  usbMIDI.read();
  cpu.run(); //* for threads
}

void potentiometers() {
  int nPotsPerMuxSum = 0; //* offsets the buttonCState at every mux reading

  //* reads the pins from every mux
    for (int j = 0; j < N_MUX; j++) {
      for (int i = 0; i < N_POTS_PER_MUX[j]; i++) {
        mux[j].SelectPin(POT_MUX_PIN[j][i]);
        potCState[i + nPotsPerMuxSum] = mux[j].AnalogRead();
      }
      nPotsPerMuxSum += N_POTS_PER_MUX[j];
    }

    for (int i = 0; i < N_POTS; i++) { //* Loops through all the potentiometers
      potMidiCState[i] = map(potCState[i], 0, 1023, 0, 127); //* Maps the reading of the potCState to a value usable in midi
      potVar = abs(potCState[i] - potPState[i]); //* Calculates the absolute value between the difference between the current and previous state of the pot
      if (potVar > varThreshold) { //* Opens the gate if the potentiometer variation is greater than the threshold
        PTime[i] = millis(); //* Stores the previous time
      }

      timer[i] = millis() - PTime[i]; //* Resets the timer 11000 - 11000 = 0ms
      if (timer[i] < TIMEOUT) { //* If the timer is less than the maximum allowed time it means that the potentiometer is still moving
        potMoving = true;
      }
      else {
        potMoving = false;
      }

      if (potMoving == true) { //* If the potentiometer is still moving, send the change control
        if (potMidiPState[i] != potMidiCState[i]) {
          usbMIDI.sendControlChange(CC + i, potMidiCState[i], MIDI_CH); //* CC number, CC value, midi channel

          potPState[i] = potCState[i]; //* Stores the current reading of the potentiometer to compare with the next
          potMidiPState[i] = potMidiCState[i];
           
           // Output Poti states to serial monitor
           /*Serial.print("Pot: ");
           Serial.print(i);
           Serial.print("  |  ch: ");
           Serial.print(MIDI_CH);
           Serial.print("  |  cc: ");
           Serial.print(CC + i);
           Serial.print("  |  value: ");
           Serial.print(potMidiCState[i]);
           Serial.print("  |  raw: ");
           Serial.println(potCState[i]);*/

          digitalWrite(13, HIGH);
          delay(5);
          digitalWrite(13, LOW);
        }
      }
    }    
}
