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
#include "V100_shared_types.h"

/******************************************************************************
**
**	Pre-defined policy levels for ease of use.
**	
**	
**  
**  
**
******************************************************************************/

extern "C"
{
    

_V100_DEVICE_POLICY POLICY_ALL = 
{
	1,					//Get_Raw_Image
	1,					//Set_Raw_Image
	1,					//Get_Scaled_Image
	1,					//Set_Scaled_Image
	1,					//Get_Composite_Image
	1,					//Set_Composite_Image
	1,					//Get_Template
	1,					//Set_Template
	1,					//Get_Cal
	1,					//Set_Cal
	1,					//Get_Config
	1,					//Set_Config
	1,					//Get_Cmd
	1,					//Set_Cmd
	1,					//Set_Record
	1,					//Get_Record
	1,					//Write_Flash
	1,					//Set_LED
	1,					//Set_License_Key
	1,					//Get_License_Key
	1,					//Set_Enrollment_Rules
	1,					//Calculate_Match
	1,					//Calculate_Extract
	1,					//Calculate_Identify
	1,					//Calculate_Verify
	1,					//Set_Spoof_Info
	1,					//Calculate_Spoof_1
	1,					//Calculate_Spoof_2
	1,					//Calculate_Spoof_3
	1,					//System_Diagnostics
	1,					//Vid_Stream
};


/******************************************************************************
**
**	Policy Structure Policy_Level_1
**	
**	Default Level -- Calculate all we can
**  No setting images, getting raw images, writing flash, etc...
**  Note:  a backdoor can be enabled to replace this file in memory.
**
******************************************************************************/


_V100_DEVICE_POLICY POLICY_LEVEL_1 = 
{
	0,					//Get_Raw_Image
	0,					//Set_Raw_Image
	0,					//Get_Scaled_Image
	0,					//Set_Scaled_Image
	1,					//Get_Composite_Image
	1,					//Set_Composite_Image
	1,					//Get_Template
	1,					//Set_Template
	0,					//Get_Cal
	0,					//Set_Cal
	1,					//Get_Config
	1,					//Set_Config
	1,					//Get_Cmd
	1,					//Set_Cmd
	0,					//Set_Record
	0,					//Get_Record
	0,					//Write_Flash
	1,					//Set_LED
	0,					//Set_License_Key
	0,					//Get_License_Key
	1,					//Set_Enrollment_Rules
	1,					//Calculate_Match
	1,					//Calculate_Extract
	0,					//Calculate_Identify
	0,					//Calculate_Verify
	0,					//Set_Spoof_Info
	0,					//Calculate_Spoof_1
	0,					//Calculate_Spoof_2
	0,					//Calculate_Spoof_3
	0,					//System_Diagnostics
	1,					//Vid_Stream
};

/******************************************************************************
**
**	Policy Structure Policy_Level_2
**	
**	Level 2 - Get composite image ONLY, no extraction or matching.
**  No setting images, getting raw images, writing flash, etc...
**  Note:  a backdoor can be enabled to replace this file in memory.
**
******************************************************************************/


_V100_DEVICE_POLICY POLICY_LEVEL_2 = 
{
	0,					//Get_Raw_Image
	0,					//Set_Raw_Image
	0,					//Get_Scaled_Image
	0,					//Set_Scaled_Image
	1,					//Get_Composite_Image
	0,					//Set_Composite_Image
	0,					//Get_Template
	0,					//Set_Template
	0,					//Get_Cal
	0,					//Set_Cal
	1,					//Get_Config
	0,					//Set_Config
	1,					//Get_Cmd
	1,					//Set_Cmd
	0,					//Set_Record
	0,					//Get_Record
	0,					//Write_Flash
	1,					//Set_LED
	0,					//Set_License_Key
	0,					//Get_License_Key
	0,					//Set_Enrollment_Rules
	0,					//Calculate_Match
	0,					//Calculate_Extract
	0,					//Calculate_Identify
	0,					//Calculate_Verify
	0,					//Set_Spoof_Info
	1,					//Calculate_Spoof_1
	0,					//Calculate_Spoof_2
	0,					//Calculate_Spoof_3
	0,					//System_Diagnostics
	1,					//Vid_Stream
};

/******************************************************************************
**
**	Policy Structure Policy_Level_3
**	
**	Level 2 - Get composite image ONLY, no extraction or matching.
**  No setting images, getting raw images, writing flash, etc...
**  Note:  a backdoor can be enabled to replace this file in memory.
**
******************************************************************************/


_V100_DEVICE_POLICY POLICY_LEVEL_3 = 
{
	0,					//Get_Raw_Image
	0,					//Set_Raw_Image
	0,					//Get_Scaled_Image
	0,					//Set_Scaled_Image
	0,					//Get_Composite_Image
	0,					//Set_Composite_Image
	0,					//Get_Template
	0,					//Set_Template
	0,					//Get_Cal
	0,					//Set_Cal
	1,					//Get_Config
	0,					//Set_Config
	1,					//Get_Cmd
	0,					//Set_Cmd
	0,					//Set_Record
	0,					//Get_Record
	0,					//Write_Flash
	0,					//Set_LED
	0,					//Set_License_Key
	0,					//Get_License_Key
	0,					//Set_Enrollment_Rules
	0,					//Calculate_Match
	0,					//Calculate_Extract
	0,					//Calculate_Identify
	0,					//Calculate_Verify
	0,					//Set_Spoof_Info
	0,					//Calculate_Spoof_1
	0,					//Calculate_Spoof_2
	0,					//Calculate_Spoof_3
	0,					//System_Diagnostics
	0,					//Vid_Stream
};



};

