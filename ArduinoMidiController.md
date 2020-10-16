Arduino Midi Controller
======================
##### Information about the process:
The general architecture is shown below. The core of the controller is an Atmega
chip. A special bootloader is used to allow the chip to be reprogrammed directly
through USB. The user program can also communicate with the computer directly 
through USB, and actually be recognized as any USB device you would like,
including HID (Human Interface Device), and in our case, a MIDI controller. You
can then interface your controller with any music software speaking MIDI.

On the Atmega chip, potentiometers and switches can be connected, and mapped in 
the software to generate corresponding MIDI signals (control, notes, play/pause, 
...). 

Note that the Atmega alone accepts only 6 analog entries (i.e. needed to interface 
potentiometers). However it is relatively simple to add one or several
multiplexers to extend the capabilities (8 entries mapped to 1 entry, i.e. up to 
48 analog entries).

1. [Features](#features)
2. [Requirements](#requirements)
   1. [Software/Operating System (OS)](#software)
   2. [Shopping the Hardware](#hardware)
      1. [Main Parts](#mparts)
      2. ["real" controller stuff](#cparts)
      3. [Tools](#tools)
3. [Building process](#building)
   1. [USB Connection](#usb)
   2. [The Core](#core)
   3. [Flashing the code](#code)
4. [Credits](#credits)

The general architecture is shown below. The core of the controller is an Atmega
chip. A special bootloader is used to allow the chip to be reprogrammed directly
through USB. The user program can also communicate with the computer directly 
through USB, and actually be recognized as any USB device you would like,
including HID (Human Interface Device), and in our case, a MIDI controller. You
can then interface your controller with any music software speaking MIDI.

On the Atmega chip, potentiometers and switches can be connected, and mapped in 
the software to generate corresponding MIDI signals (control, notes, play/pause, 
...). 

Note that the Atmega alone accepts only 6 analog entries (i.e. needed to interface 
potentiometers). However it is relatively simple to add one or several
multiplexers to extend the capabilities (8 entries mapped to 1 entry, i.e. up to 
48 analog entries).

<a name="features"></a>
### 1. Features
======================
- **Cheap to build** : use directly a standalone Arduino Teensy microcontroller ;
- **Plug-and-play** : detected as an actual MIDI device ;
- **Easy reprogram through USB** : using the USB connection, you can enter a mode to reprogram via Arduino Software ;
- **Easily customizable, extandable**.
