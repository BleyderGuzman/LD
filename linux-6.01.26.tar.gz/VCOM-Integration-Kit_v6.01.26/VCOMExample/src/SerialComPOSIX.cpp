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
#include <stdio.h>
#include "SerialComPOSIX.h"
#include <fcntl.h> /* File control definitions */
#include <unistd.h> /* UNIX standard function definitions */
#include <termios.h> /* POSIX terminal control definitions */
#include <stdio.h> /* Standard input/output definitions */

SerialComPOSIX::SerialComPOSIX() {
}

SerialComPOSIX::~SerialComPOSIX() {
}


/////////////////////////////////////////////////////////////////////////////
// SerialCom message handlers
int setRaw(int fd);
BOOL SerialComPOSIX::OpenPort(char* port) 
{
    m_nfd = open(port, O_RDWR| O_NOCTTY);

    if(-1 == m_nfd)
    {
	perror("Could not open serial port");
        return false;
    }
    setRaw(m_nfd);
    return true;
}

BOOL SerialComPOSIX::ConfigureCOMPort(DWORD BaudRate, BYTE ByteSize, DWORD fParity, BYTE Parity, BYTE StopBits)
{
    struct termios options;

    if( 0 != tcgetattr(m_nfd, &options))
    {
        perror("tcgetattr failed.");   
        return false;
    }
    speed_t Baud;
    
    switch(BaudRate)
    {
        case 2400:      { Baud = B2400;}    break;
        case 9600:      { Baud = B9600;}    break;
        case 19200:     { Baud = B19200;}   break;
        case 38400:     { Baud = B38400;}   break;
        case 57600:     { Baud = B57600;}   break;
        case 115200:    { Baud = B115200;}  break;
        case 230400:    { Baud = B230400;}  break;
        default:        { return false; }   break;
    }
    cfsetispeed(&options, Baud);
    cfsetospeed(&options, Baud);
    
     // c_lflag
    options.c_lflag =  0; /* no local flags */ 
    options.c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG);
    
    // c_cflag
    options.c_cflag &= ~CRTSCTS;
    options.c_cflag |= CS8;
    options.c_cflag |= (CLOCAL | CREAD);
    options.c_cflag |= (IXON | IXOFF); /* Software flow control */
    options.c_cflag &= ~CSTOPB;
 
    // c_cc 
    options.c_cc[VMIN] = 0;
    options.c_cc[VTIME] = 1;
     
    // c_oflag
    options.c_oflag &= ~OPOST; /* No output processing */
    options.c_oflag &= ~ONLCR; /* Don't convert linefeeds */
    
    tcflush(m_nfd, TCIFLUSH);

    /* Update the options and do it NOW */
    if (tcsetattr(m_nfd, TCSANOW, &options) != 0) {
        perror("SetupSerial 3");
        return(0);
    }

    return true;
}
int setRaw(int fd) {
/* set tty into raw mode */

    struct termios    tty_state;
    struct termios    ttystate_orig;

    if (tcgetattr(fd, &tty_state) < 0) return 0;
    /* save off original settings */
    ttystate_orig = tty_state;
    /* set raw mode */
    tty_state.c_lflag &= ~(ICANON | IEXTEN | ISIG | ECHO);
    tty_state.c_iflag &= ~(ICRNL | INPCK | ISTRIP | IXON | BRKINT);
    tty_state.c_oflag &= ~OPOST;
    tty_state.c_cflag |= CS8;	
    tty_state.c_cc[VMIN]  = 1;
    tty_state.c_cc[VTIME] = 0;
    cfsetispeed(&tty_state, B9600);
    cfsetospeed(&tty_state, B9600);
    if (tcsetattr(fd, TCSAFLUSH, &tty_state) < 0) return 0;

    return 1;
}
BOOL SerialComPOSIX::SetCommunicationTimeouts(DWORD ReadIntervalTimeout, DWORD ReadTotalTimeoutMultiplier, DWORD ReadTotalTimeoutConstant, DWORD WriteTotalTimeoutMultiplier, DWORD WriteTotalTimeoutConstant) {

    return true;
}

BOOL SerialComPOSIX::ReadBytes(BYTE* bybyte, unsigned int nSize)
{
    iBytesRead = 0;
    ssize_t res  = 0;

    while(iBytesRead < nSize)
    {
	bybyte += res;
	
	res = read(m_nfd, bybyte, nSize);

	if(-1 == res)
	{
	    printf("SerialComPOSIX::ReadBytes -- Failed!\n");
	    return false;
	}

	iBytesRead += res;
    }

    return true;
}
BOOL SerialComPOSIX::WriteBytes(const BYTE* bybyte, unsigned int nSize)
{
    iBytesWritten = 0;
    iBytesWritten = write(m_nfd, bybyte, nSize);
    //printf("SerialComPOSIX::WriteBytes -- bytes written %d (should be: %d)\n", iBytesWritten, nSize);
    //return ((iBytesWritten != 0) && (iBytesWritten == nSize))?true:false;
    if((iBytesWritten == 0) || (iBytesWritten != nSize))
    { 
        printf("SerialComPOSIX::WriteBytes -- Failed! wrote %d bytes (should have been: %d)\n", (int)iBytesWritten, nSize);
        return false;
    }

    return true;
}


BOOL SerialComPOSIX::WriteByte(BYTE bybyte) 
{
   return write(m_nfd, &bybyte, 1) == 1;
   //int response = write(m_nfd, &bybyte, 1);
   //if(response == 0) return false;
   //return true;
}

BOOL SerialComPOSIX::ReadByte(BYTE &resp)
 {
    char rx = 0;
    int response = read(m_nfd, &rx, 1);
    resp = rx;
    if(response == 0) return false;
    return true;
}


void SerialComPOSIX::ClosePort() {
    close(m_nfd);
}
