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

#include "ICmd.h"
#include "IMemMgr.h"
#include "V100_shared_types.h"
#include "string.h"



class Macro_ID_Create_DB :  public ICmd
{
public:
	Macro_ID_Create_DB();
	~Macro_ID_Create_DB();
// ICmd
     // Takes content of Command, and packs it into pPacket
	virtual bool PackChallenge(uchar** pPacket, uint& nSize);
	// Unpacks packet passed in into interal data structure
	virtual bool UnpackChallenge(const uchar* pPacket, uint nSize);
    // Takes content of Command, and packs it into pPacket
	virtual bool PackResponse(uchar** pPacket, uint& nSize);
	// Unpacks packet passed in into interal data structure
	virtual bool UnpackResponse(const uchar* pPacket, uint nSize);
	// How large is the Challenge packet?
	virtual int  GetChallengeBufferSize();
	// How large is the Response packet?
	virtual int  GetResponseBufferSize();
	// 
	_MX00_DB_INIT_STRUCT*	GetDBInitParms() { return &m_DbCreateParms;};
	void					SetDBInitParms(_MX00_DB_INIT_STRUCT* pP) { memcpy(&m_DbCreateParms, pP, sizeof(_MX00_DB_INIT_STRUCT)); };
	_MX00_DB_INIT_STRUCT	m_DbCreateParms;

};
class Macro_ID_Set_Working_DB :  public ICmd
{
public:
	Macro_ID_Set_Working_DB();
	~Macro_ID_Set_Working_DB();
// ICmd
     // Takes content of Command, and packs it into pPacket
	virtual bool PackChallenge(uchar** pPacket, uint& nSize);
	// Unpacks packet passed in into interal data structure
	virtual bool UnpackChallenge(const uchar* pPacket, uint nSize);
    // Takes content of Command, and packs it into pPacket
	virtual bool PackResponse(uchar** pPacket, uint& nSize);
	// Unpacks packet passed in into interal data structure
	virtual bool UnpackResponse(const uchar* pPacket, uint nSize);
	// How large is the Challenge packet?
	virtual int  GetChallengeBufferSize();
	// How large is the Response packet?
	virtual int  GetResponseBufferSize();
	void SetGroupID(uint nDB) { m_nGroupID = nDB; }
	uint GetGroupID()		  { return m_nGroupID;}
private:
	// SPECIALIZED DATA GOES HERE
	uint	m_nGroupID;

};

class Atomic_ID_Get_User_Record :  public ICmd
{
public:
	Atomic_ID_Get_User_Record();
	~Atomic_ID_Get_User_Record();
// ICmd
     // Takes content of Command, and packs it into pPacket
	virtual bool PackChallenge(uchar** pPacket, uint& nSize);
	// Unpacks packet passed in into interal data structure
	virtual bool UnpackChallenge(const uchar* pPacket, uint nSize);
    // Takes content of Command, and packs it into pPacket
	virtual bool PackResponse(uchar** pPacket, uint& nSize);
	// Unpacks packet passed in into interal data structure
	virtual bool UnpackResponse(const uchar* pPacket, uint nSize);
	// How large is the Challenge packet?
	virtual int  GetChallengeBufferSize();
	// How large is the Response packet?
	virtual int  GetResponseBufferSize();
	// Set header 
	void	SetUserRecordHeader(_MX00_ID_USER_RECORD& rec) { m_UserRecordHeader = rec; };
	// Set a user record
	void	SetUserRecords(const _MX00_ID_USER_RECORD& rec, const _MX00_TEMPLATE_INSTANCE instanceRecords[]);
	// Get Record Header
	_MX00_ID_USER_RECORD* GetUserRecordHeader() { return &m_UserRecordHeader;}
	// Get Num Instances in this record
	uint	GetNumInstances() { return m_UserRecordHeader.nInstances;};
	// Get a single instance
	_MX00_TEMPLATE_INSTANCE*	GetFingerInstance(uint nInstance);
	// Get Templates
	bool GetTemplates(_MX00_TEMPLATE_INSTANCE instanceRecords[]);
	_MX00_TEMPLATE_INSTANCE* GetTemplates() { return m_pTemplateInstances; }
	_MX00_TEMPLATE_INSTANCE* GetNewTemplateBuffer(uint nNumInstances, uint nSizePerTemplate);
private:
	//
	_MX00_ID_USER_RECORD		m_UserRecordHeader;
	_MX00_TEMPLATE_INSTANCE*    m_pTemplateInstances;

};

class Atomic_ID_Get_User_Record_Header :  public ICmd
{
public:
	Atomic_ID_Get_User_Record_Header();
	~Atomic_ID_Get_User_Record_Header();
// ICmd
     // Takes content of Command, and packs it into pPacket
	virtual bool PackChallenge(uchar** pPacket, uint& nSize);
	// Unpacks packet passed in into interal data structure
	virtual bool UnpackChallenge(const uchar* pPacket, uint nSize);
    // Takes content of Command, and packs it into pPacket
	virtual bool PackResponse(uchar** pPacket, uint& nSize);
	// Unpacks packet passed in into interal data structure
	virtual bool UnpackResponse(const uchar* pPacket, uint nSize);
	// How large is the Challenge packet?
	virtual int  GetChallengeBufferSize();
	// How large is the Response packet?
	virtual int  GetResponseBufferSize();
	// Set header 
	void	SetUserRecordHeader(_MX00_ID_USER_RECORD& rec) { m_UserRecordHeader = rec; };
	// Set a user record
	void	SetUserRecords(const _MX00_ID_USER_RECORD& rec, const _MX00_TEMPLATE_INSTANCE instanceRecords[]);
	// Get Record Header
	_MX00_ID_USER_RECORD* GetUserRecordHeader() { return &m_UserRecordHeader;}
	// Get Num Instances in this record
	uint	GetNumInstances() { return m_UserRecordHeader.nInstances;};
private:
	//
	_MX00_ID_USER_RECORD		m_UserRecordHeader;
};

class Atomic_ID_Set_User_Record :  public ICmd
{
public:
	Atomic_ID_Set_User_Record();
	~Atomic_ID_Set_User_Record();
// ICmd
     // Takes content of Command, and packs it into pPacket
	virtual bool PackChallenge(uchar** pPacket, uint& nSize);
	// Unpacks packet passed in into interal data structure
	virtual bool UnpackChallenge(const uchar* pPacket, uint nSize);
    // Takes content of Command, and packs it into pPacket
	virtual bool PackResponse(uchar** pPacket, uint& nSize);
	// Unpacks packet passed in into interal data structure
	virtual bool UnpackResponse(const uchar* pPacket, uint nSize);
	// How large is the Challenge packet?
	virtual int  GetChallengeBufferSize();
	// How large is the Response packet?
	virtual int  GetResponseBufferSize();
	void	SetUserRecords(const _MX00_ID_USER_RECORD& rec, const _MX00_TEMPLATE_INSTANCE instanceRecords[]);
	_MX00_ID_USER_RECORD* GetUserRecordHeader() { return &m_UserRecordHeader;}
	uint	GetNumInstances() { return m_UserRecordHeader.nInstances;};
	_MX00_TEMPLATE_INSTANCE*	GetFingerInstance(uint nInstance);
private:
	// SPECIALIZED DATA GOES HERE
	_MX00_ID_USER_RECORD		m_UserRecordHeader;
	_MX00_TEMPLATE_INSTANCE*    m_pTemplateInstances;
};
class Macro_ID_Enroll_User_Record :  public ICmd
{
public:
	Macro_ID_Enroll_User_Record();
	~Macro_ID_Enroll_User_Record();
// ICmd
     // Takes content of Command, and packs it into pPacket
	virtual bool PackChallenge(uchar** pPacket, uint& nSize);
	// Unpacks packet passed in into interal data structure
	virtual bool UnpackChallenge(const uchar* pPacket, uint nSize);
    // Takes content of Command, and packs it into pPacket
	virtual bool PackResponse(uchar** pPacket, uint& nSize);
	// Unpacks packet passed in into interal data structure
	virtual bool UnpackResponse(const uchar* pPacket, uint nSize);
	// How large is the Challenge packet?
	virtual int  GetChallengeBufferSize();
	// How large is the Response packet?
	virtual int  GetResponseBufferSize();
	//
	virtual void SetUserRecord(_MX00_ID_USER_RECORD* pUR) { memcpy(&m_nUserRecord, pUR, sizeof(_MX00_ID_USER_RECORD)); }
	virtual _MX00_ID_USER_RECORD* GetUserRecord() { return &m_nUserRecord; }
private:
	// SPECIALIZED DATA GOES HERE
	_MX00_ID_USER_RECORD	m_nUserRecord;
};

class Macro_ID_Identify_378 :  public ICmd
{
public:
	Macro_ID_Identify_378();
	~Macro_ID_Identify_378();
// ICmd
     // Takes content of Command, and packs it into pPacket
	virtual bool PackChallenge(uchar** pPacket, uint& nSize);
	// Unpacks packet passed in into interal data structure
	virtual bool UnpackChallenge(const uchar* pPacket, uint nSize);
    // Takes content of Command, and packs it into pPacket
	virtual bool PackResponse(uchar** pPacket, uint& nSize);
	// Unpacks packet passed in into interal data structure
	virtual bool UnpackResponse(const uchar* pPacket, uint nSize);
	// How large is the Challenge packet?
	virtual int  GetChallengeBufferSize();
	// How large is the Response packet?
	virtual int  GetResponseBufferSize();
	//
	bool	SetTemplate(uchar* p378Template, uint nSizeTemplate);
	uchar*  GetTemplate() { return m_pTemplate; }
	uint	GetTemplateSize() { return m_nTemplateSize;}
private:
	// 
	uchar*	m_pTemplate;
	uint	m_nTemplateSize;

};
class Macro_ID_Identify :  public ICmd
{
public:
	Macro_ID_Identify();
	~Macro_ID_Identify();
// ICmd
     // Takes content of Command, and packs it into pPacket
	virtual bool PackChallenge(uchar** pPacket, uint& nSize);
	// Unpacks packet passed in into interal data structure
	virtual bool UnpackChallenge(const uchar* pPacket, uint nSize);
    // Takes content of Command, and packs it into pPacket
	virtual bool PackResponse(uchar** pPacket, uint& nSize);
	// Unpacks packet passed in into interal data structure
	virtual bool UnpackResponse(const uchar* pPacket, uint nSize);
	// How large is the Challenge packet?
	virtual int  GetChallengeBufferSize();
	// How large is the Response packet?
	virtual int  GetResponseBufferSize();
	// Get/Set User Record
	bool	GetUserRecord(_V100_USER_RECORD* pRec) { memcpy(pRec, &m_UserRecord, sizeof(m_UserRecord)); return true;}
	bool	SetUserRecord(_V100_USER_RECORD* pRec) { memcpy(&m_UserRecord, pRec, sizeof(m_UserRecord)); return true;}
private:
	_V100_USER_RECORD m_UserRecord;

};
class Macro_ID_Delete_DB :  public ICmd
{
public:
	Macro_ID_Delete_DB();
	~Macro_ID_Delete_DB();
// ICmd
     // Takes content of Command, and packs it into pPacket
	virtual bool PackChallenge(uchar** pPacket, uint& nSize);
	// Unpacks packet passed in into interal data structure
	virtual bool UnpackChallenge(const uchar* pPacket, uint nSize);
    // Takes content of Command, and packs it into pPacket
	virtual bool PackResponse(uchar** pPacket, uint& nSize);
	// Unpacks packet passed in into interal data structure
	virtual bool UnpackResponse(const uchar* pPacket, uint nSize);
	// How large is the Challenge packet?
	virtual int  GetChallengeBufferSize();
	// How large is the Response packet?
	virtual int  GetResponseBufferSize();
	//
	void	SetDBToDelete(uint nDBToDelete) { m_nDBToDelete = nDBToDelete; }
	uint	GetDBToDelete() { return m_nDBToDelete; }
private:
	uint	m_nDBToDelete;

};

class Atomic_ID_Get_DB_Metrics :  public ICmd
{
public:
	Atomic_ID_Get_DB_Metrics();
	~Atomic_ID_Get_DB_Metrics();
// ICmd
     // Takes content of Command, and packs it into pPacket
	virtual bool PackChallenge(uchar** pPacket, uint& nSize);
	// Unpacks packet passed in into interal data structure
	virtual bool UnpackChallenge(const uchar* pPacket, uint nSize);
    // Takes content of Command, and packs it into pPacket
	virtual bool PackResponse(uchar** pPacket, uint& nSize);
	// Unpacks packet passed in into interal data structure
	virtual bool UnpackResponse(const uchar* pPacket, uint nSize);
	// How large is the Challenge packet?
	virtual int  GetChallengeBufferSize();
	// How large is the Response packet?
	virtual int  GetResponseBufferSize();
	void SetDBMetrics(_MX00_DB_METRICS dM) { m_dbMetrics = dM;}
	_MX00_DB_METRICS* GetDBMetrics() { return &m_dbMetrics;}	
private:
	// SPECIALIZED DATA GOES HERE
	_MX00_DB_METRICS	m_dbMetrics;
};

class Atomic_ID_Get_System_Metrics :  public ICmd
{
public:
	Atomic_ID_Get_System_Metrics();
	~Atomic_ID_Get_System_Metrics();
// ICmd
     // Takes content of Command, and packs it into pPacket
	virtual bool PackChallenge(uchar** pPacket, uint& nSize);
	// Unpacks packet passed in into interal data structure
	virtual bool UnpackChallenge(const uchar* pPacket, uint nSize);
    // Takes content of Command, and packs it into pPacket
	virtual bool PackResponse(uchar** pPacket, uint& nSize);
	// Unpacks packet passed in into interal data structure
	virtual bool UnpackResponse(const uchar* pPacket, uint nSize);
	// How large is the Challenge packet?
	virtual int  GetChallengeBufferSize();
	// How large is the Response packet?
	virtual int  GetResponseBufferSize();
	void SetNumMetrics(uint nMetrics);
	uint GetNumMetrics() { return m_nMetricsTracked; }
	void AddMetric(_MX00_DB_METRICS* pMetric, uint nMetricTracked);
	_MX00_DB_METRICS* GetDBMetrics(uint nIndex) { return &m_pDBMetrics[nIndex];}	
private:
	// SPECIALIZED DATA GOES HERE
	_MX00_DB_METRICS*	m_pDBMetrics;
	uint				m_nMetricsTracked;
};

class Atomic_ID_Get_Result :  public ICmd
{
public:
	Atomic_ID_Get_Result();
	~Atomic_ID_Get_Result();
// ICmd
     // Takes content of Command, and packs it into pPacket
	virtual bool PackChallenge(uchar** pPacket, uint& nSize);
	// Unpacks packet passed in into interal data structure
	virtual bool UnpackChallenge(const uchar* pPacket, uint nSize);
    // Takes content of Command, and packs it into pPacket
	virtual bool PackResponse(uchar** pPacket, uint& nSize);
	// Unpacks packet passed in into interal data structure
	virtual bool UnpackResponse(const uchar* pPacket, uint nSize);
	// How large is the Challenge packet?
	virtual int  GetChallengeBufferSize();
	// How large is the Response packet?
	virtual int  GetResponseBufferSize();
	void SetResult(_MX00_ID_RESULT res) { m_nID_Result = res; }
	_MX00_ID_RESULT* GetResult() { return &m_nID_Result; }
private:
	// SPECIALIZED DATA GOES HERE
	_MX00_ID_RESULT	m_nID_Result;
};

class Atomic_ID_Delete_User_Record :  public ICmd
{
public:
	Atomic_ID_Delete_User_Record();
	~Atomic_ID_Delete_User_Record();
// ICmd
     // Takes content of Command, and packs it into pPacket
	virtual bool PackChallenge(uchar** pPacket, uint& nSize);
	// Unpacks packet passed in into interal data structure
	virtual bool UnpackChallenge(const uchar* pPacket, uint nSize);
    // Takes content of Command, and packs it into pPacket
	virtual bool PackResponse(uchar** pPacket, uint& nSize);
	// Unpacks packet passed in into interal data structure
	virtual bool UnpackResponse(const uchar* pPacket, uint nSize);
	// How large is the Challenge packet?
	virtual int  GetChallengeBufferSize();
	// How large is the Response packet?
	virtual int  GetResponseBufferSize();
	// Get/Set User record
	void SetUserRecord(_MX00_ID_USER_RECORD& ur) { memcpy(&m_UserRecord, &ur, sizeof(_MX00_ID_USER_RECORD)); };
	_MX00_ID_USER_RECORD& GetUserRecord( ) { return m_UserRecord; }
private:
	// SPECIALIZED DATA GOES HERE
	_MX00_ID_USER_RECORD		m_UserRecord;
};

class Atomic_ID_Set_Parameters:  public ICmd
{
public:
	Atomic_ID_Set_Parameters();
	~Atomic_ID_Set_Parameters();
// ICmd
     // Takes content of Command, and packs it into pPacket
	virtual bool PackChallenge(uchar** pPacket, uint& nSize);
	// Unpacks packet passed in into interal data structure
	virtual bool UnpackChallenge(const uchar* pPacket, uint nSize);
    // Takes content of Command, and packs it into pPacket
	virtual bool PackResponse(uchar** pPacket, uint& nSize);
	// Unpacks packet passed in into interal data structure
	virtual bool UnpackResponse(const uchar* pPacket, uint nSize);
	// How large is the Challenge packet?
	virtual int  GetChallengeBufferSize();
	// How large is the Response packet?
	virtual int  GetResponseBufferSize();
	_MX00_ID_PARAMETERS* GetParameters() { return &m_Parameters; }
	void SetParameters(_MX00_ID_PARAMETERS* pM) { memcpy(&m_Parameters, pM, sizeof(m_Parameters));} 
private:
	// SPECIALIZED DATA GOES HERE
	_MX00_ID_PARAMETERS	m_Parameters;
};

class Atomic_ID_Get_Parameters:  public ICmd
{
public:
	Atomic_ID_Get_Parameters();
	~Atomic_ID_Get_Parameters();
// ICmd
     // Takes content of Command, and packs it into pPacket
	virtual bool PackChallenge(uchar** pPacket, uint& nSize);
	// Unpacks packet passed in into interal data structure
	virtual bool UnpackChallenge(const uchar* pPacket, uint nSize);
    // Takes content of Command, and packs it into pPacket
	virtual bool PackResponse(uchar** pPacket, uint& nSize);
	// Unpacks packet passed in into interal data structure
	virtual bool UnpackResponse(const uchar* pPacket, uint nSize);
	// How large is the Challenge packet?
	virtual int  GetChallengeBufferSize();
	// How large is the Response packet?
	virtual int  GetResponseBufferSize();
	_MX00_ID_PARAMETERS* GetParameters() { return &m_Parameters; }
	void SetParameters(_MX00_ID_PARAMETERS* pM) { memcpy(&m_Parameters, pM, sizeof(m_Parameters));} 
private:
	// SPECIALIZED DATA GOES HERE
	_MX00_ID_PARAMETERS	m_Parameters;
};

// CMD_XXX_XXXX *************************

class Macro_ID_Verify_User_Record :  public ICmd
{
public:
	Macro_ID_Verify_User_Record();
	~Macro_ID_Verify_User_Record();
// ICmd
     // Takes content of Command, and packs it into pPacket
	virtual bool PackChallenge(uchar** pPacket, uint& nSize);
	// Unpacks packet passed in into interal data structure
	virtual bool UnpackChallenge(const uchar* pPacket, uint nSize);
    // Takes content of Command, and packs it into pPacket
	virtual bool PackResponse(uchar** pPacket, uint& nSize);
	// Unpacks packet passed in into interal data structure
	virtual bool UnpackResponse(const uchar* pPacket, uint nSize);
	// How large is the Challenge packet?
	virtual int  GetChallengeBufferSize();
	// How large is the Response packet?
	virtual int  GetResponseBufferSize();
	// Set User Record
	void	SetUserRecord(_MX00_ID_USER_RECORD* pUR) { memcpy(&m_UserRecord, pUR, sizeof(_MX00_ID_USER_RECORD)); }
	_MX00_ID_USER_RECORD* GetUserRecord() { return &m_UserRecord; }
private:
	// SPECIALIZED DATA GOES HERE
	_MX00_ID_USER_RECORD	m_UserRecord;
};

// CMD_XXX_XXXX *************************

class Macro_ID_Verify_378 :  public ICmd
{
public:
	Macro_ID_Verify_378();
	~Macro_ID_Verify_378();
// ICmd
     // Takes content of Command, and packs it into pPacket
	virtual bool PackChallenge(uchar** pPacket, uint& nSize);
	// Unpacks packet passed in into interal data structure
	virtual bool UnpackChallenge(const uchar* pPacket, uint nSize);
    // Takes content of Command, and packs it into pPacket
	virtual bool PackResponse(uchar** pPacket, uint& nSize);
	// Unpacks packet passed in into interal data structure
	virtual bool UnpackResponse(const uchar* pPacket, uint nSize);
	// How large is the Challenge packet?
	virtual int  GetChallengeBufferSize();
	// How large is the Response packet?
	virtual int  GetResponseBufferSize();
	void	SetUserRecord(_MX00_ID_USER_RECORD* pUR) { memcpy(&m_UserRecord, pUR, sizeof(_MX00_ID_USER_RECORD)); }
	void	SetTemplate(uchar* pTpl, uint nTplSize);
	uchar*  GetTemplate() { return m_pTemplate; }
	uint	GetTemplateSize()  { return m_nTemplateSize; }
	_MX00_ID_USER_RECORD* GetUserRecord() { return &m_UserRecord; }
	int*	GetScore() { return &m_nScore; }
private:
	// SPECIALIZED DATA GOES HERE
	_MX00_ID_USER_RECORD	m_UserRecord;
	uint					m_nTemplateSize;
	uchar*					m_pTemplate;
	int						m_nScore;
};

// CMD_ID_VERIFY_MANY *************************

class Macro_ID_Verify_Many :  public ICmd
{
public:
	Macro_ID_Verify_Many();
	~Macro_ID_Verify_Many();
// ICmd
     // Takes content of Command, and packs it into pPacket
	virtual bool PackChallenge(uchar** pPacket, uint& nSize);
	// Unpacks packet passed in into interal data structure
	virtual bool UnpackChallenge(const uchar* pPacket, uint nSize);
    // Takes content of Command, and packs it into pPacket
	virtual bool PackResponse(uchar** pPacket, uint& nSize);
	// Unpacks packet passed in into interal data structure
	virtual bool UnpackResponse(const uchar* pPacket, uint nSize);
	// How large is the Challenge packet?
	virtual int  GetChallengeBufferSize();
	// How large is the Response packet?
	virtual int  GetResponseBufferSize();
	bool SetTemplates(uchar** pTemplates, uint* pSizeTemplates, uint nNumTemplates);
	uint GetNumTemplates() {return m_nNumTemplates;}
	uchar* GetTemplates() {return m_pTemplates;}
	uint* GetTemplatesSz() {return m_pSzTemplates;}
	uint GetAllTemplatesSz() {return m_nSzAllTemplates;}
private:
	// SPECIALIZED DATA GOES HERE
	uchar* m_pTemplates;
	uint*  m_pSzTemplates;
	uint   m_nNumTemplates;
	uint   m_nSzAllTemplates;

};

// CMD_VERIFY_378 *************************

class Macro_Verify_378 :  public ICmd
{
public:
	Macro_Verify_378();
	~Macro_Verify_378();
// ICmd
     // Takes content of Command, and packs it into pPacket
	virtual bool PackChallenge(uchar** pPacket, uint& nSize);
	// Unpacks packet passed in into interal data structure
	virtual bool UnpackChallenge(const uchar* pPacket, uint nSize);
    // Takes content of Command, and packs it into pPacket
	virtual bool PackResponse(uchar** pPacket, uint& nSize);
	// Unpacks packet passed in into interal data structure
	virtual bool UnpackResponse(const uchar* pPacket, uint nSize);
	// How large is the Challenge packet?
	virtual int  GetChallengeBufferSize();
	// How large is the Response packet?
	virtual int  GetResponseBufferSize();
	// SPECIALIZED DATA GOES HERE
	bool	SetGalleryTemplateData(uchar* pTemplate, uint nTemplateSize); 
	uchar*  GetGalleryTemplate() { return m_pGalleryTemplate; }
	uint    GetGalleryTemplateSize() { return m_nGalleryTemplateSize; };
private:
	// SPECIALIZED DATA GOES HERE
	uchar*	m_pGalleryTemplate;
	uint	m_nGalleryTemplateSize;
};

// CMD_XXX_XXXX *************************

class Macro_Save_Last_Capture :  public ICmd
{
public:
	Macro_Save_Last_Capture();
	~Macro_Save_Last_Capture();
// ICmd
     // Takes content of Command, and packs it into pPacket
	virtual bool PackChallenge(uchar** pPacket, uint& nSize);
	// Unpacks packet passed in into interal data structure
	virtual bool UnpackChallenge(const uchar* pPacket, uint nSize);
    // Takes content of Command, and packs it into pPacket
	virtual bool PackResponse(uchar** pPacket, uint& nSize);
	// Unpacks packet passed in into interal data structure
	virtual bool UnpackResponse(const uchar* pPacket, uint nSize);
	// How large is the Challenge packet?
	virtual int  GetChallengeBufferSize();
	// How large is the Response packet?
	virtual int  GetResponseBufferSize();
private:
	// SPECIALIZED DATA GOES HERE

};

// CMD_UPDATE_FIRMWARE *************************

class Macro_Update_Firmware :  public ICmd
{
public:
	Macro_Update_Firmware();
	~Macro_Update_Firmware();
// ICmd
     // Takes content of Command, and packs it into pPacket
	virtual bool PackChallenge(uchar** pPacket, uint& nSize);
	// Unpacks packet passed in into interal data structure
	virtual bool UnpackChallenge(const uchar* pPacket, uint nSize);
    // Takes content of Command, and packs it into pPacket
	virtual bool PackResponse(uchar** pPacket, uint& nSize);
	// Unpacks packet passed in into interal data structure
	virtual bool UnpackResponse(const uchar* pPacket, uint nSize);
	// How large is the Challenge packet?
	virtual int  GetChallengeBufferSize();
	// How large is the Response packet?
	virtual int  GetResponseBufferSize();
	//
	bool SetData(uchar* pData, uint nDataSize);
	uchar* GetData();
	uint   GetDataSize();
private:
	// SPECIALIZED DATA GOES HERE
	uchar*  m_pFWData;
	uint	m_nDataSize;
};

