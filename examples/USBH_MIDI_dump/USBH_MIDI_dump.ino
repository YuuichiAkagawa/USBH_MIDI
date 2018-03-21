/*
 *******************************************************************************
 * USB-MIDI dump utility
 * Copyright 2013-2017 Yuuichi Akagawa
 *
 * for use with USB Host Shield 2.0 from Circuitsathome.com
 * https://github.com/felis/USB_Host_Shield_2.0
 *
 * This is sample program. Do not expect perfect behavior.
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

// Satisfy the IDE, which needs to see the include statment in the ino too.
#ifdef dobogusinclude
#include <spi4teensy3.h>
#include <SPI.h>
#endif

USB Usb;
//USBHub Hub(&Usb);
USBH_MIDI  Midi(&Usb);

void MIDI_poll();

uint16_t pid, vid;

void setup()
{
  vid = pid = 0;
  Serial.begin(115200);

  //Workaround for non UHS2.0 Shield
  pinMode(7, OUTPUT);
  digitalWrite(7, HIGH);

  if (Usb.Init() == -1) {
    while (1); //halt
  }//if (Usb.Init() == -1...
  delay( 200 );
}

void loop()
{
  Usb.Task();
  //uint32_t t1 = (uint32_t)micros();
  if ( Midi )
  {
    MIDI_poll();
  }
}

// Poll USB MIDI Controler and send to serial MIDI
void MIDI_poll()
{
  char buf[20];
  uint8_t bufMidi[64];
  uint16_t  rcvd;

  if (Midi.idVendor() != vid || Midi.idProduct() != pid) {
    vid = Midi.idVendor();
    pid = Midi.idProduct();
    sprintf(buf, "VID:%04X, PID:%04X", vid, pid);
    Serial.println(buf);
  }
  if (Midi.RecvData( &rcvd,  bufMidi) == 0 ) {
    sprintf(buf, "%08lX: ", (uint32_t)millis());
    Serial.print(buf);
    Serial.print(rcvd);
    Serial.print(':');
    for (int i = 0; i < 64; i++) {
      sprintf(buf, " %02X", bufMidi[i]);
      Serial.print(buf);
    }
    Serial.println("");
  }
}

