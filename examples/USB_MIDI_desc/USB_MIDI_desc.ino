/*
 *******************************************************************************
 * USB-MIDI descriptor dump utility
 *  modified from USB_desc.ino
 * 
 * for use with USB Host Shield 2.0 from Circuitsathome.com
 * https://github.com/felis/USB_Host_Shield_2.0
 *******************************************************************************
 */

#include <usbhub.h>

USB     Usb;
//USBHub  Hub1(&Usb);
//USBHub  Hub2(&Usb);
//USBHub  Hub3(&Usb);
//USBHub  Hub4(&Usb);
//USBHub  Hub5(&Usb);
//USBHub  Hub6(&Usb);
//USBHub  Hub7(&Usb);

#define BUFFER_SIZE 512

//MS Class-Specific Interface Descriptor Subtypes
#define MSIF_MS_DESCRIPTOR_UNDEFINED (0x00)
#define MSIF_MS_HEADER               (0x01)
#define MSIF_MIDI_IN_JACK            (0x02)
#define MSIF_MIDI_OUT_JACK           (0x03)
#define MSIF_ELEMENT                 (0x04)

//MS Class-Specific Endpoint Descriptor Subtypes
#define MSEP_DESCRIPTOR_UNDEFINED    (0x00)
#define MSEP_MS_GENERAL              (0x01)

//MS MIDI IN and OUT Jack types
#define MSJT_JACK_TYPE_UNDEFINED     (0x00)
#define MSJT_EMBEDDED                (0x01)
#define MSJT_EXTERNAL                (0x02)

/* descriptor type  */
#define USB_DESCRIPTOR_CS_INTERFACE (0x24)
#define USB_DESCRIPTOR_CS_ENDPOINT (0x25)

#define LOBYTE(x) ((char*)(&(x)))[0]
#define HIBYTE(x) ((char*)(&(x)))[1]

const char Gen_Error_str[] PROGMEM = "\r\nRequest error. Error code:\t";
const char Dev_Header_str[] PROGMEM = "\r\nDevice descriptor: ";
const char Dev_Length_str[] PROGMEM = "\r\nDescriptor Length:\t";
const char Dev_Type_str[] PROGMEM = "\r\nDescriptor type:\t";
const char Dev_Version_str[] PROGMEM = "\r\nUSB version:\t\t";
const char Dev_Class_str[] PROGMEM = "\r\nDevice class:\t\t";
const char Dev_Subclass_str[] PROGMEM = "\r\nDevice Subclass:\t";
const char Dev_Protocol_str[] PROGMEM = "\r\nDevice Protocol:\t";
const char Dev_Pktsize_str[] PROGMEM = "\r\nMax.packet size:\t";
const char Dev_Vendor_str[] PROGMEM = "\r\nVendor  ID:\t\t";
const char Dev_Product_str[] PROGMEM = "\r\nProduct ID:\t\t";
const char Dev_Revision_str[] PROGMEM = "\r\nRevision ID:\t\t";
const char Dev_Mfg_str[] PROGMEM = "\r\nMfg.string index:\t";
const char Dev_Prod_str[] PROGMEM = "\r\nProd.string index:\t";
const char Dev_Serial_str[] PROGMEM = "\r\nSerial number index:\t";
const char Dev_Nconf_str[] PROGMEM = "\r\nNumber of conf.:\t";
const char Conf_Trunc_str[] PROGMEM = "Total length truncated to 256 bytes";
const char Conf_Header_str[] PROGMEM = "\r\nConfiguration descriptor:";
const char Conf_Totlen_str[] PROGMEM = "\r\nTotal length:\t\t";
const char Conf_Nint_str[] PROGMEM = "\r\nNum.intf:\t\t";
const char Conf_Value_str[] PROGMEM = "\r\nConf.value:\t\t";
const char Conf_String_str[] PROGMEM = "\r\nConf.string:\t\t";
const char Conf_Attr_str[] PROGMEM = "\r\nAttr.:\t\t\t";
const char Conf_Pwr_str[] PROGMEM = "\r\nMax.pwr:\t\t";
const char Int_Header_str[] PROGMEM = "\r\n\r\nInterface descriptor:";
const char Int_Number_str[] PROGMEM = "\r\nIntf.number:\t\t";
const char Int_Alt_str[] PROGMEM = "\r\nAlt.:\t\t\t";
const char Int_Endpoints_str[] PROGMEM = "\r\nEndpoints:\t\t";
const char Int_Class_str[] PROGMEM = "\r\nIntf. Class:\t\t";
const char Int_Subclass_str[] PROGMEM = "\r\nIntf. Subclass:\t\t";
const char Int_Protocol_str[] PROGMEM = "\r\nIntf. Protocol:\t\t";
const char Int_String_str[] PROGMEM = "\r\nIntf.string:\t\t";
const char End_Header_str[] PROGMEM = "\r\n\r\nEndpoint descriptor:";
const char End_Address_str[] PROGMEM = "\r\nEndpoint address:\t";
const char End_Attr_str[] PROGMEM = "\r\nAttr.:\t\t\t";
const char End_Pktsize_str[] PROGMEM = "\r\nMax.pkt size:\t\t";
const char End_Interval_str[] PROGMEM = "\r\nPolling interval:\t";
const char End_Attr_control_str[] PROGMEM = "(Control)";
const char End_Attr_iso_str[] PROGMEM = "(Isochronous)";
const char End_Attr_bulk_str[] PROGMEM = "(Bulk)";
const char End_Attr_int_str[] PROGMEM = "(Interrupt)";
const char Unk_Header_str[] PROGMEM = "\r\nUnknown descriptor:";
const char Unk_Length_str[] PROGMEM = "\r\nLength:\t\t";
const char Unk_Type_str[] PROGMEM = "\r\nType:\t\t";
const char Unk_Contents_str[] PROGMEM = "\r\nContents:\t";
const char Int_AC_str[] PROGMEM = "\r\n\r\n<<<AUDIO_CONTROL>>>";
const char Int_AS_str[] PROGMEM = "\r\n\r\n<<<AUDIO_STREAMING>>>";
const char Int_MS_str[] PROGMEM = "\r\n\r\n<<<MIDIStreaming>>>";
const char Int_CS_Header_str[] PROGMEM = "\r\n\r\nMS Interface descriptor";
const char End_CS_Header_str[] PROGMEM = "\r\n\r\nMS Endpoint descriptor";
const char Int_CS_Type_str[] PROGMEM = "\r\nUSB_DESCRIPTOR_CS_INTERFACE :\t";
const char End_CS_Type_str[] PROGMEM = "\r\nUSB_DESCRIPTOR_CS_ENDPOINT :\t";
const char CS_MS_Version_str[] PROGMEM = "\r\nMIDIStreaming SubClass Specification Release number: ";
const char CS_MS_Length_str[] PROGMEM = "\r\nwTotalLength:\t\t";
const char CS_MS_Subtype_str[] PROGMEM = "\r\nDescriptorSubtype: ";
const char CS_MS_Jacktype_str[] PROGMEM = "\r\nJackType:\t";
const char CS_MIDI_JACKID_str[] PROGMEM = "\r\nJackID:\t\t";
const char CS_MIDI_NrInputPins_str[] PROGMEM = "\r\nNrInputPins:\t";
const char CS_Subtype_UNDEF_str[] PROGMEM = "Undefined";
const char CS_Subtype_MS_HEADER_str[] PROGMEM = "MS_HEADER";
const char CS_Subtype_MIDI_IN_JACK_str[] PROGMEM = "MIDI_IN_JACK";
const char CS_Subtype_MIDI_OUT_JACK_str[] PROGMEM = "MIDI_OUT_JACK";
const char CS_Subtype_ELEMENT_str[] PROGMEM = "ELEMENT";
const char CS_Subtype_MIDI_JACK_TYPE_UNDEFINED_str[] PROGMEM = "(Undefined)";
const char CS_Subtype_MIDI_JACK_TYPE_EMBEDDED_str[] PROGMEM = "(Embedded)";
const char CS_Subtype_MIDI_JACK_TYPE_EXTERNAL_str[] PROGMEM = "(External)";
const char CS_Subtype_MS_GENERAL_str[] PROGMEM = "MS_GENERAL";
const char CS_MIDI_NrEmbJacks_str[] PROGMEM = "\r\nbNumEmbMIDIJack: ";
const char CRLF_str[] PROGMEM = "\r\n";

typedef struct {
  uint8_t bLength; // Length of this descriptor.
  uint8_t bDescriptorType; // INTERFACE descriptor type (USB_DESCRIPTOR_INTERFACE).
  uint8_t bDescriptorSubtype; // INTERFACE descriptor sub type
  uint16_t bcdMSC; //
  uint16_t wTotalLength; //
} __attribute__((packed)) USB_MS_INTERFACE_DESCRIPTOR;

typedef struct {
  uint8_t bLength; // Length of this descriptor.
  uint8_t bDescriptorType; // INTERFACE descriptor type (USB_DESCRIPTOR_INTERFACE).
  uint8_t bDescriptorSubtype; // INTERFACE descriptor sub type
  uint8_t bJackType; //EMBEDDED or EXTERNAL
  uint8_t bJackID; //Constant uniquely identifying the MIDI IN Jack within the USB-MIDI function
  uint8_t iJack; //Index of a string descriptor, describing the MIDI IN Jack.
} __attribute__((packed)) USB_MS_INTERFACE_MIDI_IN_JACK_DESCRIPTOR;

typedef struct {
  uint8_t bLength; // Length of this descriptor.
  uint8_t bDescriptorType; // INTERFACE descriptor type (USB_DESCRIPTOR_INTERFACE).
  uint8_t bDescriptorSubtype; // INTERFACE descriptor sub type
  uint8_t bJackType; //EMBEDDED or EXTERNAL
  uint8_t bJackID; //Constant uniquely identifying the MIDI IN Jack within the USB-MIDI function
  uint8_t bNrInputPins; //
  uint8_t *baSources; //
} __attribute__((packed)) USB_MS_INTERFACE_MIDI_OUT_JACK_DESCRIPTOR;

typedef struct {
  uint8_t bLength; // Length of this descriptor.
  uint8_t bDescriptorType; // ENDPOINT descriptor type (USB_DESCRIPTOR_ENDPOINT).
  uint8_t bDescriptorSubtype; // ENDPOINT descriptor sub type
  uint8_t bNumEmbMIDIJack; // Number of embedded MIDI IN Jacks.
  uint8_t *BaAssocJackIDs; // ID of the Embedded MIDI IN Jack.
} __attribute__((packed)) USB_MS_ENDPOINT_DESCRIPTOR;

void PrintAllAddresses(UsbDevice *pdev)
{
  UsbDeviceAddress adr;
  adr.devAddress = pdev->address.devAddress;
  Serial.print("\r\nAddr:");
  Serial.print(adr.devAddress, HEX);
  Serial.print("(");
  Serial.print(adr.bmHub, HEX);
  Serial.print(".");
  Serial.print(adr.bmParent, HEX);
  Serial.print(".");
  Serial.print(adr.bmAddress, HEX);
  Serial.println(")");
}

void PrintAddress(uint8_t addr)
{
  UsbDeviceAddress adr;
  adr.devAddress = addr;
  Serial.print("\r\nADDR:\t");
  Serial.println(adr.devAddress, HEX);
  Serial.print("DEV:\t");
  Serial.println(adr.bmAddress, HEX);
  Serial.print("PRNT:\t");
  Serial.println(adr.bmParent, HEX);
  Serial.print("HUB:\t");
  Serial.println(adr.bmHub, HEX);
}

void setup()
{
  Serial.begin( 115200 );
#if !defined(__MIPSEL__)
  while (!Serial); // Wait for serial port to connect - used on Leonardo, Teensy and other boards with built-in USB CDC serial connection
#endif
  Serial.println("Start");

  if (Usb.Init() == -1)
    Serial.println("OSC did not start.");

  delay( 200 );
}

uint8_t getdevdescr( uint8_t addr, uint8_t &num_conf );

void PrintDescriptors(uint8_t addr)
{
  uint8_t rcode = 0;
  uint8_t num_conf = 0;

  rcode = getdevdescr( (uint8_t)addr, num_conf );
  if ( rcode )
  {
    printProgStr(Gen_Error_str);
    print_hex( rcode, 8 );
  }
  Serial.print("\r\n");

  for (int i = 0; i < num_conf; i++)
  {
    rcode = getconfdescr( addr, i );                 // get configuration descriptor
    if ( rcode )
    {
      printProgStr(Gen_Error_str);
      print_hex(rcode, 8);
    }
    Serial.println("\r\n");
  }
}

void PrintAllDescriptors(UsbDevice *pdev)
{
  Serial.println("\r\n");
  print_hex(pdev->address.devAddress, 8);
  Serial.println("\r\n--");
  getallstrdescr(pdev->address.devAddress);
  PrintDescriptors( pdev->address.devAddress );
}

void loop()
{
  Usb.Task();

  if ( Usb.getUsbTaskState() == USB_STATE_RUNNING )
  {
    Usb.ForEachUsbDevice(&PrintAllDescriptors);
    Usb.ForEachUsbDevice(&PrintAllAddresses);

    while ( 1 ) { // stop
#ifdef ESP8266
      yield(); // needed in order to reset the watchdog timer on the ESP8266
#endif
    }
  }
}

uint8_t getdevdescr( uint8_t addr, uint8_t &num_conf )
{
  USB_DEVICE_DESCRIPTOR buf;
  uint8_t rcode;
  rcode = Usb.getDevDescr( addr, 0, 0x12, ( uint8_t *)&buf );
  if ( rcode ) {
    return ( rcode );
  }
  printProgStr(Dev_Header_str);
  printProgStr(Dev_Length_str);
  print_hex( buf.bLength, 8 );
  printProgStr(Dev_Type_str);
  print_hex( buf.bDescriptorType, 8 );
  printProgStr(Dev_Version_str);
  print_hex( buf.bcdUSB, 16 );
  printProgStr(Dev_Class_str);
  print_hex( buf.bDeviceClass, 8 );
  printProgStr(Dev_Subclass_str);
  print_hex( buf.bDeviceSubClass, 8 );
  printProgStr(Dev_Protocol_str);
  print_hex( buf.bDeviceProtocol, 8 );
  printProgStr(Dev_Pktsize_str);
  print_hex( buf.bMaxPacketSize0, 8 );
  printProgStr(Dev_Vendor_str);
  print_hex( buf.idVendor, 16 );
  printProgStr(Dev_Product_str);
  print_hex( buf.idProduct, 16 );
  printProgStr(Dev_Revision_str);
  print_hex( buf.bcdDevice, 16 );
  printProgStr(Dev_Mfg_str);
  print_hex( buf.iManufacturer, 8 );
  printProgStr(Dev_Prod_str);
  print_hex( buf.iProduct, 8 );
  printProgStr(Dev_Serial_str);
  print_hex( buf.iSerialNumber, 8 );
  printProgStr(Dev_Nconf_str);
  print_hex( buf.bNumConfigurations, 8 );
  num_conf = buf.bNumConfigurations;
  return ( 0 );
}

void printhubdescr(uint8_t *descrptr)
{
  HubDescriptor  *pHub = (HubDescriptor*) descrptr;
  uint8_t        len = *((uint8_t*)descrptr);

  printProgStr(PSTR("\r\n\r\nHub Descriptor:\r\n"));
  printProgStr(PSTR("bDescLength:\t\t"));
  Serial.println(pHub->bDescLength, HEX);

  printProgStr(PSTR("bDescriptorType:\t"));
  Serial.println(pHub->bDescriptorType, HEX);

  printProgStr(PSTR("bNbrPorts:\t\t"));
  Serial.println(pHub->bNbrPorts, HEX);

  printProgStr(PSTR("LogPwrSwitchMode:\t"));
  Serial.println(pHub->LogPwrSwitchMode, BIN);

  printProgStr(PSTR("CompoundDevice:\t\t"));
  Serial.println(pHub->CompoundDevice, BIN);

  printProgStr(PSTR("OverCurrentProtectMode:\t"));
  Serial.println(pHub->OverCurrentProtectMode, BIN);

  printProgStr(PSTR("TTThinkTime:\t\t"));
  Serial.println(pHub->TTThinkTime, BIN);

  printProgStr(PSTR("PortIndicatorsSupported:"));
  Serial.println(pHub->PortIndicatorsSupported, BIN);

  printProgStr(PSTR("Reserved:\t\t"));
  Serial.println(pHub->Reserved, HEX);

  printProgStr(PSTR("bPwrOn2PwrGood:\t\t"));
  Serial.println(pHub->bPwrOn2PwrGood, HEX);

  printProgStr(PSTR("bHubContrCurrent:\t"));
  Serial.println(pHub->bHubContrCurrent, HEX);

  for (uint8_t i = 7; i < len; i++)
    print_hex(descrptr[i], 8);
}

uint8_t getconfdescr( uint8_t addr, uint8_t conf )
{
  uint8_t buf[ BUFFER_SIZE ];
  uint8_t* buf_ptr = buf;
  uint8_t rcode;
  uint8_t descr_length;
  uint8_t descr_type;
  uint16_t total_length;
  bool isMidiStreaming = false;
  rcode = Usb.getConfDescr( addr, 0, 4, conf, buf );  //get total length
  LOBYTE( total_length ) = buf[ 2 ];
  HIBYTE( total_length ) = buf[ 3 ];
  if ( total_length > BUFFER_SIZE ) {   //check if total length is larger than buffer
    printProgStr(Conf_Trunc_str);
    total_length = BUFFER_SIZE;
  }
  rcode = Usb.getConfDescr( addr, 0, total_length, conf, buf ); //get the whole descriptor
  while ( buf_ptr < buf + total_length ) { //parsing descriptors
    descr_length = *( buf_ptr );
    descr_type = *( buf_ptr + 1 );
    switch ( descr_type ) {
      case ( USB_DESCRIPTOR_CONFIGURATION ):
        printconfdescr( buf_ptr );
        break;
      case ( USB_DESCRIPTOR_INTERFACE ):
        isMidiStreaming = printintfdescr( buf_ptr );
        break;
      case ( USB_DESCRIPTOR_ENDPOINT ):
        printepdescr( buf_ptr );
        break;
      case 0x29:
        printhubdescr( buf_ptr );
        break;
      case ( USB_DESCRIPTOR_CS_INTERFACE ):
        if ( isMidiStreaming ) {
          printmsifdescr( buf_ptr );
        } else {
          printunkdescr( buf_ptr );
        }
        break;
      case ( USB_DESCRIPTOR_CS_ENDPOINT ):
        if ( isMidiStreaming ) {
          printmsepdescr( buf_ptr );
        }  else {
          printunkdescr( buf_ptr );
        }
        break;
      default:
        printunkdescr( buf_ptr );
        break;
    }//switch( descr_type
    buf_ptr = ( buf_ptr + descr_length );    //advance buffer pointer
  }//while( buf_ptr <=...
  return ( rcode );
}

// function to get all string descriptors
uint8_t getallstrdescr(uint8_t addr)
{
  uint8_t rcode = 0;
  Usb.Task();
  if ( Usb.getUsbTaskState() >= USB_STATE_CONFIGURING ) { // state configuring or higher
    USB_DEVICE_DESCRIPTOR buf;
    rcode = Usb.getDevDescr( addr, 0, DEV_DESCR_LEN, ( uint8_t *)&buf );
    if ( rcode ) {
      return ( rcode );
    }
    Serial.println("String Descriptors:");
    if ( buf.iManufacturer > 0 ) {
      Serial.print("Manufacturer:\t\t");
      rcode = getstrdescr( addr, buf.iManufacturer );   // get manufacturer string
      if ( rcode ) {
        Serial.println( rcode, HEX );
      }
      Serial.print("\r\n");
    }
    if ( buf.iProduct > 0 ) {
      Serial.print("Product:\t\t");
      rcode = getstrdescr( addr, buf.iProduct );        // get product string
      if ( rcode ) {
        Serial.println( rcode, HEX );
      }
      Serial.print("\r\n");
    }
    if ( buf.iSerialNumber > 0 ) {
      Serial.print("Serial:\t\t\t");
      rcode = getstrdescr( addr, buf.iSerialNumber );   // get serial string
      if ( rcode ) {
        Serial.println( rcode, HEX );
      }
      Serial.print("\r\n");
    }
  }
  return rcode;
}

//  function to get single string description
uint8_t getstrdescr( uint8_t addr, uint8_t idx )
{
  uint8_t buf[ 256 ];
  uint8_t rcode;
  uint8_t length;
  uint8_t i;
  uint16_t langid;
  rcode = Usb.getStrDescr( addr, 0, 1, 0, 0, buf );  //get language table length
  if ( rcode ) {
    Serial.println("Error retrieving LangID table length");
    return ( rcode );
  }
  length = buf[ 0 ];      //length is the first byte
  rcode = Usb.getStrDescr( addr, 0, length, 0, 0, buf );  //get language table
  if ( rcode ) {
    Serial.print("Error retrieving LangID table ");
    return ( rcode );
  }
  langid = (buf[3] << 8) | buf[2];
  rcode = Usb.getStrDescr( addr, 0, 1, idx, langid, buf );
  if ( rcode ) {
    Serial.print("Error retrieving string length ");
    return ( rcode );
  }
  length = buf[ 0 ];
  rcode = Usb.getStrDescr( addr, 0, length, idx, langid, buf );
  if ( rcode ) {
    Serial.print("Error retrieving string ");
    return ( rcode );
  }
  for ( i = 2; i < length; i += 2 ) {   //string is UTF-16LE encoded
    Serial.print((char) buf[i]);
  }
  return ( rcode );
}

/* prints hex numbers with leading zeroes */
// copyright, Peter H Anderson, Baltimore, MD, Nov, '07
// source: http://www.phanderson.com/arduino/arduino_display.html
void print_hex(int v, int num_places)
{
  int mask = 0, n, num_nibbles, digit;

  for (n = 1; n <= num_places; n++) {
    mask = (mask << 1) | 0x0001;
  }
  v = v & mask; // truncate v to specified number of places

  num_nibbles = num_places / 4;
  if ((num_places % 4) != 0) {
    ++num_nibbles;
  }
  do {
    digit = ((v >> (num_nibbles - 1) * 4)) & 0x0f;
    Serial.print(digit, HEX);
  }
  while (--num_nibbles);
}

/* function to print configuration descriptor */
void printconfdescr( uint8_t* descr_ptr )
{
  USB_CONFIGURATION_DESCRIPTOR* conf_ptr = ( USB_CONFIGURATION_DESCRIPTOR* )descr_ptr;
  printProgStr(Conf_Header_str);
  printProgStr(Conf_Totlen_str);
  print_hex( conf_ptr->wTotalLength, 16 );
  printProgStr(Conf_Nint_str);
  print_hex( conf_ptr->bNumInterfaces, 8 );
  printProgStr(Conf_Value_str);
  print_hex( conf_ptr->bConfigurationValue, 8 );
  printProgStr(Conf_String_str);
  print_hex( conf_ptr->iConfiguration, 8 );
  printProgStr(Conf_Attr_str);
  print_hex( conf_ptr->bmAttributes, 8 );
  printProgStr(Conf_Pwr_str);
  print_hex( conf_ptr->bMaxPower, 8 );
  return;
}

/* function to print interface descriptor */
bool printintfdescr( uint8_t* descr_ptr )
{
  bool r = false;
  USB_INTERFACE_DESCRIPTOR* intf_ptr = ( USB_INTERFACE_DESCRIPTOR* )descr_ptr;
  printProgStr(Int_Header_str);
  printProgStr(Int_Number_str);
  print_hex( intf_ptr->bInterfaceNumber, 8 );
  printProgStr(Int_Alt_str);
  print_hex( intf_ptr->bAlternateSetting, 8 );
  printProgStr(Int_Endpoints_str);
  print_hex( intf_ptr->bNumEndpoints, 8 );
  printProgStr(Int_Class_str);
  print_hex( intf_ptr->bInterfaceClass, 8 );
  printProgStr(Int_Subclass_str);
  print_hex( intf_ptr->bInterfaceSubClass, 8 );
  printProgStr(Int_Protocol_str);
  print_hex( intf_ptr->bInterfaceProtocol, 8 );
  printProgStr(Int_String_str);
  print_hex( intf_ptr->iInterface, 8 );

  //Audio Class
  if (intf_ptr->bInterfaceClass == 1 ) {
    switch ( intf_ptr->bInterfaceSubClass ) {
      case 1 : //Audio Control
        printProgStr(Int_AC_str);
        break;
      case 2 : //Audio Streaming
        printProgStr(Int_AS_str);
        break;
      case 3 : //Midi Streaming
        printProgStr(Int_MS_str);
        r = true;
        break;
    }
  }
  return r;
}

/* function to print endpoint descriptor */
void printepdescr( uint8_t* descr_ptr )
{
  USB_ENDPOINT_DESCRIPTOR* ep_ptr = ( USB_ENDPOINT_DESCRIPTOR* )descr_ptr;
  printProgStr(End_Header_str);
  printProgStr(End_Address_str);
  print_hex( ep_ptr->bEndpointAddress, 8 );
  printProgStr(End_Attr_str);
  print_hex( ep_ptr->bmAttributes, 8 );
  switch(ep_ptr->bmAttributes & 3){
    case USB_TRANSFER_TYPE_CONTROL:
      printProgStr(End_Attr_control_str);
      break;
    case USB_TRANSFER_TYPE_ISOCHRONOUS:
      printProgStr(End_Attr_iso_str);
      break;
    case USB_TRANSFER_TYPE_BULK:
      printProgStr(End_Attr_bulk_str);
      break;
    case USB_TRANSFER_TYPE_INTERRUPT:
      printProgStr(End_Attr_int_str);
      break;
  }
  printProgStr(End_Pktsize_str);
  print_hex( ep_ptr->wMaxPacketSize, 16 );
  printProgStr(End_Interval_str);
  print_hex( ep_ptr->bInterval, 8 );

  return;
}

/* function to print unknown descriptor */
void printunkdescr( uint8_t* descr_ptr )
{
  uint8_t length = *descr_ptr - 2;
  uint8_t i;
  printProgStr(Unk_Header_str);
  printProgStr(Unk_Length_str);
  print_hex( *descr_ptr, 8 );
  printProgStr(Unk_Type_str);
  print_hex( *(descr_ptr + 1 ), 8 );
  printProgStr(Unk_Contents_str);
  descr_ptr += 2;
  for ( i = 0; i < length; i++ ) {
    print_hex( *descr_ptr, 8 );
    descr_ptr++;
  }
}

void printDumpHex(uint8_t* descr_ptr, uint8_t length)
{
  printProgStr(CRLF_str);
  for ( int i = 0; i < length; i++ ) {
    print_hex( *descr_ptr, 8 );
    descr_ptr++;
  }
  //printProgStr(CRLF_str);
}

/* function to print USB_DESCRIPTOR_CS_INTERFACE descriptor */
void printmsifdescr( uint8_t* descr_ptr )
{
  USB_MS_INTERFACE_DESCRIPTOR* if_ptr = ( USB_MS_INTERFACE_DESCRIPTOR* )descr_ptr;
  printProgStr(Int_CS_Header_str);
  printDumpHex(descr_ptr, if_ptr->bLength);
  printProgStr(CS_MS_Subtype_str);
  print_hex( if_ptr->bDescriptorSubtype, 8 );
  printProgStr(Int_CS_Type_str);

  if ( if_ptr->bDescriptorSubtype == MSIF_MS_HEADER) { //MS_HEADER
    printProgStr(CS_Subtype_MS_HEADER_str);
    printProgStr(CS_MS_Version_str);
    print_hex( if_ptr->bcdMSC, 16 );
    printProgStr(CS_MS_Length_str);
    print_hex( if_ptr->wTotalLength, 16 );
  } else if ( if_ptr->bDescriptorSubtype == MSIF_MIDI_IN_JACK) { //MIDI_IN_JACK
    USB_MS_INTERFACE_MIDI_IN_JACK_DESCRIPTOR* if_inptr = (USB_MS_INTERFACE_MIDI_IN_JACK_DESCRIPTOR*)descr_ptr;
    printProgStr(CS_Subtype_MIDI_IN_JACK_str);
    printmsjacktype(if_inptr->bJackType);
    printProgStr(CS_MIDI_JACKID_str);
    print_hex( if_inptr->bJackID, 8 );
  } else if ( if_ptr->bDescriptorSubtype == MSIF_MIDI_OUT_JACK) { //MIDI_OUT_JACK
    USB_MS_INTERFACE_MIDI_OUT_JACK_DESCRIPTOR* if_outptr = (USB_MS_INTERFACE_MIDI_OUT_JACK_DESCRIPTOR*)descr_ptr;
    printProgStr(CS_Subtype_MIDI_OUT_JACK_str);
    printmsjacktype(if_outptr->bJackType);
    printProgStr(CS_MIDI_JACKID_str);
    print_hex( if_outptr->bJackID, 8 );
    printProgStr(CS_MIDI_NrInputPins_str);
    print_hex( if_outptr->bNrInputPins, 8 );
    printDumpHex(descr_ptr + 6, if_outptr->bNrInputPins * 2 + 1);
  } else if ( if_ptr->bDescriptorSubtype == MSIF_ELEMENT) { //ELEMENT
    printProgStr(CS_Subtype_ELEMENT_str);
  } else {
    printProgStr(CS_Subtype_UNDEF_str);
  }
}

/* function to print MIDI IN/OUT Jack Descriptor JackType */
void printmsjacktype( uint8_t bJackType )
{
    switch (bJackType) {
      case MSJT_JACK_TYPE_UNDEFINED:
        printProgStr(CS_Subtype_MIDI_JACK_TYPE_UNDEFINED_str);
        break;
      case MSJT_EMBEDDED:
        printProgStr(CS_Subtype_MIDI_JACK_TYPE_EMBEDDED_str);
        break;
      case MSJT_EXTERNAL:
        printProgStr(CS_Subtype_MIDI_JACK_TYPE_EXTERNAL_str);
        break;
      default:
        printProgStr(CRLF_str);
        break;
    }
    printProgStr(CS_MS_Jacktype_str);
    print_hex( bJackType, 8 );
}

/* function to print CS_MS_ENDPOINT descriptor */
void printmsepdescr( uint8_t* descr_ptr )
{
  USB_MS_ENDPOINT_DESCRIPTOR* ep_ptr = ( USB_MS_ENDPOINT_DESCRIPTOR* )descr_ptr;
  printProgStr(End_CS_Header_str);
  printDumpHex(descr_ptr, ep_ptr->bLength);
  switch ( ep_ptr->bDescriptorSubtype ) {
    case 1: //MS_GENERAL
      printProgStr(End_CS_Type_str);
      printProgStr(CS_Subtype_MS_GENERAL_str);
      printProgStr(CS_MIDI_NrEmbJacks_str);
      print_hex( ep_ptr->bNumEmbMIDIJack, 8 );
      
      printDumpHex(descr_ptr + 4, ep_ptr->bNumEmbMIDIJack);
      break;
    default:
      printunkdescr( descr_ptr );
      break;
  }
}

/* Print a string from Program Memory directly to save RAM */
void printProgStr(const char* str)
{
  char c;
  if (!str) return;
  while ((c = pgm_read_byte(str++)))
    Serial.print(c);
}
