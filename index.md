USBH_MIDI is USB-MIDI class driver for Arduino [USB Host Shield 2.0 Library][UHS2].  
You can convert USB MIDI keyboard  to legacy serial MIDI.  
Please check [device list][wiki]

## How to install
USBH_MIDI is included in [USB Host Shield 2.0 Library][UHS2].

### for single device
> File->Examples->USB Host Shield library 2.0->USBH_MIDI->USB_MIDI_converter

### for multiple device (with USB hub)
> File->Examples->USB Host Shield library 2.0->USBH_MIDI->USB_MIDI_converter_multi

## API
### `uint8_t RecvData(uint8_t *outBuf)`
  Receive MIDI message (3 bytes)  
  return value is MIDI message length(0-3)

***

### `uint8_t RecvData(uint16_t *bytes_rcvd, uint8_t *dataptr)`
  Receive raw USB-MIDI Event Packets (each 4 bytes, upto 64 bytes).  
  `dataptr` must allocate 64bytes buffer.  
  return value is 0:Success, non-zero:Error(MAX3421E HRSLT) and bytes_rcvd is received USB packet length.  
  note: USB packet length is not necessarily the length of the MIDI message.

***

### `uint8_t SendData(uint8_t *dataptr, byte nCable=0)`
  Send MIDI message. You can set CableNumber(default=0).  
  return value is 0:Success, non-zero:Error(MAX3421E HRSLT)

***

### `uint8_t SendSysEx(uint8_t *dataptr, unsigned int datasize, byte nCable=0)`
  Send SysEx MIDI message. You can set CableNumber(default=0).  
  return value is 0:Success, non-zero:Error(MAX3421E HRSLT)  
  note: You must set first byte:0xf0 and last byte:0xf7

***
## License
Copyright &copy; 2012-2016 Yuuichi Akagawa (@YuuichiAkagawa)

[wiki]: https://github.com/YuuichiAkagawa/USBH_MIDI/wiki
[UHS2]: https://github.com/felis/USB_Host_Shield_2.0
