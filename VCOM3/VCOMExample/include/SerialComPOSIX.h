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
#pragma once

#include "windows.h"

class SerialComPOSIX 
{
// Construction
public:
	SerialComPOSIX();
	virtual ~SerialComPOSIX();
public:
	void  ClosePort();
	BOOL  ReadByte(BYTE &resp);
	BOOL  WriteByte(BYTE bybyte);
	BOOL  WriteBytes(const  BYTE* bybyte, unsigned int nSize);
	BOOL  ReadBytes(BYTE* bybyte, unsigned int nSize);
	BOOL  OpenPort(char* portname);
	BOOL  SetCommunicationTimeouts(DWORD ReadIntervalTimeout,DWORD ReadTotalTimeoutMultiplier,DWORD ReadTotalTimeoutConstant,DWORD WriteTotalTimeoutMultiplier,DWORD WriteTotalTimeoutConstant);
	BOOL  ConfigureCOMPort(DWORD BaudRate,BYTE ByteSize,DWORD fParity,BYTE  Parity,BYTE StopBits);
	BOOL  m_bPortReady;
	BOOL  bWriteRC;
	BOOL  bReadRC;
	DWORD iBytesWritten;
	DWORD iBytesRead;
	DWORD dwBytesRead;
private:
        int   m_nfd;
};


