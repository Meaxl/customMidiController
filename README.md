Arduino Midi Controller
======================
1. [Features](#features)
2. [Requirements (Software/OS)](#software)
3. [Shopping the Hardware](#hardware)
4. [Building process](#building)
5. [Credits](#credits)

-------------------
<a name="features"></a>
## 1. Features
- **Cheap to build** : use directly a standalone Arduino Teensy microcontroller ;
- **Plug-and-play** : detected as an actual MIDI device ;
- **Easy reprogram through USB** : using the USB connection, you can enter a mode to reprogram via Arduino Software ;
- **Easily customizable, extandable**.

<a name="software"></a>
## 2. Requirements (Software/OS)
   - OSX, Windows, Linux
   - latest [Arduino IDE](https://www.arduino.cc/en/Main/software) + [Teensy Loader Applicatoin](https://www.pjrc.com/teensy/loader.html)


<a name="hardware"></a>
## 3. Shopping the Hardware
This are the most important things to consider buying for this controller
- 1x [Teensy 3.2](https://www.pjrc.com/store/teensy32.html)
- 5x [74HC4051](https://www.reichelt.de/multi-demultiplexer-8-ch-2--10-v-dil-16-74hc-4051-p3229.html?&nbc=1) (for the 74HCT4051 you need 5V)
- 5x [Socket for 74HC4051](https://www.reichelt.de/ic-sockel-16-polig-doppelter-federkontakt-gs-16-p8208.html?&trstct=pos_2&nbc=1)
- 40x [Rotary Potentiometer](https://www.musikding.de/Alpha-Poti-16mm-gewinkelt-print-5k-lin) (5k lin) + [*Caps*](https://www.musikding.de/Doppelnasenknopf-18mm) for it
- 1x [USB Connector](https://www.reichelt.de/usb-2-0-a-stecker-freie-enden-1-8-m-sw-usb-a-10080109-p198964.html?&nbc=1)
- 5x [PCB Connector](https://www.musikding.de/Platinen-Steckverbinder-8-polig)
- Some colored wires, dont use this cheap jumper wires
- [Single/double sided prototyping board](https://www.reichelt.de/lochrasterplatine-hartpapier-160x100mm-h25pr160-p8272.html?&nbc=1)
- Optional: Creating own [PCB](https://jlcpcb.com)

<a name="building"></a>
## 4. Building process
### USB Connection
The first real power within the Teensy 3.2, is the fact that it by default can send MIDI Messages via USB. So we don't need to interface with issues relied to that. You can straight use the default (Mini)USB-Port on the Teensy or Solder your own wire directly on the bottom side (as shown below)

![Soldering wire from USB cabel](https://raw.githubusercontent.com/Meaxl/customMidiController/master/documentation/pictures/solder_usb.jpg)

The USB spec actually requires these 4 wire colors, with this assignment to the signals:
- pin 1, +5V, Red
- pin 2, D-, White
- pin 3, D+, Green
- pin 4, GND, Black

### The Core
The whole construction uses one part as its main component and thats the Teensy. To get enough analog inputs I will use the *74HC4051 Mux*. Per Mux we gain 8 additional analog inputs. There are also some Shiftregisters, 16x Mux, etc. but for this Midicontroller i decided for this ones. At the beginning i was laying out the principial logic for the use of these components on a breadboard. Later on i'll replace it with a proper PCB.
For more pictures about the building process feel free to check them out in the [documentation](./documentation/) folder

![Schematic for Teensy + Mux](https://raw.githubusercontent.com/Meaxl/customMidiController/b11f5fe3154a61964d7c11e2af461ba269272206/documentation/schematics/schematic.svg)

<a name="credits"></a>
## 4. Credits
- [Meaxl](https://github.com/Meaxl/) for creating [customMidiController](https://github.com/Meaxl/customMidiController)
- @ajfisher for [Analog Mux/Demux library](https://github.com/ajfisher/arduino-analog-multiplexer)
- @ivanseidel for [ArduinoThread](https://github.com/ivanseidel/ArduinoThread/)
