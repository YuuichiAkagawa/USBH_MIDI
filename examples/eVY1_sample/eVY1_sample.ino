/*
 *******************************************************************************
 * eVY1 Shield sample - Say 'Konnichiwa'
 * Copyright 2014-2015 Yuuichi Akagawa
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

USB  Usb;
//USBHub Hub(&Usb);
USBH_MIDI  Midi(&Usb);

void MIDI_poll();
void noteOn(uint8_t note);
void noteOff(uint8_t note);

uint16_t pid, vid;
uint8_t exdata[] ={
  0xf0, 0x43, 0x79, 0x09, 0x00, 0x50, 0x10, 
  'k',' ','o',',',    //Ko
  'N','\\',',',       //N
  'J',' ', 'i', ',',  //Ni
  't','S',' ','i',',',//Chi
  'w',' ','a',        //Wa
  0x00, 0xf7
};

void setup()
{
  vid = pid = 0;
  Serial.begin(115200);

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
    noteOn(0x3f);
    delay(400);
    noteOff(0x3f);
    delay(100);
  }
}

// Poll USB MIDI Controler
void MIDI_poll()
{
    byte inBuf[ 3 ];
    
    //first call?
    if(Midi.vid != vid || Midi.pid != pid){
      vid = Midi.vid; pid = Midi.pid;
      Midi.SendSysEx(exdata, sizeof(exdata));
//      Midi.SendData(exdata);
      delay(500);
    }
    uint8_t size = Midi.RecvData(inBuf);
}

//note On
void noteOn(uint8_t note)
{
  uint8_t buf[3];
  buf[0] = 0x90;
  buf[1] = note;
  buf[2] = 0x7f;
  Midi.SendData(buf);
}

//note Off
void noteOff(uint8_t note)
{
  uint8_t buf[3];
  buf[0] = 0x80;
  buf[1] = note;
  buf[2] = 0x00;
  Midi.SendData(buf);
}
