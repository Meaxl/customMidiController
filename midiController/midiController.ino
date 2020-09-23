int controlChange = 176; //Control-Change auf MIDI Kanal 1

int potiWert[8];
int controllerWert[8];
int controllerWertAlt[8];

int i = 0;

int bit1 = 0;
int bit2 = 0;
int bit3 = 0;

void setup() {
  pinMode(2, OUTPUT);       // Select-Pins 4051
  pinMode(3, OUTPUT);
  pinMode(4, OUTPUT);
  Serial.begin(9600);

  pinMode(13, OUTPUT);      // PIN 13 = LED
  digitalWrite(13, LOW);    // LED aus
  boot_led();               // Blinken während Boot
  delay(1000);              // Warten damit alles lädt
}

void loop() {
  usbMIDI.read();

  for (i = 0; i <= 7; i++) {
    bit1 = bitRead(i, 0);
    bit2 = bitRead(i, 1);
    bit3 = bitRead(i, 2);

    digitalWrite(2, bit1);
    digitalWrite(3, bit2);
    digitalWrite(4, bit3);

    potisAbfragen(i, A0);
    potisAbfragen(i+8,A1);    //zweiter Multiplexer
    //potisAbfragen(i+16,A2);   //dritter Multiplexer
  }
  delay(1); //delay um floating values zu verhindern. bei bedarf erhöhen
}

void potisAbfragen(int count, int analogPin) {
  potiWert[count] = 0.3 * potiWert[count] + 0.6 * analogRead(analogPin);
  controllerWert[count] = map(potiWert[count], 0, 1023, 0, 127);

  if (controllerWert[count] != controllerWertAlt[count]) {
    usbMIDI.sendControlChange(controlChange, controllerWert[count], (20 + count));
    
    digitalWrite(13, HIGH);
    delay(5);
    digitalWrite(13, LOW);

  Serial.print((20 + count));
  Serial.print(" :    ");
  Serial.print(controllerWert[count]);
  Serial.println();

    controllerWertAlt[count] = controllerWert[count];
  }
}

void boot_led() {
  digitalWrite(13, LOW);
  digitalWrite(13, HIGH);
  delay(250);
  digitalWrite(13, LOW);
  delay(250);
  digitalWrite(13, HIGH);
  delay(250);
  digitalWrite(13, LOW);
  delay(250);
}
