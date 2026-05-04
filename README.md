
# GRETS — GRETA Trace Sender

## Overview

**GRETS** is a high-throughput UDP packet generator designed to stream GRETA-like detector events from a binary file into a network socket.

It:
* Reads pre-recorded detector events from disk

* Packs them into fixed-size UDP messages

* Streams them at a controlled (or maximum) rate using multi-threading and batching
* 

The goal is **DAQ / pipeline stress testing**.

## Build

The project uses CMake.

From :

```bash
mkdir build && cd build
cmake ..
make -j
```

Executable:

```bash
./grets <rate> # The rate might not be exact.
```

---

## Usage

```bash
./grets 1000000
```

* Argument = **target message rate (messages per second)**
* Real rate is approximate.


## Data Format

### Message Structure

Each UDP packet is a **fixed-size binary blob**:

```
[ routing_header ][ wvf_message ]
```

Total size:

* `UDP_SIZE = 15272 bytes`

---

### Routing Header

```cpp
struct routing_header {
  uint8_t version;
  uint8_t flags;
  uint8_t type;
  uint8_t subtype;

  uint16_t length;
  uint16_t seqnum;

  int64_t timestamp;
  int64_t checksum;
};
```

---

### Waveform Message

```cpp
struct wvf_message {
  uint8_t version;
  uint8_t ID;

  uint16_t TrLen;
  uint16_t TrSrc;
  uint16_t Pad;

  uint64_t Timestamp;
  uint64_t Pileup;

  int16_t HistCorr[2][2];

  int16_t T0, SubT0, TLedCore, TCfdCore;
  int16_t TLedFirst, TPostPileup, TOvfl, TRst;

  int32_t Ener[N_CHANNELS];
  int16_t Tr[N_CHANNELS][TR_LEN];
};
```

Parameters:

* `N_CHANNELS = 40`
* `TR_LEN = 188`

---

### Combined Event

```cpp
struct whole_message {
  routing_header header;
  wvf_message message;
};
```
