\page APIguide
- \subpage newnavimpl - NewNav Implementation Status
\page newnavimpl NewNav Implementation Status
# Summary of Navigation Message Implementation Status in NewNav

The navigation messages broadcast by the various GNSSs on their various signals have a few things in common. 
   * The transmission is broken into sets of bits that are assigned a given meaning.  For purposes of this document, the unique definitions are referred to as a packet. 
   The term packet is being used becuase none of the existing navigation message types are using that term. 
   Depending on the particular system-signal, a packet could be called, subframes (GPS LNAV, BeiDou D1, BeiDou D2), Message Types (GPS CNAV), Pages (Galileo I/NAV), or Strings (GLONASS)
   * Generally, NewNav expects to receive the raw hard bits of the navigation message data in "chunks" by packet. If NewNav "knows" what to do with that packet, 
   it creates an appropriate NewNav object and places it in the data store.  If NewNav hasn't been programmed to process a given packet, it skips the packet and moves to the next one. 
   * Typically all navigation message types include the following items:
      * Clock, Ephemeris, and Integrity (CEI) Data.  This is the predicated orbit/clock used for navigation.  Typically a given space vehicle (SV) broadcasts its own CEI data and no other. 
      * Almanac Data. A lower-precision set of orbit/clock data used for acquistion and missing planning.  Typically, a given SV broadcasts the alamanc for all SVs in the constellation. 
      * Time Offset data.  This allows the user to translate between the GNSS system time and other time systems. Typically each navigation message type includes time offset information to UTC.
      Some message types include data for conversions between GNSS time systems.
      * Intersignal corrections (ISC). These data allow translation of results between various signal-frequency pairs. 
      * Ionospheric model parameters (IONO). These support single frequency users.
   * Additionally, different navigation message types include a variety of other "system level" information. Examples include
      * Differential corrections.  To allow interested users to obtain higher-precision PVT results given additional processing.
      * Precise Point Positioning (PPP) data.  To support PPP users. 
      * Earth orientation parameters. To allow use by space service volume users. 
      * Ionospheric maps.  To provide single frequency users with improved performance.
      * Reserved system data.  These are applicable to a variety of special cases. 
   * In general, NewNav concentrates on packets containing CEI data, Almanac, Time Offset, IONO, and ISC data.  These are the items that are required for basic PVT processing and receiver operation. 
   * Additional packets may be processed if there's a sponsor-specific interest in the data contained in those messages. 
   * Relevant information on what portions of a message are-or-are-not processed can be found in NewNav/PNB/*DataFactory.cpp.  
   Adding new capabilities will likely mean incorporating new conditionals in the appropriate PNB/*DataFactory to handle a specific packet. 
   
For each navigation message type, there is a section below containing the following subsections. 
   * Characteristics - A brief summary of the key nomenclature, structure, and timing of the particular message.
   * Implementation status - A table summarizing the following characteristics.
      * Message.  The packet designation.
      * Status (Y/N).  Is the particular packet implemented in NewNav?
      * NewNav Object.  If the packet is implemented,the name of the NewNav class that is generated when that packet is found in the data. 
      * Notes.  Any further explanatory notes specific to the packet.
   * Comments. Any general comments applicable to the navigation message type. 

# Summary by Navigation Message Type 

## Summary for GPS LNAV

### Characteristics
   * packets are known as subframes.
   * A subframe has 300 bits, transmitted at 50 bps.
   * Therefore a subframe is transmitted in 6s.
   * A frame is a set of five consecutive subframes.
   * Frames are aligned with 30s epochs of GPS time. 
   * There are five types of subframes.  Subframes 1-3 have a fixed definition. For subframes 4-5, there are multiple pages defined that contain different information.  These are abbreviated by "pg" in the following table.

### Implementation Status by Packet

| Packet | Status | NewNav Object | Notes |
| --- | --- | --- | --- |
| sf 1 | Y | GPSLNavEph, GPSLNavHealth, GPSLNavISC | CEI Data, health, T-sub-GD (considered an ISC)  |
| sf 2 | Y | GPSLNavEph | CEI Data |
| sf 3 | Y | GPSLNavEph | CEI Data |
| sf 4 |  Y | GPSLNavAlm, GPSLNavHealth | Almanac, health |
|  pg 2-5 | Y | GPSLNavAlm, GPSLNavHealth | Almanac, health |
|  pg 7-10 | Y | GPSLNavAlm, GPSLNavHealth | Almanac, health |
| sf5 pg 1-24 | Y | GPSLNavAlm, GPSLNavHealth | Almanac, health |
| sf4 pg 1, 6, 11, 16, 21 | N | | Data ID 57, Reserved |
| sf4 pg 12, 24 | N | | Data ID 62, Reserved |
| sf4 pg 13 | N | | Data ID 52, Navigation Message Correction Table (NMCT) |
| sf4 pg 14 | N | | Data ID 53, Reserved |
| sf4 pg 15 | N | | Data ID 54, Reserved |
| sf4 pg 17 | N | | Data ID 55, Special Messages |
| sf4 pg 18 | Y | GPSLNavTimeOffset, GPSLNavIono | Data ID 56, UTC and iono |
| sf4 pg 19 | N | | Data ID 58, Reserved |
| sf4 pg 20 | N | | Data ID 59, Reserved | 
| sf4 pg 22 | N | | Data ID 60, Reserved | 
| sf4 pg 23 | N | | Data ID 61, Reserved |
| sf4 pg 25 | Y | GPSLNavHealth,GPSNavConfig | Data ID 63, SV Health (PRN 25-33), SV config (all). |
| sf5 pg 25 | Y | GPSLNavHealth | Data ID 51, SV Health (PRN 1-24) |

### Comments
The following comments are applicable to the GPS LNAV implementaiton 
   * It would be helpful to have Data IDs 53-55 and 57-62 processed to we can obtain a hex output of the 24 bits of data in each 30 bit word.
   Furthermore, this should be in some CSV format to support further downstream manipulation.    
   This would simplify task-specific analysis of items that are needed infrequently. 
   * It would be useful to have Data ID 52, Navigation Message Correction Table (NMCT) processed to the extent that we can output.
      * The time tag of the message
      * The data availability field.  This tells us if the data are available/unavailable and encrypted/unencrypted. 
      * The 31 corrections in either hex (for encrypted data) or in range units (m) for unencrypted data. 


## Summary for GPS CNAV (L2C or L5)

### Characteristics
   * A packet is known as a message type (MT).
   * A MT has 300 bits, transmitted at 25 bps (L2C) or 50 bps (L5).
   * Therefore a MT is transmitted in 12s (L2C) or 6s (L5).
   * L2C MT are aligned with 12s epochs of GPS time. 
   * L5 MT are aligned with 6s epochs of GPS time. 

### Implementation Status by Packet

| Packet | Status | NewNav Object | Notes |
| --- | --- | --- | --- |
| 10 | Y | GPSCNavEph, GPSCNavHealth | Ephemeris 1 |
| 11 | Y | GPSCNavEph, GPSCNavHealth | Ephemeris 2 |
| 30 | Y | GPSCNavEph, GPSCNavIono, GPSCNavISC | Clock, Iono & Group Delay (ISC) |
| 31 | Y | GPSCNavAlm, GPSCNavHealth | Reduced Almanac | 
| 32 | N | | Clock & EOP | 
| 33 | Y | GPSCNavTimeOffset | Clock & UTC | 
| 34 | N | | Clock & Differential Correction | 
| 35 | Y | GPSCNavTimeOffset | Clock & GPS-Galileo Time Transfer (GGTO) | 
| 36 | N | | Clock & Text | 
| 37 | Y | GPSCNavRedAlm, GPSCNavHealth | Clock & Midi Almanac | 
| 12 | Y | GPSCNavRedAlm, GPSCNavHealth | Reduced Almanac |
| 13 | N | | Clock Differential Correction |
| 14 | N | | Ephemeris Differential Correction |
| 15 | N | | Text | 
| 40 | N | | Integrity Support Message | 

### Comments
The following comments are applicable to the GPS CNAV implementaiton 
   * CNAV is a Flexible Data Message (FDM) format.  Some of these MT have never been transmitted and may never be transmitted.  
   This needs to be considered in making decisions on what is to be implemented. 
   * NGA campaigned hard to get the EOP message. We should probably be prepared to crack it, but its never been transmitted to date. 
   * The Integrity Support Message is coming in the future, and will be important to CSMS, but it has never been transmitted to date.
   * Bits 39-127 of MT30-37 contain the CEI Data clock information, however, NewNav currently only cracks CEI Data clock from MT30. 
   
## Summary for GPS CNAV-2 (L1C)

### Characteristics
   * The packet definition is unusual.  There are three "subframes".  That is in quotes because the three are not identical in size and are dissimilar in content. 
   * A frame (which combines subframe 1,2,3) is 18s long.
   * Subframe 1 is very short and contains time and data synchronization information. 
   * Subframe 2 contains CEI data. 
   * Subframe 3 contains different system information or constellation data every frame. 
   * Each frame is aligned with 18s epochs of GPS time. 

### Implementation Status by Packet

| Packet Type | Status | NewNav Object | Notes |
| --- | --- | --- | --- |
| subframe 2 | Y | GPSCNav2Eph, GPSCNav2Health | CEI data, health | 
| subframe 3, page 1 | Y | GPSCNav2TimeOffset | UTC & IONO  |
|                    | Y | GPSCNav2ISC | |
|                    | Y | GPSCNaV2IONO | |
| subframe 3, page 2 | Y | GPSCNav2TimeOffset | GGTO & EOP. N.B.: Only GGTO is cracked | 
| subframe 3, page 3/4 | Y | GPSCNav2Alm, GPSCNav2Health | Almanac, health | 
| subframe 3, page 5 | N | | Differential corrections |
| subframe 3, page 6 | N | | Text | 
| subframe 3, page 7 | Y | GPSNavConfig | SV Configuration |
| subframe 3, page 8 | N | | Integrity Support Message | 

### Comments
The following comments are applicable to the GPS CNAV-2 implementation 
   * At this time, only QZSS is transmitting L1C.
   * The only GPS CNAV-2 "live data" is from SVN 74/PRN 04 post-launch testing.
   
## Summary for Galileo F/NAV
### Characteristics
   * The useful summary of the structure is in the OS-SIS-ICD sections 4.2.1-4.2.2.
   * A packet is known as subframe x/page y.
   * A page has 244 bits.
   * A page is transmitted in 10s.
   * A subframe has 5 pages (50s).
   * A frame has 12 subframes (600 sec = 10 minutes). 
   * Subframes are aligned with 50s epochs of Galileo time. 

### Implementation Status by Packet

| Packet Type | Status | NewNav Object | Notes |
| --- | --- | --- | --- |
| page 1-4 | Y | GalFNavEph | CEI Data |
| page 1 | Y | GalFNavISC | ISC | 
| page 1 | Y | GalFNavIono | Iono | 
| page 1 | Y | GalFNavHealth | Health | 
| page 4 | Y | GalFNavTimeOffset | GAL-UTC offset and GAL-GPS offset | 
| page 5,6 | Y | GalFNavAlm, GalFNavHealth | Almanac, health | 

### Comments
The following comments are applicable to the Galileo F/NAV implementation. 
   * This may be the "most completely covered" message.  In part that is because it is generally limited to the critical data needed by a user. 

## Summary for Galileo I/NAV
### Characteristics
   * The useful summary of the structure in the OS-SIS-ICD sections 4.3.1-4.3.2.
   * A packet is known as a "Word Type".  
   * Each word is transmitted in a page within a rigid subframe/page structure.
   * A word has 120 bits.
   * A word is transmitted in 2s.
   * A subframe has 15 pages/words (30s).
   * A frame has 24 subframes (12 minutes). 
   * Subframes are aligned with 30s epochs of Galileo time with E1B offset by 1s.
   * The data E1B and E5a are arranged in complementary order such that a user collecting both can obtain all date in half the time.    

### Implementation Status by Packet

| Packet Type | Status | NewNav Object | Notes |
| --- | --- | --- | --- |
| 1, 2, 3, 4, 5  | Y | GalINavEph | CEI Data,  These repeat each subframe. |
|  5  | Y | GalINavIono | Iono*  | 
|  5  | Y | GalINavISC | ISC* | 
| 5 | Y | GalINavHealth | Health* | 
| 6 | Y | GalINavTimeOffset | GAL-UTC* | 
| 7-8 | Y | GalINavAlm, GalINavHealth* | Almanac (one plus 1/2), health | 
| 8-9 | Y | GalINavAlm, GalINavHealth* | Almanac (1/2 plus 1), health |
| 10 | Y | GalINavTimeOffset | GAL-GPS* |
| 16 | N |  | Reduced Almanac* | 
* - Only certain subframes wihtin a frame.

### Comments
The following comments are applicable to the Galileo I/NAV implementation. 
   * (none at this time.) 


## Summary for GLONASS FDMA
### Characteristics
   * A packet is known as a string.
   * Strings have 80 bits of data plus overhead. 
   * A string is transmitted in 2s.
   * There is a rigid structure with frames containing 15 strings and superframe of 5 frames.
   Therefore, frames are 30s and superframes are 2.5 minutes. 
   * Frames are aligned with 30s epochs of UTC.  
   * NOTE: GLONASS follows UTC, but does include a system time offset. 
   * GLONASS time is counted in four year increments, day within the four year, and H:M:S of the day. 
   * For an overview summary see /v/sgggid/SoftwareReceiver/afggngss/IB/SummaryCards/Glonass_Summary.pdf

### Implementation Status by Packet

| Packet Type | Status | NewNav Object | Notes |
| --- | --- | --- | --- |
| 1-4 | Y | GLOFNavEph | CEI data (called Immediate data) | 
| 2,3 | Y | GLOFNavHealth | SV Health |
| 4 | Y | GLOFNavISC | ISC data |
| 5 | Y | GLOFNavTimeOffset | GLONASS-UTC, GLONASS-GPS offsets | 
|   |   |  | Day within quadrennial. Incorporated into GLOFNavEph | 
| 6/7 | Y | GLOFNavAlm | Almanac |
| 8/9 | Y | GLOFNavAlm | Almanac |
| 10/11 | Y | GLOFNavAlm | Almanac |
| 12/13 | Y | GLOFNavAlm | Almanac |
| 14 | Y | GLOFNavUT1TimeOffset | UT1 (Earth rotation) | 
| 15 | | | Reserved | 

### Comments
The following comments are applicable to the GLONASS FDMA implementation. 
   * These classes have not been widely used.  
   * Given the translation of the ICD from Russian to English, there may be problems with interpretation of the data. 

## Summary for GLONASS CDMA
### Characteristics
   * A packet is known as a string.
   * A string has 250 bits, transmitted at 125 bps.
   * Therefore a string is transmitted in 2s.
   * There is not a defined order for the different types of strings.
   * Strings are aligned with 2s epochs of UTC. 

### Implementation Status by Packet

| Packet Type | Status | NewNav Object | Notes |
| --- | --- | --- | --- |
| 10, 11, 12 | Y | GLOCNavEph | CEI Data (Immediate Data) |
| 20 | Y | GLOCNavAlm | Almanac | 
| 25 | Y | GLOCUT1TimeOffset, GLOCNavIono | Earth rotation (UT1), Iono, UTC | 
| 16 | N  | | SV Attitude | 
| 31,32 | Y | GLOCNavLTDMP | Long-trem dynamic model parameter |

### Comments
The following comments are applicable to the GLONASS CDMA implementation. 
   * The GLONASS CDMA format is not fully implemented. 
   For example, the CEI Data requires use of SV attitude data which has not yet been decoded.
   In addition, the UTC time offset doesn't seem to be cracked.


## Summary for BeiDou D1
### Characteristics
   * packets are known as subframes and pages within subframes.
   * A packet has 300 bits, transmitted at 50 bps.
   * Therefore a MT is transmitted in 6s.
   * Subframes are arranged in frames of 5 subframes each (30s).
   * A superframe consists of 24 frames (12 minutes). 
   * Packets are aligned with 6s epochs of BeiDou time. 

### Implementation Status by Packet

| Packet Type | Status | NewNav Object | Notes |
| --- | --- | --- | --- |
| subframe 1-3 | Y | BDSD1NavEph, BDSD1NavHealth, BDSD1NavIono, BDSD1NavISC | CEI Data, health, Iono, ISC | 
| subframe 4, pages 1-24 | Y | BDSD1NavAlm, BDSD1NavHealth | Almanac (Expanded) |
| subframe 5, pages 1-6 | Y | BDSD1NavAlm, BDSD1NavHealth | Almanac (Expanded) |
| subframe 5, page 7-8, 24 | Y | BDSD1NavHealth | Constellation health, Almanac overhead |
| subframe 5, page 9 | Y | BDSD1NavTimeOffset | Time offsets to other GNSSs |
| subframe 5, page 10 | Y | BDSD1NavTimeOffset | BeiDou - UTC |
| subframe 5, pages 11-23 | Y | BDSD1NavAlm, BDSD1NavHealth | Almanac (regular) |

### Comments
The following comments are applicable to the BeiDou D1 implementation. 
  * This format is broadcast by the BeiDou-2/3 MEO/IGSO SVs.

## Summary for BeiDou D2
### Characteristics
   * Packets are known as subframes and pages within subframes.
   * A packet has 300 bits, transmitted at 500 bps.
   * Therefore a packet is transmitted in 0.6s.
   * Subframes are arranged in frames of 5 subframes each (3s).
   * A superframe consists of 120 frames (6 minutes). 
   * Packets are aligned with 6s epochs of BeiDou time. 

### Implementation Status by Packet

| Packet Type | Status | NewNav Object | Notes |
| --- | --- | --- | --- |
| subframe 1, page 1-10 | Y | BDSD2NavEph, BDSD2NavHealth, BDSD2NavISC, BDSD2NavIono | CEI data, health, ISC, Iono | 
| subframe 2, pages 1-6 | N | | Differential Corrections | 
| subframe 3, pages 1-6 | N | | Integrity Data |
| subframe 4, pages 1-6 | N | | Differential Corrections |
| subframe 5, pages 1-13, 61-73 | N | | Iono map of Asia-Pacific region |
| subframe 5, pages 37-60, 95-100 | Y | BDSD2NavAlm, BDSD2NavHealth | Almanac | 
| subframe 5, pages 14-34, 74-94, 117-120 | N | | Reserved |
| subframe 5, page 35,36 | Y | BDSD2NavHealth | Constellation Health | 
| subframe 5, page 101 | Y | BDSD2NavTimeOffest | Time offsets to other GNSSs |
| subframe 5, page 102 | Y | BDSD2NavTimeOffest | BeiDou-UTC offset |
| subframe 5, page 103-115 | N | | Expanded Almanac | 
| subframe 5, page 116 | Y | BDSD2NavHealth | Constellation health | 

### Comments
The following comments are applicable to the BeiDou D2 implementation. 
   * This format is broadcast by the BeiDou-2/3 GEO SVs.

## Summary for BeiDou B1C
### Characteristics
   * TBD
   
### Implementation Status by Packet

| Packet Type | Status | NewNav Object | Notes |
| --- | --- | --- | --- |
|     |     |     |     |

### Comments
   * This format is broadcast by the BeiDou-3 MEO/IGSO SVs
   
## Summary for BeiDou B2a
### Characteristics
   * TBD
   
### Implementation Status by Packet

| Packet Type | Status | NewNav Object | Notes |
| --- | --- | --- | --- |
|     |     |     |     |

### Comments
   * This format is broadcast by the BeiDou-3 MEO/IGSO SVs

## Summary for QZSS(CNAV)
### Characteristics
   * QZSS broadcasts several signals.  Some these signals are compatible with GPS and contain similar message data.  Implementation in NewNav is spotty.
   For example, PNBLNavDataFactory, PNBCNavDataFactory, and PNBCNav2DataFactory contain a number of conditionals that choose whether to process MT as GPS CNAV or QZSS.  We need a separate sweep through the code to establish the QZSS capability. 
   * TBD
   
### Implementation Status by Packet

| Packet Type | Status | NewNav Object | Notes |
| --- | --- | --- | --- |
|     |     |     |     |

### Comments
   * TBD


