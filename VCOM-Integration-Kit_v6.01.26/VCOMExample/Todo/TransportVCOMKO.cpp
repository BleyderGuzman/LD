/******************************<%BEGIN LICENSE%>******************************/
// (c) Copyright 2011 Lumidigm, Inc. (Unpublished Copyright) ALL RIGHTS RESERVED.
//
//
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


#include <errno.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>

#include "TransportVCOMKO.h"
#include "windows.h"
#include "VCOMCore.h"

#include "usbcmdset.h"

TransportVCOMKO::TransportVCOMKO(void)
{
	memset(hPhysicalDeviceID,0,2048);
	handleToClose = 0;
}
TransportVCOMKO::~TransportVCOMKO(void)
{
    close((int) ((intptr_t) handleToClose));
    handleToClose = 0;
}
// ITransport
uint TransportVCOMKO::Initialize(V100_DEVICE_TRANSPORT_INFO* pTransport)
{
    char deviceName[255];
    sprintf(deviceName,"/dev/vcom%d",pTransport->DeviceNumber);
    handleToClose = (void *) ((intptr_t) open(deviceName, O_RDWR|O_NDELAY,0));
    pTransport->hWrite = pTransport->hRead = handleToClose;

	if(pTransport->hWrite == (void*)-1)
	{
		fprintf(stdout,"\n (%s,%d) - Failed open.", __FILE__, __LINE__);
		return 5;//USB_WRITE_PIPE_ERROR; //should be read write error?
	}
		
	fprintf(stdout,"\n Transport for VCOM KO Initialized (%s,%d)", __FILE__, __LINE__);
	return 0;// USB_OK;
}

uint  TransportVCOMKO::Close(V100_DEVICE_TRANSPORT_INFO* pTransport)
{
	// fix this later, check log, may not be needed may happen automatically
	//	close((int)pTransport->hWrite);
	return 0;
}
bool TransportVCOMKO::GetDeviceId(V100_DEVICE_TRANSPORT_INFO* pTransport, char* szDeviceId, uint& nLength)
{
	nLength = 0;
	return false;
}


#define MSEC_TIMEOUT 15000

uint TransportVCOMKO::TransmitCommand(V100_DEVICE_TRANSPORT_INFO* pTransport, uchar* myPacket, uint nTxSize, uchar* pResponse, uint& nRxSize)
{
	bool	ret = true;
	// Fill out simple header
	USBCB usbcb;								// command block
	usbcb.ulCommand = 0x00;						// command
	usbcb.ulCount	= nTxSize;					// not used 
	usbcb.ulData	= 0x0;						// not used 
	nRxSize = 0;
	// Send Challenge Header Packet
	if (!WriteBytes(pTransport->hWrite, &usbcb, sizeof(usbcb), MSEC_TIMEOUT))
	{
		fprintf(stdout,"\n (%s,%d) - Failed write (usbcb).", __FILE__, __LINE__);
		
		return FALSE;
	}
	// Send Challenge Data Packet
	if (!WriteBytes(pTransport->hWrite, myPacket, usbcb.ulCount, MSEC_TIMEOUT))
	{
		fprintf(stdout,"\n (%s,%d) - Failed write (Packet).", __FILE__, __LINE__);
		return FALSE;
	}
	// Read Response Header Packet
	if (!ReadBytes(pTransport->hRead, &usbcb, sizeof(usbcb), MSEC_TIMEOUT))
	{
		fprintf(stdout,"\n (%s,%d) - Failed read (usbcb).", __FILE__, __LINE__);

		return FALSE;
	}
	if (!ReadBytes(pTransport->hRead, pResponse, usbcb.ulCount, MSEC_TIMEOUT))
	{
		fprintf(stdout,"\n (%s,%d) - Failed read (packet).", __FILE__, __LINE__);

		return FALSE;
	}
	if( pResponse[0] != 0x0D || pResponse[1] != 0x56 )
	{
		int breakme = 1;
		fprintf(stdout,"\n Break point %d hit (%s,%d).", breakme, __FILE__, __LINE__);
	}
	nRxSize = usbcb.ulCount;
	return ret;
}

bool TransportVCOMKO::WriteBytes(HANDLE hWrite, void* pPacket, uint nPacketSize, uint mSecTimeout)
{
	// Send X number of 64K packets	     
    int nIterations   = nPacketSize/MAX_DATA_BYTES_EZEXTENDER;
    int nRemainder    = nPacketSize%MAX_DATA_BYTES_EZEXTENDER;
    DWORD ulActualBytes = 0;

	if(nPacketSize == MAX_DATA_BYTES_EZEXTENDER)
	{
		nIterations = 0;
		nRemainder  = MAX_DATA_BYTES_EZEXTENDER;
	}

    for(int ii = 0 ; ii < nIterations ; ii++)
    { 
  	    char* pData = (char*)pPacket + ii*MAX_DATA_BYTES_EZEXTENDER;
  	    uint nElementCount = MAX_DATA_BYTES_EZEXTENDER;
  	     
	    if (!WriteBytes(hWrite, pData, nElementCount, mSecTimeout) )
	    {
		     return false;
		}
     }
     // Write remainder
     char* pData = (char*)pPacket + nIterations*MAX_DATA_BYTES_EZEXTENDER;
     uint nElementCount = nRemainder;

     bool  bTimeOut = false;

	int nTOcount = 0;

	while(!bTimeOut)
	{
		ulActualBytes = write((size_t)hWrite, pData, nElementCount);
		if((int)ulActualBytes == 0)
		{
			if(nTOcount++ >= 5000)//5 sec timeout
			{
				bTimeOut = true;
				fprintf(stdout, "\nError TransportVCOMKO::WriteBytes Timed out\n");
				break;
			}
			else
			{
				usleep(10);
				continue;
			}
		}
		else if((int)ulActualBytes == -1)
		{
			bTimeOut = true;
			perror("ERROR: TransportVCOMKO::WriteBytes:  ");
			break;
		}
		else
			break;
	}

	if(bTimeOut == true) 
	{
		return false;
	}

	return true;
}
/*
** Reads bytes, returns false on timeout or other error
*/
bool TransportVCOMKO::ReadBytes(HANDLE hRead, void* pPacket, uint nPacketSize, uint mSecTimeout)
{
	DWORD nRxSize;
	int nIterations   = nPacketSize/MAX_DATA_BYTES_EZEXTENDER;
	int nRemainder    = nPacketSize%MAX_DATA_BYTES_EZEXTENDER;
    	unsigned char * pRxBuffer = (unsigned char *)pPacket;
	    
	if(nPacketSize == MAX_DATA_BYTES_EZEXTENDER)
	{
		nIterations = 0;
		nRemainder  = MAX_DATA_BYTES_EZEXTENDER;
	}
	for(int ii = 0 ; ii < nIterations ; ii++)
    { 
  	     pRxBuffer = (unsigned char *)pPacket + ii*MAX_DATA_BYTES_EZEXTENDER;
		 if(FALSE == ReadBytes(hRead, pRxBuffer, MAX_DATA_BYTES_EZEXTENDER, mSecTimeout))
		 {
			return false;
		 }
    }
    // Read and time remainder, since this is the only thing that actually reads a pipe anyways.
	bool  bTimeOut = false;
    	pRxBuffer = (unsigned char *)pPacket + nIterations*MAX_DATA_BYTES_EZEXTENDER;

	int nTOcount = 0;

	while(!bTimeOut)
	{
		nRxSize = read((size_t)hRead, pRxBuffer, nRemainder);
		if(nRxSize == 0)
		{
		
			if(nTOcount++ >= 5000)//5 sec timeout
			{
				bTimeOut = true;
				fprintf(stdout, "\nError TransportVCOMKO::ReadBytes Timed out (%d bytes) \n", nRemainder);
				break;
			}
			else
			{
				usleep(10);
				continue;
			}
		}
		else if(nRxSize == (DWORD)-1)
		{
			bTimeOut = true;
			perror("ERROR: TransportVCOMKO::ReadBytes:  ");
			break;
		}
		else
			break;
	}

	// Check actual received size
    if( (nRxSize != (DWORD)nRemainder) && (bTimeOut == false) )
	 {
	 	printf("nRxSize:%d is not equal to nRemainder:%d",(int)nRxSize,(int)nRemainder);
		return false;
	 }
	if(bTimeOut == true) 
	{
		return false;
	}
	return true;
}

