# Arduino Midi Controller
Trying to build my own custom Midi Controller. Code and any further documentation can be found here.

Features
--------
- **Cheap to build** : use directly a standalone Arduino Teensy microcontroller ;
- **Plug-and-play** : detected as an actual MIDI device automatically ;
- **Easy reprogram through USB** : using the USB connection, you can enter a mode to reprogram via Arduino Software ;
- **Easily customizable, extandable**.


Comments
--------
Maybe need this piece code later
```
delay(10); // to prevent slight fluctuations from sending through
// MIDI Controllers should discard incoming MIDI messages.
while (usbMIDI.read()) {
}
```
