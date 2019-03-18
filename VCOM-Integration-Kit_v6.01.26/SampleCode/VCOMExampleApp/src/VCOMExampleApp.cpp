/******************************************************************************
**  You are free to use this example code to generate similar functionality  
**  tailored to your own specific needs.  

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
****************************************************************************************/
// VCOMExampleApp.cpp : Defines the entry point for the console application. The 
//                      application provides sample code for general VCOM commands 
//                      and an example of using the 1:N Identification VCOM commands. 
// 
// Note: This application is designed to compile and run on different platforms,
//       specifically, Win32, WinCE and Linux. Platform-specific code is compiled
//       in/out as needed.
//  
// 
// SetupDeviceConnection: Sets up the device handle and establish communication 
//                        with the device.
// RunCaptureAndMatchTest Demonstrates how to perform 2 captures and a match.
// RunAPITest:            Demonstrates general VCOM API commands such as Capture, 
//                        Match, Verify images, getting command and
//                        configuration structures.
// RunIDTest:             Demonstrates 1:N Identification commands
// RunVerifyTest:         Demonstrates Verify using an external database.
// WaitForOperationCompletion: Wait for long operations.
// 
#ifdef __GNUC__
#include "windows.h"    // yes, this is required - do not remove.
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <iostream>

#ifndef __GNUC__
#define SLEEP_TIME 100
#include "afxwin.h"
#define ASCII_ENTER_CODE 13
#ifndef _WIN32_WCE
#include "conio.h"
#endif
#else
    #define SLEEP_TIME 1
    #define ASCII_ENTER_CODE 10
	#include <termios.h>
	#include <unistd.h>
#endif

#include "VCOMCore.h"
#include "VersionInfo.h"

using namespace std;


/////////////////////////////////////////////////////////////////////////////////
// #defines
/////////////////////////////////////////////////////////////////////////////////
#define M30X_PROD_ID 8492
#define V30X_PROD_ID 13348
#define M31X_PROD_ID 65
#define V31X_PROD_ID 33


/////////////////////////////////////////////////////////////////////////////////
// Constant definitions
/////////////////////////////////////////////////////////////////////////////////
const char* SELECT_CONNECT_MENU =
{ 
	"\nSelect the communication method to use:\
	 \n 1)  USB\
	 \n 2)  RS-232\
	 \n <Q> Quit\
	 \n\n" 
};
const char* SELECT_COM_INDEX_MENU = 
{
	"\nEnter the COM port number to use:\
	\n(hit the enter key after typing the COM port number)\n"
};
const char* SELECT_TEST_MENU = 
{
	"\n\nSelect a Test Case\
	\n-----------------------------------------------\
	\n 1)  Run Capture and Match Test\
	\n 2)  Run API Test\
	\n <Q> Quit\n\n"
};

const char* SELECT_TEST_MENU_IDTEST = 
{
	"\n\nSelect a Test Case\
	\n-----------------------------------------------\
	\n 1)  Run Capture and Match Test\
	\n 2)  Run API Test\
	\n 3)  Run Identification Test\
	\n 4)  Run Verification Test\
	\n <Q> Quit\n\n"
};

/////////////////////////////////////////////////////////////////////////////////
// enums
/////////////////////////////////////////////////////////////////////////////////
const char* DeviceTypeStr[9] =
{
	"UNKNOWN_LUMI_DEVICE",
	"VENUS_V30X",
	"MERCURY_M30X",
	"MERCURY_M31X",
	"VENUS_V31X",
	"VENUS_V371",
	"VENUS_V40X",
	"VENUS_V42X",
	"MERCURY_M32X"
};

_V100_SENSOR_TYPE g_nDeviceType;
bool g_bFingerLift;

/////////////////////////////////////////////////////////////////////////////////
// Function Prototypes
/////////////////////////////////////////////////////////////////////////////////
bool SetupDeviceConnection(V100_DEVICE_TRANSPORT_INFO* pDev, bool bSetupForRS232);
void CloseDeviceConnection(V100_DEVICE_TRANSPORT_INFO* pDev);
bool ChooseTransport(V100_DEVICE_TRANSPORT_INFO* pDev);
bool ChooseCOMIndex(int& nCOMIndex);
bool ChooseAndRunTest(V100_DEVICE_TRANSPORT_INFO* pDev);
// Test Cases
bool RunOnWinCE(V100_DEVICE_TRANSPORT_INFO* pDev);
bool RunCaptureAndMatchTest(V100_DEVICE_TRANSPORT_INFO* pDev);
bool RunAPITest(V100_DEVICE_TRANSPORT_INFO* pDev);
bool RunIDTest(V100_DEVICE_TRANSPORT_INFO* pDev);
bool RunVerifyTest(V100_DEVICE_TRANSPORT_INFO* pDev);
// Helper Functions
bool Capture(V100_DEVICE_TRANSPORT_INFO *pDev,
							  uchar* pCompositeImage,
							  uint&	 nWidth,
							  uint&  nHeight,
							  uchar* pTemplate,
							  uint&	 nTemplateSize,
							  int&	 nSpoof,
							  int	 getComposite,
							  int	 getTemplate);
_V100_OP_ERROR WaitForOperationCompletion(V100_DEVICE_TRANSPORT_INFO* pDev);
void PrintReturnCode(V100_ERROR_CODE rc);
void PrintOPReturnCode(_V100_OP_ERROR rc);
void PrintConfig(_V100_INTERFACE_CONFIGURATION_TYPE *config, uint serial_number);
void PrintDBMetrics(_MX00_DB_METRICS* dbMetrics);
// Multi-platform handlers
void SleepVIK(int nMilliSecToSleep);
int  GetUserInput(void);

/////////////////////////////////////////////////////////////////////////////////
// Main function
/////////////////////////////////////////////////////////////////////////////////
#ifdef __GNUC__
int main(int argc, char * argv[])
#else
int _tmain(int argc, _TCHAR* argv[])
#endif
{
	// Initialize the device handle
	V100_DEVICE_TRANSPORT_INFO Dev;	
	memset(&Dev,0,sizeof(V100_DEVICE_TRANSPORT_INFO));

	fprintf(stdout,"\n\nVCOM Integration Example - Version %s", VersionInfo);
	fprintf(stdout,"\n-----------------------------------------------");
	
	g_nDeviceType = UNKNOWN_LUMI_DEVICE;
	g_bFingerLift = false;

#ifdef _WIN32_WCE
	if(false == RunOnWinCE(&Dev))
	{
		fprintf(stdout,"\n\n WinCE Tests failed!");
	}
	goto Exit;
#endif	

	// Choose transport (USB or RS232)
	if(false == ChooseTransport(&Dev))
	{
		goto Exit;
	}
	
	// Choose which test to run
	if(false == ChooseAndRunTest(&Dev))
	{
		goto Exit;
	}

 
Exit:
	CloseDeviceConnection(&Dev);
	printf("\n\nPress Enter to continue...");
	GetUserInput();
	return 0;
}

bool ChooseTransport(V100_DEVICE_TRANSPORT_INFO* pDev)
{
	bool bValidSelection = false;
	while(false == bValidSelection)
	{
		fprintf(stdout, "%s", SELECT_CONNECT_MENU);
		int ch = GetUserInput();
		switch(ch)
		{
			case '1': // USB
				{
					bValidSelection = true;
					if(false == SetupDeviceConnection(pDev, false)) { return false; }					
				} break;
			case '2': // RS232
				{
					bValidSelection = true;
					if(false == SetupDeviceConnection(pDev, true)) { return false; }
				} break;
			case 'q':
			case 'Q': // Quit
				{
					return false;
				} break;
			default:
				{
					fprintf(stdout, "Invalid Option! Please try again.\n");
				} break;
		}
	}	

    return true;
}

bool ChooseAndRunTest(V100_DEVICE_TRANSPORT_INFO* pDev)
{
	// Determine the test to run
	bool bQuit = false;
	while(false == bQuit)
	{		
		if(g_nDeviceType >= MERCURY_M30X) //Not VENUS_NS
		{	
			fprintf(stdout, "%s", SELECT_TEST_MENU_IDTEST); 
		}
		else if(g_nDeviceType == VENUS_V30X)
		{	
			_V100_INTERFACE_CONFIGURATION_TYPE config;
			V100_Get_Config(pDev, &config);
			if(config.ID_Rev == 0)
			{
				fprintf(stdout, "%s", SELECT_TEST_MENU);
			}
			else //V300 that supports ID
			{
				fprintf(stdout, "%s", SELECT_TEST_MENU_IDTEST); 
			}
		}
		else
		{
			fprintf(stdout, "%s", SELECT_TEST_MENU);
		}		
		int ch = GetUserInput();
		switch(ch)
		{
		case '1':
			{
				if(false == RunCaptureAndMatchTest(pDev))
				{
					fprintf(stdout,"\n\n Capture and Match Test failed!");
					return false;
				}
			} break;
		case '2':
			{
				if(false == RunAPITest(pDev))
				{
					fprintf(stdout,"\n\n API Test failed!");
					return false;
				}
			} break;
		case '3':
			{
				if(g_nDeviceType == UNKNOWN_LUMI_DEVICE) 
				{ 
					fprintf(stdout, "Invalid Option! Please try again.\n"); // Invalid if sensor is a V30x
					continue; 
				}
				else if(g_nDeviceType==VENUS_V30X)
				{
					_V100_INTERFACE_CONFIGURATION_TYPE config;
					/*V100_ERROR_CODE rc =*/ V100_Get_Config(pDev, &config);
					if(config.ID_Rev == 0)
					{
						fprintf(stdout, "Invalid Option! Please try again.\n"); // Invalid if no ID available
						continue;
					}
				}

				if(false == RunIDTest(pDev))
				{
						fprintf(stdout,"\n\nIdentification Test failed!");
						return false;
				}
			} break;
		case '4':
			{
				if(g_nDeviceType==VENUS_V30X)
				{
					_V100_INTERFACE_CONFIGURATION_TYPE config;
					/*V100_ERROR_CODE rc =*/ V100_Get_Config(pDev, &config);
					if(config.Firmware_Rev <= 20292)
					{
						fprintf(stdout, "Invalid Option! Please try again.\n"); // Invalid if Firmware is less than 20292
						continue;
					}
				}

				if(false == RunVerifyTest(pDev))
				{
					fprintf(stdout,"\n\n Verify Test failed!");
					return false;
				}
			} break;
		case 'q':
		case 'Q':
			{
				bQuit = true;
			} break;
		default:
			{
				fprintf(stdout, "Invalid Option! Please try again.\n");
			} break;
		}
	}	
	return true;
}


bool SetupDeviceConnection(V100_DEVICE_TRANSPORT_INFO* pDev, bool bSetupForRS232)
{
	V100_ERROR_CODE rc = GEN_OK;

	if(bSetupForRS232) // RS232
	{
		pDev->nBaudRate = 9600;
		int nCOMIndex = 1;
		if(false == ChooseCOMIndex(nCOMIndex)) { return false; }
		pDev->nCOMIndex = nCOMIndex;	
	}
	else // USB
	{
		// Query number of devices connected
		int nNumDevices = 0;
		fprintf(stdout, "\nV100_Get_Num_USB_Devices:  ");
		rc =  V100_Get_Num_USB_Devices(&nNumDevices);
		PrintReturnCode(rc);
		// If no devices found, return and display error message
		if(nNumDevices == 0)
		{
			fprintf(stdout,"\nNo devices found\n");
			return false;
		}
	}

	// Open the device
	fprintf(stdout, "\nV100_Open:  ");
	rc = V100_Open(pDev);
	PrintReturnCode(rc);

	// If cannot open the device, return and display error message
	if( rc != GEN_OK)
	{
		fprintf(stdout, "\n Cannot open the device\n");
		return false;
	}	

	// Get Configuration Structure 
	_V100_INTERFACE_CONFIGURATION_TYPE config;
	fprintf(stdout, "\nV100_Get_Config:  ");
	rc = V100_Get_Config(pDev, &config);
	PrintReturnCode(rc);
	
	// If cannot get Configuration structure, return
	if( rc != GEN_OK)
	{
		fprintf(stdout, "\n Cannot get configuration from the device\n");
		return false;
	}

	// Get Serial Number
	// NOTE: The firmware field in the config structure is defined as a short. Obviously, this does not support large serial 
	//       number values (over 65535) so the number obtained by config.Device_Serial_Number may not match what is on the
	//       label of the snesor. The V100_Get_Serial command has been added to retrieve the "correct" serial number from the
	//		 device. Earlier firmware revisions on V30x and M30x sensors do not support the V100_Get_Serial command so just 
	//       use the number reported from config.Device_Serial_Number and don't bother checking the return code. 
	uint nSerialNum = config.Device_Serial_Number;

	V100_Get_Serial(pDev, &nSerialNum);
	
	PrintConfig(&config, nSerialNum);

	// Set the device type
	fprintf(stdout, "\nV100_Get_Sensor_Type:");
	rc = V100_Get_Sensor_Type(pDev, g_nDeviceType);
	PrintReturnCode(rc);
	if(rc != GEN_OK)
	{
		fprintf(stdout, "\n Cannot do V100_Get_Sensor_Type\n");
		return false;
	}

	switch(g_nDeviceType)
	{
		case VENUS_V30X:
			g_bFingerLift = false;
			break;
		case MERCURY_M30X:
			if(config.Firmware_Rev < 16048)	g_bFingerLift = false;
			else g_bFingerLift = true;
			break;
		case MERCURY_M31X:
			g_bFingerLift = true;
			break;
		case VENUS_V31X:
			g_bFingerLift = true;
			break;
		case VENUS_V371:
			g_bFingerLift = true;
			break;
		case VENUS_V40X:
			g_bFingerLift = false;
			break;
		case VENUS_V42X:
			g_bFingerLift = false;
			break;
		case MERCURY_M32X:
			g_bFingerLift = true;
			break;
		default:
			fprintf(stdout, "\n Cannot do V100_Get_Sensor_Type\n");
			return false;
	}

	cout << endl << "Sensor Type: ....................... " << DeviceTypeStr[g_nDeviceType] << endl;

	if(bSetupForRS232)
	{
		// Set the device with new baudrate
		fprintf(stdout, "\n\nSetting device with new baudrate 57600...");
		// Config Comport
		uint BaudRate = 57600;
		fprintf(stdout, "\nV100_Config_Comport:  ");
		rc =  V100_Config_Comport(pDev, BaudRate);
		PrintReturnCode(rc);

		//If cannot configure comport return and display error message
		if( rc != GEN_OK)
		{
			fprintf(stdout, "\n Cannot config the comport\n");
			return false;
		}

		// Reboot the device with new baudrate
		fprintf(stdout, "\nV100_Close:  ");
		rc = V100_Close(pDev);
		PrintReturnCode(rc);

		pDev->nBaudRate = BaudRate;

		fprintf(stdout, "\nV100_Open:  ");
		rc = V100_Open(pDev);
		PrintReturnCode(rc);

		// If cannot open the device, return and display error message
		if( rc != GEN_OK)
		{
			fprintf(stdout, "\nCannot reopen the device\n");
			return false;
		}

		SleepVIK(1000);
	}

	// Get Command Structure
	_V100_INTERFACE_COMMAND_TYPE cmd;
	fprintf(stdout, "\nV100_Get_Cmd:  ");
	rc = V100_Get_Cmd(pDev, &cmd);
	PrintReturnCode(rc);
	if(rc != GEN_OK)
	{
		fprintf(stdout, "\nCannot get command structure\n");
		return false;
	}

	// Set preprocessing, extraction, matching
	// Turn on all on board processing
	cmd.Select_PreProc    = 1;
	cmd.Select_Matcher    = 1;
	cmd.Select_Extractor  = 1;
	cmd.Override_Trigger  = 0;

	// Set the Command structure with all on board processing turned on
	fprintf(stdout, "\nV100_Set_Cmd:  ");
	rc = V100_Set_Cmd(pDev, &cmd);
	PrintReturnCode(rc);
	if(rc != GEN_OK)
	{
		fprintf(stdout, "\nCannot set command structure\n");
		return false;
	}

	if(g_nDeviceType == VENUS_V30X && config.Firmware_Rev > 20292)
	{
		g_bFingerLift = true;
		_V100_OPTION_TYPE OptType = OPTION_SET_LATENT_DETECTION_MODE;
		_V100_LATENT_DETECTION_MODE LatentType = LATENT_DETECTION_OFF;
		rc = V100_Set_Option(pDev, OptType,(uchar *)(&LatentType), sizeof(_V100_LATENT_DETECTION_MODE));
		if(rc != GEN_OK)
		{
			fprintf(stdout, "\nCannot set Option\n");
			return false;
		}
	}

	if(g_nDeviceType == VENUS_V31X || g_nDeviceType == VENUS_V371)
	{
		// For legacy reasons turning off latent detection and using finger lift mode
		_V100_OPTION_TYPE OptType = OPTION_SET_LATENT_DETECTION_MODE;
		_V100_LATENT_DETECTION_MODE LatentType = LATENT_DETECTION_OFF;
		rc = V100_Set_Option(pDev, OptType,(uchar *)(&LatentType), sizeof(_V100_LATENT_DETECTION_MODE));
		if(rc != GEN_OK)
		{
			fprintf(stdout, "\nCannot set Option\n");
			return false;
		}
	}

	return true;
}

// Reboot device if necessary
void CloseDeviceConnection(V100_DEVICE_TRANSPORT_INFO* pDev)
{
	V100_ERROR_CODE rc = GEN_OK;

    //Reset device if RS232
	if(pDev->nBaudRate > 0)
	{
		fprintf(stdout, "\n\nV100_Reset:  ");
		rc = V100_Reset(pDev);
		PrintReturnCode(rc);

		fflush(stdout);
		// Give some time for the sensor to reboot in case the test is run again quickly
		SleepVIK(3000);
	}

	V100_Close(pDev);
}

bool ChooseCOMIndex(int& nCOMIndex)
{
	string strCOMIndex;
	int c = 0;
	fprintf(stdout, "%s", SELECT_COM_INDEX_MENU);
	do
	{	
		c = GetUserInput();
		switch(c)
		{
			case 0:			
			{
				// 0 is bad imput
			}break;
			default:
			{
				strCOMIndex += c;
				cout << (char)c; // Print out what was typed
			}break;
		}
	}while(c != ASCII_ENTER_CODE);
	nCOMIndex = atoi(strCOMIndex.c_str());
	if(nCOMIndex == 0)
	{
		return false; // atoi failed due to non number first character or a 0 was inputed, either way, return false;
	}
	return true;
}

bool RunOnWinCE(V100_DEVICE_TRANSPORT_INFO* pDev)
{
	g_nDeviceType = VENUS_V30X;

	// Note: RS-232 is not supported in WinCE
	if(false == SetupDeviceConnection(pDev, false))
	{
		fprintf(stdout, "\n Unable to establish connection to the device!");
		return false;
	}

	if(g_nDeviceType > MERCURY_M30X) // Not V300 or M300
	{
		fprintf(stdout,"\n\n The connected sensor is not supported on this platform!");
		return false;
	}
	
	// Run all tests sequentially on WinCE
	if(false == RunAPITest(pDev))
	{
		fprintf(stdout,"\n\n API Test failed!");
		return false;
	}

	if(g_nDeviceType == MERCURY_M30X)
	{
		if(false == RunIDTest(pDev))
		{
			fprintf(stdout,"\n\nIdentification Test failed!");
			return false;
		}
	}

	return true;
}

bool RunCaptureAndMatchTest(V100_DEVICE_TRANSPORT_INFO* pDev)
{
	// Initialize variables	
	_V100_INTERFACE_CONFIGURATION_TYPE	config;
	V100_ERROR_CODE						rc;
	uchar*	pCompositeImage1 = NULL;
	uchar*	pCompositeImage2 = NULL;
	uchar	pTemplate1[2048];
	uchar   pTemplate2[2048];
	uint	nWidth;
	uint	nHeight;
	uint	nTemplateSize1, nTemplateSize2;
	uint	Score;
	int		nSpoof;
	bool    retCode = true;
	int		nGetCompImg = 0;

	fprintf(stdout, "\n\n--- Running Capture and Match Test ---\n");

	if(pDev->nBaudRate > 0)
	{
		nGetCompImg = 0; // Do not get composite image over RS232 - too slow
	}
	else
	{
		nGetCompImg = 1; // Get composite image over USB
	}


	// Get Config Structure
	fprintf(stdout, "\nV100_Get_Config:  ");
	rc = V100_Get_Config(pDev, &config);
	PrintReturnCode(rc);
	if(rc != GEN_OK)
	{
		fprintf(stdout, "\nError occured getting config structure\n");
		retCode = false;
		goto Abort;
	}

	// Initialize width and height of Composite image
	nWidth = config.Composite_Image_Size_X;
	nHeight = config.Composite_Image_Size_Y;
	
	pCompositeImage1 = new uchar[nWidth*nHeight];
	pCompositeImage2 = new uchar[nWidth*nHeight];


	// Capture first image and get template
	fprintf(stdout,"\n\nFirst Capture...  Place finger on sensor");	
	if(false == Capture(pDev, pCompositeImage1, nWidth, nHeight, pTemplate1, nTemplateSize1, nSpoof, nGetCompImg, 1))
	{
		goto Abort;
	}

	// Capture second image and get template
	fprintf(stdout,"\n\nSecond Capture...  Place finger on sensor");
	if(false == Capture(pDev, pCompositeImage2, nWidth, nHeight, pTemplate2, nTemplateSize2, nSpoof, nGetCompImg, 1))
	{
		goto Abort;
	}

	

	// Match templates
	fprintf(stdout,"\n\nMatching...");
	fprintf(stdout, "\nV100_Match:  ");
	rc = V100_Match(pDev, pTemplate1, nTemplateSize1, pTemplate2, nTemplateSize2, Score);
	
	PrintReturnCode(rc);
	fprintf(stdout,"\nMatch Score %d\n\n",Score);


Abort:	
	delete[] pCompositeImage1;
	delete[] pCompositeImage2;
	return retCode;
}

bool RunAPITest(V100_DEVICE_TRANSPORT_INFO* pDev)
{
	// Initialize variables	
	_V100_INTERFACE_CONFIGURATION_TYPE	config;
	V100_ERROR_CODE						rc;
	_V100_LED_CONTROL					control;
	_V100_OPTION_TYPE					OptType;
	_V100_PRESENCE_DETECTION_TYPE		PdType;
	_V100_SENSOR_TYPE sensorType = UNKNOWN_LUMI_DEVICE;

	uchar*	pCompositeImage1 = NULL;
	uchar*	pCompositeImage2 = NULL;
	uchar	pTemplate1[2048];
	uchar   pTemplate2[2048];

	uint	nWidth;
	uint	nHeight;
	uint	nTemplateSize1, nTemplateSize2;
	uint	Score;
	uint	nImageSize;
    uint nW;
    uint nH;
    uchar *pCompositeImage = NULL;
    uint nTemplateSize;
    uchar* pTemplate = NULL;
	int		nSpoof;
	bool    retCode = true;
	
	DWORD dwStartTime = 0;

	fprintf(stdout, "\n\n--- Running API Test ---\n");

	// Get Config Structure
	fprintf(stdout, "\nV100_Get_Config:  ");
	rc = V100_Get_Config(pDev, &config);
	PrintReturnCode(rc);
	if(rc != GEN_OK)
	{
		fprintf(stdout, "\nError occured getting config structure\n");
		retCode = false;
		goto Abort;
	}

	rc = V100_Get_Sensor_Type(pDev, sensorType);
	if(rc != GEN_OK)
	{
		fprintf(stdout, "\nError occured getting sensor type enum\n");
	}

	// Initialize width and height of Composite image
	nWidth = config.Composite_Image_Size_X;
	nHeight = config.Composite_Image_Size_Y;
	
	pCompositeImage1 = new uchar[nWidth*nHeight];
	pCompositeImage2 = new uchar[nWidth*nHeight];

	// Capture first image and get composite image
	fprintf(stdout,"\n\nFirst Capture(Capture and Get Composite image)... Place finger on sensor");

	if(false == Capture(pDev, pCompositeImage1, nWidth, nHeight, NULL, nTemplateSize1, nSpoof, 1, 0))
	{
		goto Abort;
	}
	
	fprintf(stdout,"\nSpoof Score %d",nSpoof);

	// Get latest extracted template from the device
	fprintf(stdout,"\n\nGetting template...");
	fprintf(stdout, "\nV100_Get_Template:  ");
	rc = V100_Get_Template(pDev, pTemplate1, &nTemplateSize1);
	PrintReturnCode(rc);

	// Capture second image and get template 
	fprintf(stdout,"\n\nSecond Capture(Capture and Get Template)... Place finger on sensor");

	if(false == Capture(pDev, NULL, nWidth, nHeight, pTemplate2, nTemplateSize2, nSpoof, 0, 1))
	{
		goto Abort;
	}

	// Match templates
	fprintf(stdout,"\n\nMatching...");
	fprintf(stdout, "\nV100_Match:  ");
	rc = V100_Match(pDev,
					pTemplate1,
					nTemplateSize1,
					pTemplate2,
					nTemplateSize2,
					Score);
	
	PrintReturnCode(rc);
	fprintf(stdout,"\nMatch Score %d",Score);

	//Change presence detection threshold
	fprintf(stdout,"\n\nChanging presence detection level to low sensitivity...");
	OptType = OPTION_PD_LEVEL;
	// Low sensitivity means hard to detect
	PdType = PD_SENSITIVITY_LOW;
	fprintf(stdout, "\nV100_Set_Option:  ");
	rc = V100_Set_Option(pDev, OptType,(uchar *)(&PdType), sizeof(_V100_PRESENCE_DETECTION_TYPE));
	PrintReturnCode(rc);

	// Verify with second template
	fprintf(stdout,"\n\nVerifying with second capture... Place finger on sensor");	
	fprintf(stdout, "\nV100_Verify:  ");
	rc = V100_Verify(pDev,
					 pTemplate2,
					 nTemplateSize2,
					 nSpoof,
					 Score);
					 
	PrintReturnCode(rc);
	fprintf(stdout,"\nMatch Score %d",Score);
	fprintf(stdout,"\nSpoof Score %d",nSpoof);

	if(g_bFingerLift)
	{
		// Wait for user to lift finger
		fprintf(stdout,"\n\nVerify complete. Please lift your finger...");
		fprintf(stdout, "\nV100_WaitForFingerClear:  ");
		rc = V100_WaitForFingerClear(pDev);
		PrintReturnCode(rc);
		if( rc != GEN_OK)
		{
			fprintf(stdout, "\nError: Call to V100_WaitFingerClear failed!\n");
			goto Abort;
		}
	}
	
	// Get current composite image
	fprintf(stdout,"\n\nGetting Composite Image...");
	fprintf(stdout, "\nV100_Get_Composite_Image:  ");
	rc = V100_Get_Composite_Image(pDev, pCompositeImage2, &nSpoof, &nImageSize);
	PrintReturnCode(rc);
	fprintf(stdout,"\nSpoof Score %d",nSpoof);

	if(g_nDeviceType == VENUS_V30X)
	{
		// LED CONTROL. 
		// Set Red light on
		fprintf(stdout,"\n\nSetting Red light on");	
		control = RED_ON;	
		fprintf(stdout, "\nV100_Set_LED:  ");
		rc = V100_Set_LED(pDev, control);
		PrintReturnCode(rc);

		// Wait 3 secs
		dwStartTime = GetTickCount();
		while(1)
		{			
			if(GetTickCount() - dwStartTime > 3000) break;
		}

		// Set Red light off
		fprintf(stdout,"\n\nSetting Red light off");	
		control = RED_OFF;	
		fprintf(stdout, "\nV100_Set_LED:  ");
		rc = V100_Set_LED(pDev, control);
		PrintReturnCode(rc);
	}


    //For Venus Firmware > 9538 - some additional features like Spoof On and Off etc
	if (g_nDeviceType == VENUS_V30X && config.Firmware_Rev > 9538)
	{
      _V100_INTERFACE_COMMAND_TYPE origCMD, tempCMD;
	  char m_buffer[255];
      //Turn off spoof
	  fprintf(stdout,"\n\nCapture with spoof turned OFF...");
      rc = V100_Get_Cmd(pDev, &origCMD);
      if(rc != GEN_OK)
      {
            memset(&m_buffer, 0, 255);
            printf( "FAILED - Test_GET_SET_CMD() V100_Get_Cmd() returned [%d] \n\n", rc);
            return false;
      }
      memcpy(&tempCMD,&origCMD,sizeof(origCMD));
      tempCMD.Select_Spoof_Model = 0;
      rc = V100_Set_Cmd(pDev, &tempCMD);
      if(rc != GEN_OK)
      {
            memset(&m_buffer, 0, 255);
            printf("FAILED - Test_GET_SET_CMD() V100_Set_Cmd() returned [%d]\n\n", rc);
            rc = V100_Set_Cmd(pDev, &origCMD);
            return false;
      }

	  nW = config.Composite_Image_Size_X;
      nH = config.Composite_Image_Size_Y;
      pCompositeImage = new uchar[nW*nH];
      memset(pCompositeImage,0,nW*nH);
      nTemplateSize = 2048;
      pTemplate = new uchar[nTemplateSize];

      rc = V100_Capture(pDev, pCompositeImage, nW, nH, pTemplate, nTemplateSize, nSpoof, 1,1);
      if(rc != GEN_OK)
      {
            memset(&m_buffer, 0, 255);
            printf("FAILED - Test_GET_SET_CMD() V100_Capture() returned [%d]\n\n", rc);
            delete []pCompositeImage;
            delete []pTemplate;
            rc = V100_Set_Cmd(pDev, &origCMD);
            return false;
      }
      if(nSpoof != -1)
      {
            memset(&m_buffer, 0, 255);
            printf("FAILED - Test_GET_SET_CMD() Spoof returned [%d] instead of -1\n\n", nSpoof);
            delete []pCompositeImage;
            delete []pTemplate;
            rc = V100_Set_Cmd(pDev, &origCMD);
            return false;
      }
	  rc = V100_Set_Cmd(pDev, &origCMD);
	  PrintReturnCode(rc);
      fprintf(stdout,"\nSpoof turned ON");
	  

	  fprintf(stdout,"\n\nTurning Off Extractor...");
      //turn off extractor
      tempCMD.Select_Extractor = 0;
      rc = V100_Set_Cmd(pDev, &tempCMD);
      if(rc != GEN_OK)
      {
            memset(&m_buffer, 0, 255);
            printf("FAILED - Test_GET_SET_CMD() V100_Set_Cmd(Extractor off) returned [%d]\n\n", rc);
            rc = V100_Set_Cmd(pDev, &origCMD);
            return false;
      }
    
	  rc = V100_Set_Cmd(pDev, &origCMD);
	  PrintReturnCode(rc);	
      fprintf(stdout,"\nTurning the Extractor ON\n\n");

	  fprintf(stdout,"\nSetting Crop Level to 0 for a Full Size Image...");
      //test crop level
      rc = V100_Get_Config(pDev,&config);
      tempCMD.Select_Crop_Level = 0;
      rc = V100_Set_Cmd(pDev, &tempCMD);
      if(rc != GEN_OK)
      {
            memset(&m_buffer, 0, 255);
            printf("FAILED - Test_GET_SET_CMD() V100_Set_CMD() returned [%d] \n\n", rc);
            delete []pCompositeImage;
            delete []pTemplate;
            rc = V100_Set_Cmd(pDev, &origCMD);
            return false;
      }
      rc = V100_Get_Config(pDev,&config);
      if(config.Composite_Image_Size_X == 272)
      {
            memset(&m_buffer, 0, 255);
            printf("FAILED - Test_GET_SET_CMD() select crop level() returned [%d]\n\n", config.Composite_Image_Size_X);
            delete []pCompositeImage;
            delete []pTemplate;
            rc = V100_Set_Cmd(pDev, &origCMD);
            return false;
      }
	  rc = V100_Set_Cmd(pDev, &origCMD);
	  PrintReturnCode(rc);
	  fprintf(stdout,"\nReset the Image to Cropped Image Successful\n");

}


	  delete []pCompositeImage;
      delete []pTemplate;
      return true;

Abort:
	
	delete[] pCompositeImage1;
	delete[] pCompositeImage2;

	return retCode;
}


bool Capture(V100_DEVICE_TRANSPORT_INFO *pDev,
							  uchar* pCompositeImage,
							  uint&	 nWidth,
							  uint&  nHeight,
							  uchar* pTemplate,
							  uint&	 nTemplateSize,
							  int&	 nSpoof,
							  int	 getComposite,
							  int	 getTemplate)
{
	V100_ERROR_CODE rc = GEN_OK;
	int nTimeouts = 0;
	// If timeout attempt 10 times to capture the image before giving up
	while(nTimeouts < 10)
	{		
		fprintf(stdout, "\nV100_Capture:  ");
		rc = V100_Capture(pDev, pCompositeImage, nWidth, nHeight, pTemplate, nTemplateSize, nSpoof, getComposite, getTemplate);
		PrintReturnCode(rc);
		if(rc == GEN_ERROR_TIMEOUT)
			nTimeouts++;	
		else if(rc == GEN_ERROR_TIMEOUT_LATENT)
		{
			nTimeouts++;
			fprintf(stdout, " -- Please lift your finger and place again\n");
			fprintf(stdout, "\nV100_Capture:  ");
		}
		else
			break;
	}	
	if( rc != GEN_OK)
	{
		fprintf(stdout, "\nError occured while capturing image\n");
		return false;
	}

	if(g_bFingerLift)
	{
		// Wait for user to lift finger
		fprintf(stdout,"\n\nCapture complete. Please lift your finger...");
		fprintf(stdout, "\nV100_WaitForFingerClear:  ");
		rc = V100_WaitForFingerClear(pDev);
		PrintReturnCode(rc);
		if( rc != GEN_OK)
		{
			fprintf(stdout, "\nError: Call to V100_WaitFingerClear failed!\n");
			return false;
		}
	}

	return true;
}


bool RunIDTest(V100_DEVICE_TRANSPORT_INFO* pDev)
{
	V100_ERROR_CODE	rc;
	_V100_OP_ERROR	Oprc;
	fprintf(stdout, "\n\n--- Running Identification Test ---\n");

	//----------------------------------------------------------------------//
	// 1. Get system information from the sensor							//
	//----------------------------------------------------------------------//
	// Get System Metrics
	_MX00_DB_METRICS* sysDBMetrics = NULL;
	uint nNumDBsFound = 0;
	fprintf(stdout, "\nV100_ID_Get_System_Metrics:  ");
	rc = V100_ID_Get_System_Metrics(pDev, &sysDBMetrics, nNumDBsFound);
	PrintReturnCode(rc);
	if( rc != GEN_OK)
	{
		fprintf(stdout, "\nError: Call to V100_ID_Get_System_Metrics failed!\n");
		return false;
	}
	fprintf(stdout, "\nNumber of DBs found: %d\n", nNumDBsFound);
	fprintf(stdout, "\nV100_ID_Release_System_Metrics:  ");
	rc = V100_ID_Release_System_Metrics(pDev, sysDBMetrics);
	PrintReturnCode(rc);
	if( rc != GEN_OK)
	{
		fprintf(stdout, "\nError: Call to V100_ID_Release_System_Metrics failed!\n");
		return false;
	}

	// Get Identification Parameters
	_MX00_ID_PARAMETERS idParams;
	fprintf(stdout, "\n\nV100_ID_Get_Parameters:  ");
	rc = V100_ID_Get_Parameters(pDev, idParams);
	PrintReturnCode(rc);
	if( rc != GEN_OK)
	{
		fprintf(stdout, "\nError: Call to V100_ID_Get_Parameters failed!\n");
		return false;
	}
	fprintf(stdout, "\nIdentification Parameters");
	fprintf(stdout, "\nSecurity Level: %d", idParams.nSecurityLevel);
	fprintf(stdout, "\nE/V Flags:      %d\n", idParams.nFlags);

	//----------------------------------------------------------------------//
	// 2. Create the initial Database on the sensor							//
	//----------------------------------------------------------------------//
	_MX00_DB_INIT_STRUCT dbInitStructIn;
	memset(&dbInitStructIn, 0, sizeof(_MX00_DB_INIT_STRUCT));
	// DB_INIT_FLAG_ID_DEFAULT = Identification Enabled
	// DB_INIT_FLAG_ID_ENHANCED = Enhanced 1:N search capabilities
	dbInitStructIn.nFlags =  DB_INIT_FLAG_ID_DEFAULT | DB_INIT_FLAG_ID_ENHANCED;
	dbInitStructIn.nGroupID = 0;
	dbInitStructIn.nInstancesPerFinger = 3;
	dbInitStructIn.nFingersPerUser = 2;
	dbInitStructIn.nUsers = 100;

	// Delete DB if exists
	fprintf(stdout, "\nV100_ID_Delete_DB:  ");
	rc = V100_ID_Delete_DB(pDev, dbInitStructIn.nGroupID);
	PrintReturnCode(rc);
	if( rc != GEN_OK)
	{
		fprintf(stdout, "\nError: Call to V100_ID_Delete_DB failed!\n");
		return false;
	}
	fprintf(stdout, "\nWaitForOperationCompletion:  ");
	Oprc = WaitForOperationCompletion(pDev);
	PrintOPReturnCode(Oprc);
	if(Oprc !=0)
	{
		fprintf(stdout, "\nError: V100_ID_Delete_DB was not successful");
		return false;
	}

	// Create DB
	fprintf(stdout, "\n\nV100_ID_Create_DB:  ");
	rc = V100_ID_Create_DB(pDev, dbInitStructIn);
	PrintReturnCode(rc);
	if( rc != GEN_OK)
	{
		fprintf(stdout, "\nError: Call to V100_ID_Create_DB failed!\n");
		return false;
	}
	fprintf(stdout, "\nWaitForOperationCompletion:  ");
	Oprc = WaitForOperationCompletion(pDev);
	PrintOPReturnCode(Oprc);
	if(Oprc !=0)
	{
		fprintf(stdout, "\nError: V100_ID_Create_DB was not successful");
		return false;
	}

	// Set working DB
	fprintf(stdout, "\n\nV100_ID_Set_Working_DB:  ");
	rc = V100_ID_Set_Working_DB(pDev, dbInitStructIn.nGroupID);
	PrintReturnCode(rc);
	if( rc != GEN_OK)
	{
		fprintf(stdout, "\nError: Call to V100_ID_Set_Working_DB failed!\n");
		return false;
	}
	fprintf(stdout, "\nWaitForOperationCompletion:  ");
	Oprc = WaitForOperationCompletion(pDev);
	PrintOPReturnCode(Oprc);
	if(Oprc !=0)
	{
		fprintf(stdout, "\nError: V100_ID_Set_Working_DB was not successful");
		return false;
	}

	//----------------------------------------------------------------------//
	// 3. Enroll some user record into the database							//
	//----------------------------------------------------------------------//
	// Note: This is an example of enrolling UserRecords. A UserRecord
	//       represents one user/finger combination. A user may have up to 
	//       nFingersPerUser UserRecords as defined by the database group it
	//       resides in. Here we create two separate UserRecords giving them
	//       different fingerIds. See the Lumidigm Identification Demo for a more
	//       sophisticated enrollment example.
	
	// Setup first User Record for enrollment
	_MX00_ID_USER_RECORD UserRecord;
	UserRecord.nFinger = 6;	// Hard code to Right Index
	UserRecord.nGroupID = dbInitStructIn.nGroupID;
	UserRecord.nInstances = dbInitStructIn.nInstancesPerFinger;
	sprintf(UserRecord.szUserID, "%d", 1001);

	// Setup second User Record for enrollment
	_MX00_ID_USER_RECORD UserRecord1;
	UserRecord1.nFinger = 7;	// Hard code to Right Middle
	UserRecord1.nGroupID = dbInitStructIn.nGroupID;
	UserRecord1.nInstances = dbInitStructIn.nInstancesPerFinger;
	sprintf(UserRecord1.szUserID, "%d", 1001);

	//setup the third user record for cloning
	_MX00_ID_USER_RECORD UserRecord2;
	UserRecord2.nFinger = 7;
	UserRecord2.nGroupID = dbInitStructIn.nGroupID;
	UserRecord2.nInstances = dbInitStructIn.nInstancesPerFinger;
	
	
	// Enroll first User Record
	fprintf(stdout,"\n\nEnroll User Record 1001 with Right Index finger");
	uint nTimeouts = 0;
	while(nTimeouts < 10)
	{		
		fprintf(stdout, "\nV100_ID_Enroll_User_Record:  ");
		rc = V100_ID_Enroll_User_Record(pDev, UserRecord);
		PrintReturnCode(rc);
		if( rc != GEN_OK)
		{
			fprintf(stdout, "\nError: Call to V100_ID_Enroll_User_Record failed!\n");
			return false;
		}
		fprintf(stdout, "\nWaitForOperationCompletion:  ");
		Oprc = WaitForOperationCompletion(pDev);
		PrintOPReturnCode(Oprc);

		if(Oprc == ERROR_CAPTURE_TIMEOUT)
			nTimeouts++;	
		else if(Oprc == ERROR_CAPTURE_LATENT)
		{
			nTimeouts++;
			fprintf(stdout, "\n-- Please lift your finger and place again --\n");
		}
		else if(0 != Oprc)
		{
			fprintf(stdout, "\nError: V100_ID_Enroll_User_Record was not successful");
			return false;
		}
		else 
			break;
	}	
	
	// Enroll Second User Record
	fprintf(stdout,"\n\nEnroll User Record 1001 with Right Middle finger");
	nTimeouts = 0;
	while(nTimeouts < 10)
	{		
		fprintf(stdout, "\nV100_ID_Enroll_User_Record:  ");
		rc = V100_ID_Enroll_User_Record(pDev, UserRecord1);
		PrintReturnCode(rc);
		if( rc != GEN_OK)
		{
			fprintf(stdout, "\nError: Call to V100_ID_Enroll_User_Record failed!\n");
			return false;
		}
		fprintf(stdout, "\nWaitForOperationCompletion:  ");
		Oprc = WaitForOperationCompletion(pDev);
		PrintOPReturnCode(Oprc);

		if(Oprc == ERROR_CAPTURE_TIMEOUT)
			nTimeouts++;	
		else if(Oprc == ERROR_CAPTURE_LATENT)
		{
			nTimeouts++;
			fprintf(stdout, "\n-- Please lift your finger and place again --\n");
		}
		else if(0 != Oprc)
		{
			fprintf(stdout, "\nError: V100_ID_Enroll_User_Record was not successful");
			return false;
		}
		else 
			break;
	}
	// Replicate user record as different user
	_MX00_TEMPLATE_INSTANCE *pInstanceRecords = new _MX00_TEMPLATE_INSTANCE[dbInitStructIn.nInstancesPerFinger];
	fprintf(stdout,"\n\nV100_ID_Get_User_Record:  ");
	rc = V100_ID_Get_User_Record(pDev,1,UserRecord1, pInstanceRecords);
	if(rc != GEN_OK)
	{
		fprintf(stdout, "\nError: Call to V100_ID_Get_User_Record failed: %d!\n",rc);
        delete [] pInstanceRecords;
		return false;
	}
	fprintf(stdout,"Success! \n  ");
	memcpy(&UserRecord2,&UserRecord1,sizeof(UserRecord1));
	sprintf(UserRecord2.szUserID, "clone");
	fprintf(stdout,"\n\nV100_ID_Set_User_Record:  ");
	rc = V100_ID_Set_User_Record(pDev,UserRecord2, pInstanceRecords);
	if(rc != GEN_OK)
	{
		fprintf(stdout, "\nError: Call to V100_ID_Set_User_Record failed: %d!\n",rc);
        delete [] pInstanceRecords;
		return false;
	}
    delete [] pInstanceRecords;
	fprintf(stdout,"Success! \n  ");
	// Get DB Metrics
	fprintf(stdout, "\n\nV100_ID_Get_DB_Metrics:  ");
	_MX00_DB_METRICS dbMetrics;
	memset(&dbMetrics, 0, sizeof(_MX00_DB_METRICS));
	rc = V100_ID_Get_DB_Metrics(pDev, &dbMetrics, true);
	PrintReturnCode(rc);
	if( rc != GEN_OK)
	{
		fprintf(stdout, "\nError: Call to V100_ID_Get_DB_Metrics failed!\n");
		return false;
	}
	fprintf(stdout, "\nDatabase Metrics after Enrollment");
	PrintDBMetrics(&dbMetrics);


	//----------------------------------------------------------------------//
	// 4. Identification process											//
	//----------------------------------------------------------------------//
	// Identify User
	fprintf(stdout,"\n\nIdentify User. Place a finger on the sensor...");
	nTimeouts = 0;
	while(nTimeouts < 10)
	{		
		fprintf(stdout, "\nV100_ID_Identify:  ");
		rc = V100_ID_Identify(pDev);
		PrintReturnCode(rc);
		if( rc != GEN_OK)
		{
			fprintf(stdout, "\nError: Call to V100_ID_Identify failed!\n");
			return false;
		}
		fprintf(stdout, "\nWaitForOperationCompletion:  ");
		Oprc = WaitForOperationCompletion(pDev);
		PrintOPReturnCode(Oprc);
		
		if(Oprc == ERROR_CAPTURE_TIMEOUT)
			nTimeouts++;	
		else if(Oprc == ERROR_CAPTURE_LATENT)
		{
			nTimeouts++;
			fprintf(stdout, "\n-- Please lift your finger and place again --\n");
		}
		else if(0 != Oprc)
		{
			fprintf(stdout, "\nError: V100_ID_Identify was not successful");
			return false;
		}
		else 
			break;
	}

	// Get result of identification
	_MX00_ID_RESULT Result;
	fprintf(stdout, "\nV100_ID_Get_Result:  ");
	rc = V100_ID_Get_Result(pDev, Result);
	PrintReturnCode(rc);
	if( rc != GEN_OK)
	{
		fprintf(stdout, "\nError: Call to V100_ID_Get_Result failed!\n");
		return false;
	}
	fprintf(stdout,
		"\n\nIdentification Results:\n  UserID: %s\n  Finger: %d\n  Match Score: %d\n  Spoof Score: %d",
		    Result.szUserID, Result.nFinger, Result.nIDScore, Result.nSpoofScore);
	fprintf(stdout,"\n  eLastStatus: ");
	PrintOPReturnCode(Result.eLastStatus);

	if(g_bFingerLift)
	{
		// Wait for user to lift finger
		fprintf(stdout,"\n\nIdentification complete. Please lift your finger...");
		fprintf(stdout, "\nV100_WaitForFingerClear:  ");
		rc = V100_WaitForFingerClear(pDev);
		PrintReturnCode(rc);
		if( rc != GEN_OK)
		{
			fprintf(stdout, "\nError: Call to V100_WaitFingerClear failed!\n");
			return false;
		}
	}

	// Get current composite image in FIR format
	fprintf(stdout,"\n\nGetting FIR image...");
	_V100_FIR_RECORD_TYPE FIRType = LUMI_FIR_ISO;
	_V100_FINGER_PALM_POSITION FingerType = LUMI_FINGER_POSITION_RIGHT_INDEX_FINGER;
	uint nFIRImageSz =  0;
	uchar* pFIRImage = NULL;
	// Get Config Structure
	fprintf(stdout, "\nV100_Get_Config:  ");
	_V100_INTERFACE_CONFIGURATION_TYPE	config;
	rc = V100_Get_Config(pDev, &config);
	PrintReturnCode(rc);
	if(rc != GEN_OK)
	{
		fprintf(stdout, "\nError occured getting config structure\n");
		return false;
	}
	fprintf(stdout, "\nV100_Get_FIR_Image:  ");
	nFIRImageSz =  ISO_19794_4_HDR_SIZE + config.Composite_Image_Size_X * config.Composite_Image_Size_Y;
	pFIRImage = new uchar[nFIRImageSz];
	rc = V100_Get_FIR_Image(pDev, FIRType, FingerType, pFIRImage, &nFIRImageSz);
	PrintReturnCode(rc);
	delete[] pFIRImage;
	fprintf(stdout,"\n\n");


	//----------------------------------------------------------------------//
	// 5. Verification process												//
	//----------------------------------------------------------------------//
	// Verify Middle Finger
	fprintf(stdout,"\nVerify User 1001. Place Right Middle finger on sensor...");
	nTimeouts = 0;
	while(nTimeouts < 10)
	{		
		fprintf(stdout, "\nV100_ID_Verify_User_Record:  ");
		rc = V100_ID_Verify_User_Record(pDev, UserRecord1, 7);
		PrintReturnCode(rc);
		if( rc != GEN_OK)
		{
			fprintf(stdout, "\nError: Call to V100_ID_Verify_User_Record failed!\n");
			return false;
		}
		fprintf(stdout, "\nWaitForOperationCompletion:  ");
		Oprc = WaitForOperationCompletion(pDev);
		PrintOPReturnCode(Oprc);

		if(Oprc == ERROR_CAPTURE_TIMEOUT)
			nTimeouts++;	
		else if(Oprc == ERROR_CAPTURE_LATENT)
		{
			nTimeouts++;
			fprintf(stdout, "\n-- Please lift your finger and place again --\n");
		}
		else if(0 != Oprc)
		{
			fprintf(stdout, "\nError: V100_ID_Verify_User_Record was not successful");
			return false;
		}
		else 
			break;
	}

	// Get result of verification
	_MX00_ID_RESULT verResult;
	fprintf(stdout, "\nV100_ID_Get_Result:  ");
	rc = V100_ID_Get_Result(pDev, verResult);
	PrintReturnCode(rc);
	if( rc != GEN_OK)
	{
		fprintf(stdout, "\nError: Call to V100_ID_Get_Result failed!\n");
		return false;
	}
	fprintf(stdout,
		    "\n\nVerification Results:\n  UserID: %s\n  Finger: %d\n  Match Score: %d\n  Spoof Score: %d",
		    verResult.szUserID, verResult.nFinger, verResult.nIDScore, verResult.nSpoofScore);
	fprintf(stdout,"\n  eLastStatus: ");
	PrintOPReturnCode(verResult.eLastStatus);

	if(g_bFingerLift)
	{
		// Wait for user to lift finger
		fprintf(stdout,"\n\nVerify User complete. Please lift your finger...");
		fprintf(stdout, "\nV100_WaitForFingerClear:  ");
		rc = V100_WaitForFingerClear(pDev);
		PrintReturnCode(rc);
		if( rc != GEN_OK)
		{
			fprintf(stdout, "\nError: Call to V100_WaitFingerClear failed!\n");
			return false;
		}
	}
	fprintf(stdout,"\n\n");

	//----------------------------------------------------------------------//
	// 6. Deleting a database group											//
	//----------------------------------------------------------------------//

	_MX00_TEMPLATE_INSTANCE* pNotUsed = new _MX00_TEMPLATE_INSTANCE[dbMetrics.nInstancesPerFinger];

	//example delete a single user record
	_MX00_ID_USER_RECORD urToDel;
	memset(&urToDel, 0, sizeof(_MX00_ID_USER_RECORD));
	urToDel.nFinger = 7;	// Hard code to Right Middle
	urToDel.nGroupID = dbMetrics.nGroupID;
	urToDel.nInstances = dbMetrics.nInstancesPerFinger;
	sprintf(urToDel.szUserID, "%d", 1001);

	fprintf(stdout, "\nV100_ID_Get_User_Record:  ");
	rc = V100_ID_Get_User_Record(pDev, -1, urToDel, pNotUsed);
	delete [] pNotUsed;
	PrintReturnCode(rc);
	if( rc != GEN_OK)
	{
		if(rc == GEN_ERROR_USER_NOT_FOUND)
		{
			fprintf(stdout, "\nUser '%s' was not found in the database\n", urToDel.szUserID);
			goto DelDB;
		}

		fprintf(stdout, "\nError: Call to V100_ID_Get_User_Record failed!\n");
		return false;
	}
	
	fprintf(stdout, "\nV100_ID_Delete_User_Record:  ");
	rc = V100_ID_Delete_User_Record(pDev, urToDel, true);
	PrintReturnCode(rc);
	if( rc != GEN_OK)
	{
		if(rc == GEN_ERROR_USER_NOT_FOUND)
		{
			fprintf(stdout, "\nUser '%s' was not found in the database\n", urToDel.szUserID);
			goto DelDB;
		}

		fprintf(stdout, "\nError: Call to V100_ID_Delete_User_Record failed!\n");
		return false;
	}

DelDB:
	// Delete DB group
	fprintf(stdout, "\nV100_ID_Delete_DB:  ");
	rc = V100_ID_Delete_DB(pDev, dbInitStructIn.nGroupID);
	PrintReturnCode(rc);
	if( rc != GEN_OK)
	{
		fprintf(stdout, "\nError: Call to V100_ID_Delete_DB failed!\n");
		return false;
	}
	fprintf(stdout, "\nWaitForOperationCompletion:  ");
	Oprc = WaitForOperationCompletion(pDev);
	PrintOPReturnCode(Oprc);
	if(Oprc !=0)
	{
		fprintf(stdout, "\nError: V100_ID_Delete_DB was not successful");
		return false;
	}


	return true;
}

bool RunVerifyTest(V100_DEVICE_TRANSPORT_INFO* pDev)
{
	// Initialize variables	
	_V100_INTERFACE_CONFIGURATION_TYPE	config;
	V100_ERROR_CODE						rc;
	uchar	pTemplate[2048];
	uint	nWidth;
	uint	nHeight;
	uint	nTemplateSize;
	int		nSpoof;
	bool    retCode = true;

	fprintf(stdout, "\n\n--- Running Capture and Verify Test ---\n");

	// Get Config Structure
	fprintf(stdout, "\nV100_Get_Config:  ");
	rc = V100_Get_Config(pDev, &config);
	PrintReturnCode(rc);
	if(rc != GEN_OK)
	{
		fprintf(stdout, "\nError occured getting config structure\n");
		retCode = false;
		goto Abort;
	}

	// Initialize width and height of Composite image
	nWidth = config.Composite_Image_Size_X;
	nHeight = config.Composite_Image_Size_Y;
	
	// Capture first image and get template
	fprintf(stdout,"\n\nCapture (simulated enrollment)...  Place finger on sensor");	
	if(false == Capture(pDev, NULL, nWidth, nHeight, pTemplate, nTemplateSize, nSpoof, 0, 1))
	{
		goto Abort;
	}

	// Capture second image and verify against the template just taken
	fprintf(stdout,"\n\nVerify 378...  Place finger on sensor...");
	rc = V100_Verify_378(pDev, pTemplate, nTemplateSize);
	PrintReturnCode(rc);
	if( rc == GEN_NOT_SUPPORTED && (g_nDeviceType == VENUS_V31X || g_nDeviceType == VENUS_V371))
	{
		fprintf(stdout, "\nVerify_378 is not supported by %s\n", DeviceTypeStr[g_nDeviceType]);
	}
	else if( rc != GEN_OK)
	{
		fprintf(stdout, "\nError occured while capturing image\n");
		return false;
	}
	else
	{
		// Wait for the verification to complete
		fprintf(stdout, "\nWaitForOperationCompletion:  ");
		_V100_OP_ERROR Oprc;
		Oprc= WaitForOperationCompletion(pDev);
		PrintOPReturnCode(Oprc);
		if(Oprc !=0)
		{
			fprintf(stdout, "\nError: V100_Verify_378 was not successful");
			return false;
		}

		// Get result of identification
		_MX00_ID_RESULT Result;
		fprintf(stdout, "\nV100_ID_Get_Result:  ");
		rc = V100_ID_Get_Result(pDev, Result);
		PrintReturnCode(rc);
		if( rc != GEN_OK)
		{
			fprintf(stdout, "\nError: Call to V100_ID_Get_Result failed!\n");
			return false;
		}
		fprintf(stdout,
			"\n\nIdentification Results:\n  Match Score: %d\n  Spoof Score: %d",
				Result.nIDScore, Result.nSpoofScore);
		fprintf(stdout,"\n  eLastStatus: ");
		PrintOPReturnCode(Result.eLastStatus);
	}

Abort:	
	return retCode;
}

// Wait for completion
_V100_OP_ERROR WaitForOperationCompletion(V100_DEVICE_TRANSPORT_INFO* pDev)
{
	int nLastInst = -1;
	_V100_OP_STATUS opStatus;
	
	while(1)
	{
		V100_ERROR_CODE rc = V100_Get_OP_Status(pDev, opStatus);
		if(GEN_OK != rc)
		{
			fprintf(stdout, "\nError: Call to V100_Get_OP_Status failed with code: ");
			PrintReturnCode(rc);
			fprintf(stdout, "\nError: WaitForOperationCompletion failed with code: ");
		    return ERROR_CAPTURE_INTERNAL;
		}

		switch(opStatus.nMode)
		{
		case OP_IN_PROGRESS:
			if(opStatus.eMacroCommand == CMD_ID_ENROLL_USER_RECORD)
			{
				if(opStatus.eAcqStatus != ACQ_NOOP) // Ignore the NO OP where the system is deciding if it can enroll the user
				{
					if(nLastInst != (int)opStatus.nParameter)
						fprintf(stdout, "\nWaiting for insertion %d ", opStatus.nParameter);
				}
				nLastInst = (int)opStatus.nParameter;
			}
			if(opStatus.eAcqStatus == ACQ_FINGER_PRESENT)
			{
				fprintf(stdout, "\nPlease lift your finger");
			}
			break;
		case OP_ERROR:
			if((_V100_OP_ERROR)opStatus.nParameter != ERROR_CAPTURE_LATENT)
			{
				fprintf(stdout, "\nError occured. Error Code: %d\n", opStatus.nParameter);
			}
			return (_V100_OP_ERROR)opStatus.nParameter;
			break;
		case OP_COMPLETE:
			fprintf(stdout, "\nOperation complete\n");
			return STATUS_OK;
			break;
		case NO_OP:
			fprintf(stdout, "\nNo Operation\n");
			return STATUS_OK;
			break;
		}

		fprintf(stdout, ".");
		SleepVIK(100);
		fflush(stdout);
	}
}

// Print error code
void PrintReturnCode(V100_ERROR_CODE rc)
{
	switch(rc)
	{
		case	GEN_OK: 
			fprintf(stdout,"GEN_OK");
			break;
		case	GEN_ENCRYPTION_FAIL:
			fprintf(stdout,"GEN_ENCRYPTION_FAIL");
			break;
		case	GEN_DECRYPTION_FAIL:
			fprintf(stdout, "GEN_DECRYPTION_FAIL");
			break;
		case	GET_PD_INIT_FAIL:
			fprintf(stdout,"GET_PD_INIT_FAIL");
			break;
		case	PD_HISTOGRAM_FAIL:
			fprintf(stdout,"PD_HISTOGRAM_FAIL");
			break;
		case	INVALID_ACQ_MODE:
			fprintf(stdout, "INVALID_ACQ_MODE");
			break;
		case	BURNIN_THREAD_FAIL:
			fprintf(stdout,"BURNIN_THREAD_FAIL");
			break;
		case	UPDATER_THREAD_FAIL:
			fprintf(stdout,"UPDATER_THREAD_FAIL");
			break;
		case	GEN_ERROR_START:
			fprintf(stdout, "GEN_ERROR_START");
			break;
		case	GEN_ERROR_PROCESSING:
			fprintf(stdout,"GEN_ERROR_PROCESSING,");
			break;
		case	GEN_ERROR_VERIFY:
			fprintf(stdout,"GEN_ERROR_VERIFY,");
			break;
		case	GEN_ERROR_MATCH:
			fprintf(stdout, "GEN_ERROR_MATCH");
			break;
		case	GEN_ERROR_INTERNAL: 
			fprintf(stdout,"GEN_ERROR_INTERNAL");
			break;
		case	GEN_ERROR_INVALID_CMD:
			fprintf(stdout,"GEN_ERROR_INVALID_CMD");
			break;
		case	GEN_ERROR_PARAMETER:
			fprintf(stdout, "GEN_ERROR_PARAMETER");
			break;
		case	GEN_NOT_SUPPORTED:
			fprintf(stdout,"GEN_NOT_SUPPORTED");
			break;
		case	GEN_INVALID_ARGUEMENT:
			fprintf(stdout,"GEN_INVALID_ARGUEMENT");
			break;
		case	GEN_ERROR_TIMEOUT:
			fprintf(stdout, "GEN_ERROR_TIMEOUT");
			break;
		case	GEN_ERROR_LICENSE:
			fprintf(stdout,"GEN_ERROR_LICENSE");
			break;
		case	GEN_ERROR_COMM_TIMEOUT:
			fprintf(stdout,"GEN_ERROR_COMM_TIMEOUT");
			break;
		case	GEN_FS_ERR_CD:
			fprintf(stdout,"GEN_FS_ERR_CD");
			break;
		case	GEN_FS_ERR_DELETE:
			fprintf(stdout,"GEN_FS_ERR_DELETE");
			break;
		case	GEN_FS_ERR_FIND:
			fprintf(stdout,"GEN_FS_ERR_FIND");
			break;
		case	GEN_FS_ERR_WRITE:
			fprintf(stdout,"GEN_FS_ERR_WRITE");
			break;
		case	GEN_FS_ERR_READ:
			fprintf(stdout,"GEN_FS_ERR_READ");
			break;
		case	GEN_FS_ERR_FORMAT:
			fprintf(stdout,"GEN_FS_ERR_FORMAT");
			break;
		case	GEN_ERROR_MEMORY:
			fprintf(stdout,"GEN_ERROR_MEMORY");
			break;
		case	GEN_ERROR_RECORD_NOT_FOUND:
			fprintf(stdout,"GEN_ERROR_RECORD_NOT_FOUND");
			break;
		case	GEN_VER_INVALID_RECORD_FORMAT:
			fprintf(stdout,"GEN_VER_INVALID_RECORD_FORMAT");
			break;
		case	GEN_ERROR_DB_FULL:
			fprintf(stdout,"GEN_ERROR_DB_FULL");
			break;
		case	GEN_ERROR_INVALID_SIZE:
			fprintf(stdout,"GEN_ERROR_INVALID_SIZE");
			break;
		case	GEN_ERROR_TAG_NOT_FOUND:
			fprintf(stdout,"GEN_ERROR_TAG_NOT_FOUND");
			break;
		case	GEN_ERROR_APP_BUSY:
			fprintf(stdout,"GEN_ERROR_APP_BUSY");
			break;
		case	GEN_ERROR_DEVICE_UNCONFIGURED:
			fprintf(stdout,"GEN_ERROR_DEVICE_UNCONFIGURED");
			break;
		case	GEN_ERROR_TIMEOUT_LATENT:
			fprintf(stdout,"GEN_ERROR_TIMEOUT_LATENT");
			break;
		case	GEN_ERROR_DB_NOT_LOADED:
			fprintf(stdout,"GEN_ERROR_DB_NOT_LOADED");
			break;
		case	GEN_ERROR_DB_DOESNOT_EXIST:
			fprintf(stdout,"GEN_ERROR_DB_DOESNOT_EXIST");
			break;
		case	GEN_ERROR_ENROLLMENTS_DO_NOT_MATCH:
			fprintf(stdout,"GEN_ERROR_ENROLLMENTS_DO_NOT_MATCH");
			break;
		case	GEN_ERROR_USER_NOT_FOUND:
			fprintf(stdout,"GEN_ERROR_USER_NOT_FOUND");
			break;
		case	GEN_ERROR_DB_USER_FINGERS_FULL:
			fprintf(stdout,"GEN_ERROR_DB_USER_FINGERS_FULL");
			break;
		case    GEN_ERROR_DB_USERS_FULL:
			fprintf(stdout, "GEN_ERROR_DB_USERS_FULL");
			break;
		case    GEN_ERROR_USER_EXISTS:
			fprintf(stdout, "GEN_ERROR_USER_EXISTS");
			break;
		case    GEN_ERROR_DEVICE_NOT_FOUND:
			fprintf(stdout, "GEN_ERROR_DEVICE_NOT_FOUND");
			break;
		case    GEN_ERROR_DEVICE_NOT_READY:
			fprintf(stdout, "GEN_ERROR_DEVICE_NOT_READY");
			break;
		case    GEN_ERROR_PIPE_READ:
			fprintf(stdout, "GEN_ERROR_PIPE_READ");
			break;
		case    GEN_ERROR_PIPE_WRITE:
			fprintf(stdout, "GEN_ERROR_PIPE_WRITE");
			break;
		case    GEN_ERROR_SENGINE_SHUTTING_DOWN:
			fprintf(stdout, "GEN_ERROR_SENGINE_SHUTTING_DOWN");
			break;
        case    GEN_ERROR_SPOOF_DETECTED:
            fprintf(stdout,"GEN_ERROR_SPOOF_DETECTED");
            break;
        case    GEN_ERROR_DATA_UNAVAILABLE:
            fprintf(stdout,"GEN_ERROR_DATA_UNAVAILABLE");
            break;
		case	GEN_LAST:
			fprintf(stdout,"GEN_LAST");
			break;
        case    GEN_ERROR_CRYPTO_FAIL:
            fprintf(stdout,"GEN_ERROR_CRYPTO_FAIL");
            break;
	}
}

// Print error code
void PrintOPReturnCode(_V100_OP_ERROR rc)
{	
	switch(rc)
	{
		case	STATUS_OK: 
			fprintf(stdout,"STATUS_OK");
			break;
		case	ERROR_UID_EXISTS:
			fprintf(stdout,"ERROR_UID_EXISTS");
			break;
		case	ERROR_ENROLLMENT_QUALIFICATION:
			fprintf(stdout, "ERROR_ENROLLMENT_QUALIFICATION");
			break;
		case	ERROR_UID_DOES_NOT_EXIST:
			fprintf(stdout,"ERROR_UID_DOES_NOT_EXIST");
			break;
		case	ERROR_DB_FULL:
			fprintf(stdout,"ERROR_DB_FULL");
			break;
		case	ERROR_QUALIFICATION:
			fprintf(stdout, "ERROR_QUALIFICATION");
			break;
		case	ERROR_DEV_TIMEOUT:
			fprintf(stdout,"ERROR_DEV_TIMEOUT");
			break;
		case	ERROR_USER_CANCELLED:
			fprintf(stdout,"ERROR_USER_CANCELLED");
			break;
		case	ERROR_SPOOF_DETECTED:
			fprintf(stdout, "ERROR_SPOOF_DETECTED");
			break;
		case	ERROR_DB_EXISTS:
			fprintf(stdout,"ERROR_DB_EXISTS,");
			break;
		case	ERROR_DB_DOES_NOT_EXIST:
			fprintf(stdout,"ERROR_DB_DOES_NOT_EXIST,");
			break;
		case	ERROR_ID_DB_TOO_LARGE:
			fprintf(stdout, "ERROR_ID_DB_TOO_LARGE");
			break;
		case	ERROR_ID_DB_EXISTS: 
			fprintf(stdout,"ERROR_ID_DB_EXISTS");
			break;
		case	ERROR_ID_USER_EXISTS:
			fprintf(stdout,"ERROR_ID_DB_USER_EXISTS");
			break;
		case	ERROR_ID_USER_NOT_FOUND:
			fprintf(stdout, "ERROR_ID_DB_USER_NOT_FOUND");
			break;
		case	STATUS_ID_MATCH:
			fprintf(stdout,"STATUS_ID_MATCH");
			break;
		case	STATUS_ID_NO_MATCH:
			fprintf(stdout,"STATUS_ID_NO_MATCH");
			break;
		case	ERROR_ID_PARAMETER:
			fprintf(stdout, "ERROR_ID_PARAMETER");
			break;
		case	ERROR_ID_GENERAL:
			fprintf(stdout,"ERROR_ID_GENERAL");
			break;
		case	ERROR_ID_FILE:
			fprintf(stdout,"ERROR_ID_FILE");
			break;
		case	ERROR_ID_NOT_INITIALIZED:
			fprintf(stdout,"ERROR_ID_NOT_INITIALIZED");
			break;
		case	ERROR_ID_DB_FULL:
			fprintf(stdout,"ERROR_ID_DB_FULL");
			break;
		case	ERROR_ID_DB_DOESNT_EXIST:
			fprintf(stdout,"ERROR_ID_DB_DOESNT_EXIST");
			break;
		case	ERROR_ID_DB_NOT_LOADED:
			fprintf(stdout,"ERROR_ID_DB_NOT_LOADED");
			break;
		case	ERROR_ID_RECORD_NOT_FOUND:
			fprintf(stdout,"ERROR_ID_RECORD_NOT_FOUND");
			break;
		case	ERROR_ID_FS:
			fprintf(stdout,"ERROR_ID_FS");
			break;
		case	ERROR_ID_CREATE_FAIL:
			fprintf(stdout,"ERROR_ID_CREATE_FAIL");
			break;
		case	ERROR_ID_INTERNAL:
			fprintf(stdout,"ERROR_ID_INTERNAL");
			break;
		case	ERROR_ID_MEM:
			fprintf(stdout,"ERROR_ID_MEM");
			break;	
		case	STATUS_ID_USER_FOUND:
			fprintf(stdout,"STATUS_ID_USER_FOUND");
			break;
		case	STATUS_ID_USER_NOT_FOUND:
			fprintf(stdout,"STATUS_ID_USER_NOT_FOUND");
			break;
		case	ERROR_ID_USER_FINGERS_FULL:
			fprintf(stdout,"ERROR_ID_USER_FINGERS_FULL");
			break;
		case	ERROR_ID_USER_MULTI_FINGERS_NOT_FOUND:
			fprintf(stdout,"ERROR_ID_USER_MULTI_FINGERS_NOT_FOUND");
			break;
		case	ERROR_ID_USERS_FULL:
			fprintf(stdout,"ERROR_ID_USERS_FULL");
			break;
		case	ERROR_ID_OPERATION_NOT_SUPPORTED:
			fprintf(stdout,"ERROR_ID_OPERATION_NOT_SUPPORTED");
			break;
		case	ERROR_ID_NOT_ENOUGH_SPACE:
			fprintf(stdout,"ERROR_ID_NOT_ENOUGH_SPACE");
			break;
		case	ERROR_ID_DUPLICATE:
			fprintf(stdout,"ERROR_ID_DUPLICATE");
			break;
		case	ERROR_CAPTURE_TIMEOUT:
			fprintf(stdout,"ERROR_CAPTURE_TIMEOUT");
			break;
		case	ERROR_CAPTURE_LATENT:
			fprintf(stdout,"ERROR_CAPTURE_LATENT");
			break;
		case	ERROR_CAPTURE_CANCELLED:
			fprintf(stdout,"ERROR_CAPTURE_CANCELLED");
			break;
		case	ERROR_CAPTURE_INTERNAL:
			fprintf(stdout,"ERROR_CAPTURE_INTERNAL");
			break;
		case	STATUS_NO_OP:
			fprintf(stdout,"STATUS_NO_OP");
			break;
		case	ERROR_UPDATE_MEMORY_ERROR:
			fprintf(stdout,"ERROR_UPDATE_MEMORY_ERROR");
			break;
		case	ERROR_UPDATE_DECRYPTION_FAIL:
			fprintf(stdout,"ERROR_UPDATE_DECRYPTION_FAIL");
			break;
		case	ERROR_UPDATE_FIRMWARE_VERSION_ERROR:
			fprintf(stdout,"ERROR_UPDATE_FIRMWARE_VERSION_ERROR");
			break;
		case	ERROR_UPDATE_FLASH_WRITE_ERROR:
			fprintf(stdout,"ERROR_UPDATE_FLASH_WRITE_ERROR");
			break;
		case	ERROR_UPDATE_INVALID_TYPE:
			fprintf(stdout,"ERROR_UPDATE_INVALID_TYPE");
			break;
		case	ERROR_UPDATE_FORMAT_ERROR:
			fprintf(stdout,"ERROR_UPDATE_FORMAT_ERROR");
			break;
		case	ERROR_UPDATE_FIRMWARE_SIZE_ERROR:
			fprintf(stdout,"ERROR_UPDATE_FIRMWARE_SIZE_ERROR");
			break;
        case    ERROR_UPDATE_RESTORE_FAIL:
            fprintf(stdout, "ERROR_UPDATE_RESTORE_FAIL");
            break;
        case    ERROR_CRYPTO_ERROR:
            fprintf(stdout, "ERROR_CRYPTO_ERROR");
            break;
        case    ERROR_UPDATE_FIRMWARE_INVALID:
            fprintf(stdout, "ERROR_FIRMWARE_INVALID");
            break;
	}

}

// Print Configuration structure
void PrintConfig(_V100_INTERFACE_CONFIGURATION_TYPE *config, uint serial_number)
{
	cout << endl << endl;
	cout << "Printing Configuration structure..." << endl;
	cout << "Device Serial Number: .............. " << serial_number << endl;
	cout << "Hardware Revision Number: .......... " << config -> Hardware_Rev << endl;
	cout << "Firmware Revision Number: .......... " << config -> Firmware_Rev << endl;
	cout << "Spoof Revision Number: ............. " << config -> Spoof_Rev << endl;
	cout << "PreProcessor Revision Number: ...... " << config -> PreProc_Rev << endl;
	cout << "Feature Extractor Revision Number: . " << config -> Feature_Extractor_Rev << endl;
	cout << "Matcher Revision Number: ........... " << config -> Matcher_Rev << endl;
	cout << "Identifier Revision Number: ........ " << config -> ID_Rev << endl;
}

// Print DB Metrics structure
void PrintDBMetrics(_MX00_DB_METRICS* dbMetrics)
{
	cout << endl << endl;
	cout << "Printing DB Metrics structure..." << endl;
	cout << "GroupID: " << dbMetrics->nGroupID << endl;
	cout << "MaxUsers: " << dbMetrics->nMaxUsers << endl;
	cout << "FingersPerUser: " << dbMetrics->nFingersPerUser << endl;
	cout << "InstancesPerFinger: " << dbMetrics->nInstancesPerFinger << endl;
	cout << "Flags: " << dbMetrics->nFlags << endl;
	cout << "CurEnrolledUserFingers: " << dbMetrics->nCurEnrolledUserFingers << endl;
}

// Multi-platform support for Sleep
void SleepVIK(int nMilliSecToSleep)
{
#ifdef __GNUC__
	usleep(nMilliSecToSleep*1000);
#else
	Sleep(nMilliSecToSleep);
#endif
}

// Multi-platform support for getting user input
int GetUserInput(void)
{
	int ch;
#ifdef _WIN32
#ifndef _WIN32_WCE
	ch = getch();
#else
	ch = getchar();
#endif
#else
	struct termios oldt, newt;
	tcgetattr( STDIN_FILENO, &oldt );
	newt = oldt;
	newt.c_lflag &= ~( ICANON | ECHO );
	tcsetattr( STDIN_FILENO, TCSANOW, &newt );
	ch = getchar();
	tcsetattr( STDIN_FILENO, TCSANOW, &oldt );
#endif
	return ch;
}
