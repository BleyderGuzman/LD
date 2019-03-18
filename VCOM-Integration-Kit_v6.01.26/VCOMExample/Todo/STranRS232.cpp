
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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "STranRS232.h"
#ifdef __GNUC__
	#include "SerialComPOSIX.h"
	#define NOPARITY    0
	#define ONESTOPBIT  0
#else
	#include "SerialCom.h"
#endif


STranRS232::STranRS232()
{

}
STranRS232::~STranRS232()
{

}

bool STranRS232::OpenPort(Comport cport, uint baudrate, uint parity, uint timeout)
{
	#ifdef _WIN32
		sprintf_s(comStrBuffer, 255, "COM%d", (int(cport)+1));
	#else
		sprintf(comStrBuffer,"/dev/ttyS%d", (int)(cport));
	#endif

	if(false == com.OpenPort(comStrBuffer)) return false;
	if(false == com.ConfigureCOMPort(baudrate,8,FALSE,NOPARITY,ONESTOPBIT)) return false;
	if(false == com.SetCommunicationTimeouts(100, 100, 100, 100, 100)) return false;
 
	return true;
}

bool STranRS232::ClosePort()
{
    com.ClosePort();
    return true;
}

bool STranRS232::Transmit(uchar pBuffer)
{
	return (FALSE != com.WriteByte(pBuffer))?true:false;
}

bool STranRS232::Transmit(const unsigned char *src, int srcsz)
{
	return (FALSE != com.WriteBytes(src,srcsz))?true:false;
}

bool STranRS232::Receive(unsigned char *dest, int destsz)
{
	return (FALSE != com.ReadBytes(dest,destsz))?true:false;
}
