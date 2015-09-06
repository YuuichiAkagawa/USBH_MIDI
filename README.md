# USBH_MIDI v0.2.0

USB-MIDI class driver for Arduino [USB Host Shield 2.0 Library][UHS2]

You can convert USB MIDI keyboard  to legacy serial MIDI.

Please check [device list][wiki]

## Requirement
- Arduino IDE 1.6.x or lator
- USB Host Shield 2.0 Library 1.0.0 or lator

## How to install

Open the Arduino IDE and click to the "Sketch" menu and then "Include Library" > "Add .ZIP Library".  
And Navigate to the downloaded zip file's location and open it.

### for single device
> File->Examples->USBH_MIDI->USB_MIDI_converter

### for multiple device (with USB hub)
> File->Examples->USBH_MIDI->USB_MIDI_converter_multi

## API

- `uint8_t RecvData(uint8_t *outBuf)`
  Receive MIDI message (3 bytes)  
  return value is MIDI message length(0-3)

- `uint8_t RecvData(uint16_t *bytes_rcvd, uint8_t *dataptr)`
  Receive raw USB-MIDI Event Packets (each 4 bytes, upto 64 bytes)  
  `dataptr` must allocate 64bytes buffer.  
  return value is 0:Success, non-zero:Error(MAX3421E HRSLT) and bytes_rcvd is received USB packet length.  
  note: USB packet length is not necessarily the length of the MIDI message.

- `uint8_t SendData(uint8_t *dataptr, byte nCable=0)`
  Send MIDI message. You can set CableNumber(default=0).  
  return value is 0:Success, non-zero:Error(MAX3421E HRSLT)

- `uint8_t SendSysEx(uint8_t *dataptr, unsigned int datasize, byte nCable=0)`
  Send SysEx MIDI message. You can set CableNumber(default=0).  
  return value is 0:Success, non-zero:Error(MAX3421E HRSLT)  
  note: You must set first byte:0xf0 and last byte:0xf7

## ChangeLog
2015.09.06
* Compatible with USB Host Shield 2.0 Library 1.0.0 or lator.
* Compatible with Arduino IDE 1.6.0 or lator.
* Fix for less than 64 bytes USB packet devices
* SysEx message was broken since felis/USB_Host_Shield_2.0@45df706

2014.07.06
* Merge IOP_ArduinoMIDI branch into master
* Change class name to USBH_MIDI
* Rename the function RcvData to RecvData (Old name is still available)
* Fix examples for Arduino MIDI Library 4.2 compatibility
* Add SendSysEx()
* Add new example (eVY1_sample)

2014.03.23
* Fix examples for Arduino MIDI Library 4.0 compatibility and Leonardo

2013.12.20
* Fix multiple MIDI message problem.
* Add new example (USBH_MIDI_dump)

2013.11.05
* Removed all unnecessary includes. (latest UHS2 compatibility)
* Rename all example extensions to .ino

2013.08.28
* Fix MIDI Channel issue.

2013.08.18  
* RcvData() Return type is changed to uint8_t.
* Fix examples.

2012.06.22  
* Support MIDI out and loosen device check

2012.04.21  
* First release


## License

Copyright &copy; 2012-2015 Yuuichi Akagawa

Licensed under the [GNU General Public License v2.0][GPL2]

[GPL2]: http://www.gnu.org/licenses/gpl2.html
[wiki]: https://github.com/YuuichiAkagawa/USBH_MIDI/wiki
[UHS2]: https://github.com/felis/USB_Host_Shield_2.0
