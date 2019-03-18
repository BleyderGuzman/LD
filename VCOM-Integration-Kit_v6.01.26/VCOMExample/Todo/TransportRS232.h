
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

#include "ITransport.h"

class XModem;

class TransportRS232 : public ITransport
{
public:
	TransportRS232(void);
	~TransportRS232(void);
	// ITransport
	uint Initialize(V100_DEVICE_TRANSPORT_INFO* pTransport);
	uint Close(V100_DEVICE_TRANSPORT_INFO* pTransport);
	uint TransmitCommand(V100_DEVICE_TRANSPORT_INFO* pTransport, const uchar* myPacket, uint nTxSize, uchar* pResponse, uint& nRxSize);
	bool GetDeviceId(V100_DEVICE_TRANSPORT_INFO* pTransport, char* szDeviceId, uint& nLength);
private:
	XModem* m_pXmodem;
};
