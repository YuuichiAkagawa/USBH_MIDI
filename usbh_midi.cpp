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

#include "usbh_midi.h"
//////////////////////////
// MIDI MESAGES 
// midi.org/techspecs/
//////////////////////////
// STATUS BYTES
// 0x80 == noteOn
// 0x90 == noteOff
// 0xA0 == afterTouch
// 0xB0 == controlChange
//////////////////////////
//DATA BYTE 1
// note# == (0-127)
// or
// control# == (0-119)
//////////////////////////
// DATA BYTE 2
// velocity == (0-127)
// or
// controlVal == (0-127)
//////////////////////////

const uint8_t	MIDI::epDataInIndex  = 1;
const uint8_t	MIDI::epDataOutIndex = 2;

MIDI::MIDI(USB *p)
{
  pUsb = p;
  bAddress = 0;
  bNumEP = 1;
  bPollEnable  = false;

  // initialize endpoint data structures
  for(uint8_t i=0; i<MIDI_MAX_ENDPOINTS; i++) {
    epInfo[i].epAddr        = 0;
    epInfo[i].maxPktSize  = (i) ? 0 : 8;
    epInfo[i].epAttribs     = 0;
    epInfo[i].bmNakPower  = (i) ? USB_NAK_NOWAIT : 4;
  }
  // register in USB subsystem
  if (pUsb) {
    pUsb->RegisterDeviceClass(this);
  }
}

/* Connection initialization of an MIDI Device */
uint8_t MIDI::Init(uint8_t parent, uint8_t port, bool lowspeed)
{
  uint8_t    buf[sizeof(USB_DEVICE_DESCRIPTOR)];
  uint8_t    rcode;
  UsbDevice  *p = NULL;
  EpInfo     *oldep_ptr = NULL;
  uint8_t    num_of_conf;  // number of configurations
  
  // get memory address of USB device address pool
  AddressPool &addrPool = pUsb->GetAddressPool();
//  USBTRACE("\rMIDI Init\r\n");

  // check if address has already been assigned to an instance
  if (bAddress) {
    return USB_ERROR_CLASS_INSTANCE_ALREADY_IN_USE;
  }
  // Get pointer to pseudo device with address 0 assigned
  p = addrPool.GetUsbDevicePtr(0);
  if (!p) {
    return USB_ERROR_ADDRESS_NOT_FOUND_IN_POOL;
  }
  if (!p->epinfo) {
    return USB_ERROR_EPINFO_IS_NULL;
  }

  // Save old pointer to EP_RECORD of address 0
  oldep_ptr = p->epinfo;

  // Temporary assign new pointer to epInfo to p->epinfo in order to avoid toggle inconsistence
  p->epinfo = epInfo;
  p->lowspeed = lowspeed;

  // Get device descriptor
  rcode = pUsb->getDevDescr( 0, 0, sizeof(USB_DEVICE_DESCRIPTOR), (uint8_t*)buf );

  // Restore p->epinfo
  p->epinfo = oldep_ptr;

  if( rcode ){ 
    goto FailGetDevDescr;
  }

  // Allocate new address according to device class
  bAddress = addrPool.AllocAddress(parent, false, port);
  if (!bAddress) {
    return USB_ERROR_OUT_OF_ADDRESS_SPACE_IN_POOL;
  }

  // Extract Max Packet Size from device descriptor
  epInfo[0].maxPktSize = (uint8_t)((USB_DEVICE_DESCRIPTOR*)buf)->bMaxPacketSize0; 

  // Assign new address to the device
  rcode = pUsb->setAddr( 0, 0, bAddress );
  if (rcode) {
    p->lowspeed = false;
    addrPool.FreeAddress(bAddress);
    bAddress = 0;
    return rcode;
  }//if (rcode...
//  USBTRACE2("Addr:", bAddress);
  
  p->lowspeed = false;

  //get pointer to assigned address record
  p = addrPool.GetUsbDevicePtr(bAddress);
  if (!p) {
    return USB_ERROR_ADDRESS_NOT_FOUND_IN_POOL;
  }
  p->lowspeed = lowspeed;

  num_of_conf = ((USB_DEVICE_DESCRIPTOR*)buf)->bNumConfigurations;

  // Assign epInfo to epinfo pointer
  rcode = pUsb->setEpInfoEntry(bAddress, 1, epInfo);
  if (rcode) {
//    USBTRACE("setEpInfoEntry failed");
    goto FailSetDevTblEntry;
  }

//  USBTRACE2("NC:", num_of_conf);

  for (uint8_t i=0; i<num_of_conf; i++) {
    ConfigDescParser<USB_CLASS_AUDIO, USB_SUBCLASS_MIDISTREAMING, 0, CP_MASK_COMPARE_ALL>	confDescrParser(this);
    rcode = pUsb->getConfDescr(bAddress, 0, i, &confDescrParser);
    if (bNumEP > 1)
      break;
  } // for
//  USBTRACE2("NumEP:", bNumEP);

  // Assign epInfo to epinfo pointer
  rcode = pUsb->setEpInfoEntry(bAddress, bNumEP, epInfo);
//  USBTRACE2("Conf:", bConfNum);

  // Set Configuration Value
  rcode = pUsb->setConf(bAddress, 0, bConfNum);
  if (rcode) {
    goto FailSetConfDescr;
  }
//  USBTRACE("Init done.");
  bPollEnable = true;
  return 0;
FailGetDevDescr:
FailSetDevTblEntry:
FailSetConfDescr:
  Release();
  return rcode;
}

/* Extracts bulk-IN and bulk-OUT endpoint information from config descriptor */
void MIDI::EndpointXtract(uint8_t conf, uint8_t iface, uint8_t alt, uint8_t proto, const USB_ENDPOINT_DESCRIPTOR *pep) 
{
  bConfNum = conf;
  uint8_t index;
  if ((pep->bmAttributes & 0x02) == 2) {
    index = ((pep->bEndpointAddress & 0x80) == 0x80) ? epDataInIndex : epDataOutIndex;
  }
  // Fill in the endpoint info structure
  epInfo[index].epAddr		= (pep->bEndpointAddress & 0x0F);
  epInfo[index].maxPktSize	= (uint8_t)pep->wMaxPacketSize;
  bNumEP ++;
//  PrintEndpointDescriptor(pep);
}

/* Performs a cleanup after failed Init() attempt */
uint8_t MIDI::Release()
{
  pUsb->GetAddressPool().FreeAddress(bAddress);
  bNumEP       = 1;		//must have to be reset to 1	
  bAddress     = 0;
  bPollEnable  = false;
  return 0;
}

/* Receive data from MIDI device */
uint8_t MIDI::RcvData(uint16_t *bytes_rcvd, uint8_t *dataptr)
{
  bytes_rcvd[0] = (uint16_t)epInfo[epDataInIndex].maxPktSize;
  return pUsb->inTransfer(bAddress, epInfo[epDataInIndex].epAddr, bytes_rcvd, dataptr);
}

/* Receive data from MIDI device */
bool MIDI::RcvData(uint8_t *outBuf)
{
  byte i, codeIndexNumber;
  byte rcode = 0;     //return code
  uint16_t  rcvd;
  uint8_t rcvbuf[64];

  if( bPollEnable == false ) return false;

  rcode = RcvData( &rcvd,  rcvbuf);
  if( rcode != 0 ) {
    return false;
  }
  
  //if all data is zero, no valid data received.
  if( rcvbuf[0] == 0 && rcvbuf[1] == 0 && rcvbuf[2] == 0 && rcvbuf[3] == 0 ) {
    return false;
  }
  outBuf[0] = rcvbuf[1];
  outBuf[1] = rcvbuf[2];
  outBuf[2] = rcvbuf[3];
  return true;
}

