/*
 *******************************************************************************
 * USB-MIDI to Legacy Serial MIDI converter with SysEx support
 * Copyright 2017-2021 Yuuichi Akagawa
 *
 * Idea from LPK25 USB-MIDI to Serial MIDI converter
 *   by Collin Cunningham - makezine.com, narbotic.com
 *
 * for use with USB Host Shield 2.0 from Circuitsathome.com
 * https://github.com/felis/USB_Host_Shield_2.0
 *******************************************************************************
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>
 *******************************************************************************
 */

#include <usbh_midi.h>
#include <usbhub.h>

#ifdef USBCON
#define _MIDI_SERIAL_PORT Serial1
#else
#define _MIDI_SERIAL_PORT Serial
#endif
//////////////////////////
// MIDI Pin assign
// 2 : GND
// 4 : +5V(Vcc) with 220ohm
// 5 : TX
//////////////////////////

USB Usb;
USBH_MIDI  Midi(&Usb);

void MIDI_poll();

void setup()
{
  _MIDI_SERIAL_PORT.begin(31250);

  if (Usb.Init() == -1) {
    while (1); //halt
  }//if (Usb.Init() == -1...
  delay( 200 );
}

void loop()
{
  Usb.Task();

  if ( Midi ) {
    MIDI_poll();
  }
  //delay(1ms) if you want
  //delayMicroseconds(1000);
}

// Poll USB MIDI Controler and send to serial MIDI
void MIDI_poll()
{
  uint8_t size;
  uint8_t outBuf[4];
  uint8_t sysexBuf[3];

  do {
    if ( (size = Midi.RecvRawData(outBuf)) > 0 ) {
      //MIDI Output
      uint8_t rc = Midi.extractSysExData(outBuf, sysexBuf);
      if ( rc != 0 ) { //SysEx
        _MIDI_SERIAL_PORT.write(sysexBuf, rc);
      } else { //SysEx
        _MIDI_SERIAL_PORT.write(outBuf + 1, size);
      }
    }
  } while (size > 0);
}
