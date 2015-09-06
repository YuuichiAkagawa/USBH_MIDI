/*
 *******************************************************************************
 * Legacy Serial MIDI and USB Host bidirectional converter
 * Copyright 2013-2015 Yuuichi Akagawa
 *
 * for use with USB Host Shield 2.0 from Circuitsathome.com
 * https://github.com/felis/USB_Host_Shield_2.0
 * and Arduino MIDI library
 * http://playground.arduino.cc/Main/MIDILibrary
 *
 * Note:
 *  If you want use with Leonardo, you must choose Arduino MIDI library v4.0 or higher.
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

#include <MIDI.h>
#include <usbh_midi.h>
#include <usbhub.h>

// Satisfy the IDE, which needs to see the include statment in the ino too.
#ifdef dobogusinclude
#include <spi4teensy3.h>
#include <SPI.h>
#endif

//Arduino MIDI library v4.2 compatibility
#ifdef MIDI_CREATE_DEFAULT_INSTANCE
MIDI_CREATE_DEFAULT_INSTANCE();
#endif
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

USB  Usb;
USBH_MIDI  Midi(&Usb);

void MIDI_poll();
void doDelay(unsigned long t1, unsigned long t2, unsigned long delayTime);

void setup()
{
  MIDI.begin(MIDI_CHANNEL_OMNI);

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
  unsigned long t1;
  uint8_t msg[4];

  Usb.Task();
  t1 = micros();
  if( Usb.getUsbTaskState() == USB_STATE_RUNNING )
  {
    MIDI_poll();
    if (MIDI.read()) {
       msg[0] = MIDI.getType();
       if( msg[0] == 0xf0 ) { //SysEX
         //TODO
         //SysEx implementation is not yet.
       }else{
         msg[1] = MIDI.getData1();
         msg[2] = MIDI.getData2();
         Midi.SendData(msg, 0);
       }
    }
  }
  //delay(1ms)
  doDelay(t1, micros(), 1000);
}

// Poll USB MIDI Controler and send to serial MIDI
void MIDI_poll()
{
    byte outBuf[ 3 ];
    uint8_t size;

    if( (size=Midi.RecvData(outBuf)) > 0 ){
      //MIDI Output
      _MIDI_SERIAL_PORT.write(outBuf, size);
    }
}

// Delay time (max 16383 us)
void doDelay(unsigned long t1, unsigned long t2, unsigned long delayTime)
{
    unsigned long t3;

    if( t1 > t2 ){
      t3 = (4294967295 - t1 + t2);
    }else{
      t3 = t2 - t1;
    }

    if( t3 < delayTime ){
      delayMicroseconds(delayTime - t3);
    }
}
