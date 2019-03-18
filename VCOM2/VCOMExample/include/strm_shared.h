/******************************<%BEGIN LICENSE%>******************************/
// (c) Copyright 2013 Lumidigm, Inc. (Unpublished Copyright) ALL RIGHTS RESERVED.
//
// For a list of applicable patents and patents pending, visit www.lumidigm.com/patents/
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
// FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
// COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
// IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
// CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
//
//
/******************************<%END LICENSE%>******************************/
#ifndef _strm_shared_h_
#define _strm_shared_h_

#include "lumi_stdint.h"

//
//	Stream Shared File
//	These Interface Definitions are Shared btw the Cypress 8051 Controller and Host Driver
//

// Product ID's Used to Select Platform
#define _V31x_PLATFORM_PID_		0x0021
#define _M31x_PLATFORM_PID_		0x0041
#define _V61x_PLATFORM_PID_		0x0041
#define _uID3_PLATFORM_PID_     0x0011

/*
**	Start of Frame Tag
*/
#define MAX_PAYLOAD_SIZE 		54
#define EEPROM_MAX_BYTES		8192			// Number of Bytes in EEPROM

#define IMP_EOA 0
//#define LED_EOA 0xFFFF

/*
**	Control Transfer Packet Definition
**	MUST be 64 Bytes in Length
*/
#if defined(__GNUC__) || defined(WIN32) || defined(_WIN32)
#pragma pack(push,1)
#endif
typedef struct
{
	uint8_t id;
	uint8_t cmd;
	uint8_t Device_Address;
	uint8_t Register_Address;
	uint8_t Address_MSB;
	uint8_t Address_LSB;
	uint8_t NumBytes;
	uint8_t Value1;
	uint8_t Value2;
	uint8_t Width;
	uint8_t Data[MAX_PAYLOAD_SIZE];
} _CONTROL_PACKET_TYPE;

/*
**	Vendor Specific Interface Commands
*/
typedef enum
{
	LOOPBACK_WRITE		= 0x20,
	LOOPBACK_READ		= 0x21,
	JTAG_INIT			= 0x22,
	JTAG_EXEC			= 0x23,
	JTAG_WRITE			= 0x24,
	JTAG_READ			= 0x25,
	JTAG_STOP			= 0x26,
	PORTA_WRITE			= 0x27,
	PORTA_READ			= 0x28,
	PORTD_WRITE			= 0x29,
	PORTD_READ			= 0x2A,
	I2C_WRITE			= 0x2B,
	I2C_READ			= 0x2C,
	I2C_EEPROM_WRITE	= 0x2D,
	I2C_EEPROM_READ		= 0x2E,
	TWI_WRITE			= 0x2F,
	TWI_READ			= 0x30,
	TWI_EEPROM_WRITE	= 0x31,
	TWI_EEPROM_READ		= 0x32,
	CONFIG_PORT_DIR		= 0x33,
	XI_SER_WRITE		= 0x34,
	XI_SER_ENABLE		= 0x35,
	GET_FW_REVISION		= 0x36,
	GET_CPLD_REVISION	= 0x37,
	LDI_RENUM			= 0x38,
	I2C_BLOCK_WRITE		= 0x39,
	I2C_BLOCK_READ		= 0x3A,
	// ...
	LDI_WRITE_CMD		= 0x70,
	LDI_READ_CMD		= 0x71,
    LDI_ACQ_STEP_WRITE  = 0x72,
    LDI_ACQUIRE         = 0x73,
    LDI_IMP_STEP_WRITE  = 0x74,
	LDI_VGPIO			= 0x75
} Vendor_Interface_Type;

/*
**	EEPROM Format
*/
typedef struct
{
	/*
	**	1st 8 Bytes MUST Comply with Cypress Boot Loader Format
	**	DO NOT MODIFY
	*/
	uint8_t 
		type,				// Boot Must be either 0xC0 or 0xC2
		VID_LSB,
		VID_MSB,
		PID_LSB,			// Pre-Enum Product ID
		PID_MSB,
		DEVID_LSB,
		DEVID_MSB,
		CONFIG;
	// end required by Cypress format

	// start Lumidigm assignement
	uint32_t
		Serial_Number,
		CPLD_Firmware_Revision,
		ManDateCode;
	uint16_t
		Product_ID,			// Post-Enum Product ID
		Platform_Type,
		Bx_Row,				// V31x Only
		Bx_Col,				// V31x Only
		PD_Row,				// V31x Only
		PD_Col,				// V31x Only
		DPI,				// V31x Only
		MfgStateFlag;		// Manufacturing state flag

	uint8_t 
		pCalData[1520];		// V31x Only
	uint8_t 
		pTagData[256];		// For V100_Set_Tag and V100_Get_Tag functionality first byte is size

} EEPROM_Format_Type;

// maxumum number of steps in acquisition sequence
#define MAX_ACQ_STEPS 64
#define MAX_IMP_STEPS 64

typedef struct
{
    uint16_t  Seq;
    uint16_t  Gain;
    uint16_t  Exp;
    uint16_t  Bits;
    uint32_t  LED_State;
    uint8_t   Mode_AEC;
    uint8_t   Mode_AGC;
    uint8_t   Exp_Skip_Frm;
    uint8_t   Gain_Skip_Frm;
} Acq_Step;

typedef struct
{
    uint16_t Seq;
    uint16_t Settle_Time;
    uint32_t Frequency;
    uint8_t  Gain;
    uint8_t  Voltage;
    uint8_t  Mux;
    uint8_t  Pad1;
} Imp_Step;

typedef struct
{
    uint32_t FrameNum;
    uint32_t CryptoState;
    uint16_t LEDState;
} M310_FrameHdr;
#define SIZEOF_M310_FrameHdr 10

typedef struct
{
	uint32_t FrameNum;
	uint32_t ImpNumber;
	uint32_t LEDState;
	uint16_t Exp;
	uint16_t Gain;
	uint16_t ICV;
	uint16_t DarkAvg;
	uint16_t Bits;
	uint16_t Mode;
	uint16_t CryptoState;
	uint8_t  Acq_Index;

	uint16_t Imp_Real;
	uint16_t Imp_Imag;
	uint32_t Imp_Frequency;
	uint8_t  Imp_Voltage;
	uint8_t  Imp_Gain;
	uint8_t  Imp_Index;

	uint8_t  Mux;

	uint16_t  Pad2;
} M310_SA_FrameHdr;		// sizeof() == 41
#define SIZEOF_M310_SA_FrameHdr 41

typedef struct
{
    uint32_t FrameNum;
    uint32_t CryptoState;
    uint16_t LEDState;
} V310_FrameHdr;
#define SIZEOF_V310_FrameHdr 10

typedef struct
{
	uint32_t FrameNum;
	uint32_t ImpNumber;
	uint32_t LEDState;
	uint16_t Exp;
	uint16_t Gain;
	uint16_t ICV;
	uint16_t DarkAvg;
	uint16_t Bits;
	uint16_t Mode;
	uint16_t CryptoState;
	uint8_t  Acq_Index;
	
	uint16_t Imp_Real;
	uint16_t Imp_Imag;
	uint32_t Imp_Frequency;
	uint8_t  Imp_Voltage;
	uint8_t  Imp_Gain;
	uint8_t  Imp_Index;
	
	uint8_t  Mux;

	uint16_t  Pad2;
} V610_FrameHdr;		// sizeof() == 41
#define SIZEOF_V610_FrameHdr 41

typedef struct
{
    uint32_t FrameNum;
    uint32_t ImpNumber;
    uint32_t LEDState;
    uint16_t Exp;
    uint16_t Gain;
    uint16_t ICV;
    uint16_t DarkAvg;
    uint16_t Bits;
    uint16_t Mode;
    uint16_t CryptoState;
    uint8_t  Acq_Index;

    int32_t Imp_Real[8];
    int32_t Imp_Imag[8];
    uint8_t  Imp_Index;
    uint16_t Imp_Ctrl;
    uint8_t  Pad1;
} V620_FrameHdr;    // sizeof() == 95
#define SIZEOF_V620_FrameHdr 95

typedef struct
{
	uint32_t FrameNum;
	uint32_t LEDState;
	uint16_t Exp;
	uint16_t Gain;
	uint8_t  Acq_Index;
	uint16_t Bits;
} uID3_FrameHdr;		// sizeof() == 15
#define SIZEOF_uID3_FrameHdr 15

#if defined(__GNUC__) || defined(WIN32) || defined(_WIN32)
#pragma pack(pop)
#endif

#endif	// _strm_shared_h_
