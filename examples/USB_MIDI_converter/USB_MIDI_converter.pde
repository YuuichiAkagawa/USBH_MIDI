/*
 *******************************************************************************
 * USB-MIDI to Legacy Serial MIDI converter
 * Copyright 2012 Yuuichi Akagawa
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

#include <Usb.h>
#include <usbh_midi.h>

//////////////////////////
// MIDI Pin assign
// 2 : GND
// 4 : +5V(Vcc) with 220ohm
// 5 : TX
//////////////////////////

USB  Usb;
MIDI  Midi(&Usb);

void setup()
{
  Serial.begin(31500);

  //Workaround for non UHS2.0 Shield 
  pinMode(7,OUTPUT);
  digitalWrite(7,HIGH);

  if (Usb.Init() == -1) {
    while(1); //halt
  }//if (Usb.Init() == -1...
  delay( 200 );
}

void loop()
{
  Usb.Task();
  if( Usb.getUsbTaskState() == USB_STATE_RUNNING )
  {
    MIDI_poll();
  }
  delay(1);
}

// Poll USB MIDI Controler and send to serial MIDI
void MIDI_poll()
{
    byte outBuf[ 3 ];
    if( Midi.RcvData(outBuf) == true ){
      //MIDI Output
      Serial.write(outBuf, 3);
    }
}
