/*
 *******************************************************************************
 * USB-MIDI class driver for USB Host Shield 2.0 Library
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

#if !defined(_USBH_MIDI_H_)
#define _USBH_MIDI_H_
#include <inttypes.h>
#include <avr/pgmspace.h>
#include "avrpins.h"
#include "max3421e.h"
#include "usbhost.h"
#include "usb_ch9.h"
#include "Usb.h"
#include "confdescparser.h"

#if defined(ARDUINO) && ARDUINO >=100
#include "Arduino.h"
#else
#include <WProgram.h>
#endif

#define MIDI_MAX_ENDPOINTS 3 //endpoint 0, bulk_IN, bulk_OUT
#define USB_SUBCLASS_MIDISTREAMING 3
#define DESC_BUFF_SIZE        256

class MIDI;

class MIDI : public USBDeviceConfig
{
protected:
  static const uint8_t	epDataInIndex;			// DataIn endpoint index
  static const uint8_t	epDataOutIndex;			// DataOUT endpoint index

  /* mandatory members */
  USB      *pUsb;
  uint8_t  bAddress;
  uint8_t  bConfNum;    // configuration number
  uint8_t  bNumEP;      // total number of EP in the configuration
  bool     bPollEnable;
  /* Endpoint data structure */
  EpInfo  epInfo[MIDI_MAX_ENDPOINTS];

  void parseConfigDescr(byte addr, byte conf);
#ifdef DEBUG
  void PrintEndpointDescriptor( const USB_ENDPOINT_DESCRIPTOR* ep_ptr );
#endif
public:
  uint16_t pid, vid;
  MIDI(USB *p);
  // Methods for recieving and sending data
  uint8_t RcvData(uint16_t *bytes_rcvd, uint8_t *dataptr);
  bool    RcvData(uint8_t *outBuf);
  // USBDeviceConfig implementation
  virtual uint8_t Init(uint8_t parent, uint8_t port, bool lowspeed);
  virtual uint8_t Release();
  virtual uint8_t Poll(){}; //not implemented
  virtual uint8_t GetAddress() { return bAddress; };
};

#endif //_USBH_MIDI_H_
