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
#include "windows.h"
#include "TransportSE.h"
#include <stdio.h>
#include <string.h>
#include "VCOMCore.h"
#include "IXServiceProvider.h"
#include "Device.h"
#include "V100DeviceHandler.h"
#include "usbcmdset.h"

TransportSE::TransportSE(void)	:
	m_pStreamDvc(NULL)
{}

TransportSE::~TransportSE(void)
{
	if (m_pStreamDvc)
		delete static_cast<IStreamDvc*>(m_pStreamDvc);
}

// pure virtual derived of ITransport
extern IStreamDvc * SEngineGetStreamDvc(int);
uint TransportSE::Initialize(V100_DEVICE_TRANSPORT_INFO* pTransport)
{
	IStreamDvc * pStreamDvc = SEngineGetStreamDvc(COMM_PIPE);
	pStreamDvc->ConnectStream("VCOM client", NULL, ClientMode, 0);
	m_pStreamDvc = pStreamDvc;
	return 0;
}

// pure virtual derived of ITransport
uint TransportSE::Close(V100_DEVICE_TRANSPORT_INFO* pTransport)
{
	extern void SEngineCore_shutdown();
	SEngineCore_shutdown();
	return 0;
}

// pure virtual derived of ITransport
bool TransportSE::GetDeviceId(V100_DEVICE_TRANSPORT_INFO* pTransport, char* szDeviceId, uint& nLength)
{
	*szDeviceId = '\0';
	nLength = 0;
	return true;
}

/*
** pure virtual derived of ITransport
** performs the four-step transmit/receive sequence for sending a command and getting the result.
** returns: positive byte count success
**          0 failure/timeout
*/
uint TransportSE::TransmitCommand(V100_DEVICE_TRANSPORT_INFO* pTransport, const uchar* myPacket, uint nTxSize, uchar* pResponse, uint& nRxSize)
{
	static const uint umSecTimeout = 15 * 1000;
	//fprintf(stderr, "TransportSEPOSIX::TransmitCommand BEGIN\n");
	bool	ret = true;
	// Fill out simple header
	USBCB usbcb;								// command block
	usbcb.ulCommand = 0x00;						// command
	usbcb.ulCount	= nTxSize;					// not used 
	usbcb.ulData	= 0x0;						// not used 
	nRxSize = 0;
	// Send Challenge Header Packet
	if (!WriteBytes(pTransport->hWrite, &usbcb, sizeof(usbcb), umSecTimeout))
	{
        fprintf(stderr, "TransportSEPOSIX::TransmitCommand FAIL phase 1\n");
		return 0;
	}
	// Send Challenge Data Packet
	if (!WriteBytes(pTransport->hWrite, myPacket, usbcb.ulCount, umSecTimeout))
	{
        fprintf(stderr, "TransportSEPOSIX::TransmitCommand FAIL phase 2\n");
		return 0;
	}
	// Read Response Header Packet
    uint s = sizeof(usbcb);
	if (!ReadBytes(pTransport->hRead, &usbcb, s, umSecTimeout) && s == sizeof(usbcb))
	{
        fprintf(stderr, "TransportSEPOSIX::TransmitCommand FAIL phase 3\n");
		return 0;
	}
	// Read Response Data Packet
	if (!ReadBytes(pTransport->hRead, pResponse, nRxSize, umSecTimeout))
	{
        fprintf(stderr, "TransportSEPOSIX::TransmitCommand FAIL phase 4\n");
		return 0;
	}
	//fprintf(stderr, "TransportSEPOSIX::TransmitCommand END RxSize %d\n", nRxSize);
	return ret;
}

/*
** Writes bytes, returns false on timeout or other error
*/
bool TransportSE::WriteBytes(HANDLE hWrite, const void* pPacket, uint nPacketSize, uint mSecTimeout)
{
	IStreamDvc * pStreamDvc = static_cast<IStreamDvc*>(m_pStreamDvc);
	if (!pStreamDvc)
	{
		fprintf(stderr, "TransportSEPOSIX::WriteBytes: unable to get streaming device\n");
		return false;
	}
	if (pStreamDvc->WriteStream((unsigned char*) pPacket, nPacketSize, mSecTimeout) != TRANSFER_OK)
	{
		fprintf(stderr, "TransportSEPOSIX::WriteBytes: unable to write streaming device\n");
		delete pStreamDvc;
		return false;
	}
	//fprintf(stderr, "TransportSEPOSIX::WriteBytes %d OK\n", nPacketSize);
	return true;
}

/*
** Reads bytes, returns false on timeout or other error
*/
bool TransportSE::ReadBytes(HANDLE hRead, void* pPacket, uint & nPacketSize, uint mSecTimeout)
{
	IStreamDvc * pStreamDvc = static_cast<IStreamDvc*>(m_pStreamDvc);
	if (!pStreamDvc)
	{
		fprintf(stderr, "TransportSEPOSIX::ReadBytes: unable to get streaming device\n");
		return false;
	}
	if (pStreamDvc->ReadStream((unsigned char*) pPacket, nPacketSize, mSecTimeout) != TRANSFER_OK)
	{
		fprintf(stderr, "TransportSEPOSIX::ReadBytes: unable to read streaming device\n");
		return false;
	}
	//fprintf(stderr, "TransportSEPOSIX::ReadBytes %d OK\n", nPacketSize);
	return true;
}
