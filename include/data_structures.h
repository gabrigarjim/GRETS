
#ifndef DATA_STRUCTURES_H
#define DATA_STRUCTURES_H

#include "definitions.h"
#include "libs.h"

struct routing_header{

 uint8_t version;
 uint8_t flags;
 uint8_t type;
 uint8_t subtype;

 uint16_t length;
 uint16_t seqnum;

 int64_t timestamp;
 int64_t checksum;


};



struct wvf_message{

 uint8_t version;
 uint8_t ID;

 uint16_t TrLen;
 uint16_t TrSrc;
 uint16_t Pad;

 uint64_t Timestamp;
 uint64_t Pileup;

 int16_t HistCorr[2][2];

 int16_t T0;
 int16_t SubT0;
 int16_t TLedCore;
 int16_t TCfdCore;
 int16_t TLedFirst;
 int16_t TPostPileup;
 int16_t TOvfl;
 int16_t TRst;

 int32_t Ener[N_CHANNELS];

 int16_t Tr[N_CHANNELS][TR_LEN];

};

struct wvf_custom_message{ // Add fields if needed: for now this is for testing parallel traces. Tell Anna about this. 

 uint8_t version;
 
 uint16_t TrLen;

 uint64_t Timestamp;
 
 int16_t Tr[N_CHANNELS][(TR_LEN + OVERLAP) * MINIBATCH_SIZE];

};



struct whole_message { 

   routing_header header; 
   wvf_message message; 

};


struct whole_custom_message{

  routing_header header; 
  wvf_custom_message custom_message;

};

#endif

