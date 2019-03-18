
/***************************************************************************************/
// ©Copyright 2016 HID Global Corporation/ASSA ABLOY AB. ALL RIGHTS RESERVED.
//
// For a list of applicable patents and patents pending, visit www.hidglobal.com/patents/
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR 
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS 
// FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
// COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER 
// IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN 
// CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
//
//
/***************************************************************************************/

#pragma once

#include <stdio.h>

#ifdef __GNUC__
	#include "SerialComPOSIX.h"
#else
	#include "SerialCom.h"
#endif

#define SOH  0x01
#define STX  0x02
#define EOT  0x04
#define ACK  0x06
#define NAK  0x15
#define CAN  0x18
#define CTRLZ 0x1A

#define DLY_1S 10
#define MAXRETRANS 25

typedef unsigned char  uchar;
typedef unsigned short ushort;
typedef unsigned int   uint;

typedef enum
{
	COM1 = 0,
	COM2,
	COM3,
	COM4,
	COM5,
	COM6,
	COM7,
	COM8,
	COM9,
	COM10
} Comport ;

class STranRS232
{
public:
	// Constructor
	STranRS232();
	virtual ~STranRS232();
	bool	OpenPort(Comport cport, uint baudrate, uint parity, uint timeout);
	bool	ClosePort();
	// Transmit single byte
	bool	Transmit(uchar pBuffer);
	// Overload - transmit multiple bytes
	bool	Transmit(const uchar* pBuffer, int nBytes);
	// Receive multiple bytes.
	bool	Receive(uchar* pBuffer, int nBytes);

private:

#ifdef __GNUC__
	SerialComPOSIX com;
#else
	SerialCom com;
#endif

	char comStrBuffer[255];

};

