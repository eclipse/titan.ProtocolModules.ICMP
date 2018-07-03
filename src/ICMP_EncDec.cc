/******************************************************************************
* Copyright (c) 2007, 2014  Ericsson AB
* All rights reserved. This program and the accompanying materials
* are made available under the terms of the Eclipse Public License v2.0
* which accompanies this distribution, and is available at
* https://www.eclipse.org/org/documents/epl-2.0/EPL-2.0.html
*
* Contributors:
*   Gabor Bettesch - initial implementation and initial documentation
******************************************************************************/
//
//  File:     ICMP_EncDec.cc
//  Rev:      R2A
//  Prodnr:   CNL 113 529
///////////////////////////////////////////////////////////////////////////////
#include "ICMP_Types.hh"

namespace ICMP__Types {

TTCN_Module ICMP__EncDec("ICMP_EncDec", __DATE__, __TIME__);

OCTETSTRING f__enc__PDU__ICMP(const PDU__ICMP& pdu)
{
  if (TTCN_Logger::log_this_event(TTCN_DEBUG)) {
    TTCN_Logger::begin_event(TTCN_DEBUG);
    TTCN_Logger::log_event_str("Encoding PDU_ICMP: ");
    pdu.log();
    TTCN_Logger::end_event();
  }
  if (pdu.get_selection() == PDU__ICMP::ALT_unknown)
    return pdu.unknown();

  TTCN_Buffer buf;
  pdu.encode(PDU__ICMP_descr_, buf, TTCN_EncDec::CT_RAW);

  const unsigned char* buf_ptr = buf.get_data();
  size_t datalen = buf.get_len();


//checksum calculation
  unsigned long sum = ( buf_ptr[1] << 8) + buf_ptr[0];

//Skip checksum field

  for (size_t i = 4; i <= datalen - 2; i = i + 2)
    sum += (buf_ptr[i+1] << 8) + buf_ptr[i];

  if (datalen % 2) // datalen is odd
  {
    sum += buf_ptr[datalen-1];
  }

  sum = (sum & 0xFFFF) + (sum >> 16);
  sum = (sum & 0xFFFF) + (sum >> 16);
  sum = ~sum;

  unsigned char ret[4];
  ret[0] = buf_ptr[0];
  ret[1] = buf_ptr[1];
  ret[2] = (sum) & 0x00FF;
  ret[3] = ((sum) & 0xFF00) >> 8;

//put checksum into the checksum field of the message
  OCTETSTRING ret_val = OCTETSTRING(4, ret) + OCTETSTRING(buf.get_len()-4,buf.get_data()+4);

  if (TTCN_Logger::log_this_event(TTCN_DEBUG)) {
    TTCN_Logger::begin_event(TTCN_DEBUG);
    TTCN_Logger::log_event_str("PDU_ICMP after encoding: ");
    ret_val.log();
    TTCN_Logger::end_event();
  }
  return ret_val;
}

}//namespace
