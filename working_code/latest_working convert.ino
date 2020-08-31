boolean debug = false;

// Byte für ControlChange-Befehl auf MIDI Kanal 1
byte controlChange = 176;

// Array für 2 achtfach Multiplexer = 16 Werte
int potiWert[16];
byte controllerWert[16];
byte controllerWertAlt[16];

// 3 Bits für den Zähler der Multiplexer
byte bit1 = 0;
byte bit2 = 0;
byte bit3 = 0;

void setup() {
  //Select-Pins 4051s
  pinMode(2, OUTPUT);
  pinMode(3, OUTPUT);
  pinMode(4, OUTPUT);

  //Pin 13 (Teensy LED)
  pinMode(13, OUTPUT);

  // 31250 Standard Midi Baud Rate
  // 9600 für Debugging
  Serial.begin(31250);
}

void loop() {
  usbMIDI.read();

  // Schleife fragt alle Potis an den 4051 Multiplexern ab
  for (byte i = 0; i <= 7; i++) {
    bit1 = bitRead(i,0);
    bit2 = bitRead(i,1);
    bit3 = bitRead(i,2);
  
    digitalWrite(2, bit1);
    digitalWrite(3, bit2);
    digitalWrite(4, bit3);
    
    potisAbfragen(i,A0);        //erster Multiplexer
    //potisAbfragen(i+8,A1);    //zweiter Multiplexer
    //potisAbfragen(i+16,A2);   //dritter Multiplexer
  }
}

void potisAbfragen(byte zaehler, int analogPin) {
  //Formel um ein Poti abzufragen und die Messwerte zu glätten
  potiWert[zaehler] = 0.2 * potiWert[zaehler] + 0.8 * analogRead(analogPin);
  controllerWert[zaehler] = map(potiWert[zaehler],0,1023,0,127);
  if (controllerWert[zaehler] != controllerWertAlt[zaehler]) {
    usbMIDI.sendControlChange(controlChange, controllerWert[count], (20 + count));
    controllerWertAlt[zaehler] = controllerWert[zaehler];

    //LED blinken lassen wenn Poti gedreht wird
    digitalWrite(13, HIGH);
    delay(5);
    digitalWrite(13, LOW);

    //Ausgabe für Debugging
    if (debug == true) {
      if (controllerWert[zaehler] != controllerWertAlt[zaehler]) {
        Serial.print((20 + count));
        Serial.print(" :    ");
        Serial.print(controllerWert[count]);
        Serial.println();
      }
    }
  }
}