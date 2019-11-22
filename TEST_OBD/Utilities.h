
/**
 ******************************************************************************
 * @file		Utilities.h
 * @author		Zhww
 * @version		V1.0
 * @date		���� 2018
 * @brief
 ******************************************************************************
 * @attention
 * ��Ӧ��ʹ���Ѷ��ļ����Ժܸߵ���䣬���Ǻ��б�Ҫʱ��
 * �߼��ɵ���䲻���ڸ�Ч�ʵĳ���ʵ���ϳ����Ч�ʹؼ������㷨��
 *  �� 
 * //*pu8Data++ +=1��
 * //*p ++pu8Data +=1;
 * //Ӧ�÷ֱ��Ϊ���£�
 * //*pu8Data +=1;    pu8Data++;// C�˶���乲�Ǹ����൱��"*pu8Data++ +1;"
 * @Contact:  
 ******************************************************************************
 *
 */
#pragma once
//#include "stdafx.h"
#include <string>


typedef unsigned short	 INT16U;

#if defined(UNICODE) || defined(_UNICODE)
typedef std::wstring tstring;
#else
typedef std::string tstring;
#endif

namespace Utilities
{



	//��װ��ģ��·���� CPath
	class CPath
	{
	public:
		CPath(void);
		~CPath(void);

		static tstring GetAppPath();												// ��ȡӦ�ó���ִ��·��
		static std::string GetAppPathAscii();										// ��ȡӦ�ó���ִ��·��
		static tstring GetCurDir();													// ��ȡӦ�ó���ǰĿ¼
		static tstring GetTempPath();												// ��ȡ��ǰϵͳ����ʱ�ļ��е�·��
		static tstring GetTempFileName(LPCTSTR lpszFileName);						// ��ȡ��ǰϵͳ����ʱ�ļ��е�·���µ�Ψһ��������ʱ�ļ���(ȫ·��)
		static tstring GetRandomFileName(LPCTSTR lpszPath, LPCTSTR lpszFileName);	// ��ȡ����ļ���(ȫ·��)

		static BOOL IsDirectory(LPCTSTR lpszPath);									// ���ָ��·���Ƿ�Ŀ¼
		static BOOL IsFileExist(LPCTSTR lpszFileName);								// ���ָ���ļ��Ƿ����
		static BOOL IsDirectoryExist(LPCTSTR lpszPath);								// ���ָ��Ŀ¼�Ƿ����
		static BOOL CreateDirectory(LPCTSTR lpPathName, LPSECURITY_ATTRIBUTES lpSecurityAttributes = NULL);

		static tstring GetPathRoot(LPCTSTR lpszPath);								// ��ȡָ��·���ĸ�Ŀ¼��Ϣ
		static tstring GetDirectoryName(LPCTSTR lpszPath);							// ����ָ��·���ַ�����Ŀ¼��Ϣ
		static tstring GetFileName(LPCTSTR lpszPath);								// ����ָ��·���ַ������ļ�������չ��
		static tstring GetFileNameWithoutExtension(LPCTSTR lpszPath);				// ���ز�������չ����·���ַ������ļ���
		static tstring GetExtension(LPCTSTR lpszPath);								// ����ָ����·���ַ�������չ��
		static tstring GetFullPath(LPCTSTR lpszPath);								// ����ָ�������·����ȡ����·��
		static long    GetFileSize(LPCTSTR lpszPath);								// ��ȡ�ļ���С
	};



	class CIniFile
	{
	public:
		CIniFile();
		~CIniFile();

	public:
		UINT ReadInt(LPCTSTR lpAppName, LPCTSTR lpKeyName, long nDefault, LPCTSTR lpFileName);
		BOOL ReadString(LPCTSTR lpAppName, LPCTSTR lpKeyName, LPCTSTR lpDefault, LPTSTR lpReturnedString, DWORD nSize, LPCTSTR lpFileName);
		BOOL WriteInt(LPCTSTR lpAppName, LPCTSTR lpKeyName, long nValue, LPCTSTR lpFileName);
		BOOL WriteString(LPCTSTR lpAppName, LPCTSTR lpKeyName, LPCTSTR lpString, LPCTSTR lpFileName);
	};


	enum LOG_LEVEL
	{
		LOG_LEVEL_INFO,
		LOG_LEVEL_WARNING,
		LOG_LEVEL_ERROR
	};

	//ע�⣺�����ӡ����־��Ϣ�������ģ����ʽ���ַ���Ҫ��_T()�����������(...)�ɱ����
	//e.g. LOG_INFO(_T("GroupID=%u, GroupName=%s, GroupName=%s."), lpGroupInfo->m_nGroupCode, lpGroupInfo->m_strAccount.c_str(), lpGroupInfo->m_strName.c_str());
//����һ�������� ��Ϊ���������ӡ����Ϣ̫ȫ�������ǲ���Ҫ
	// �������ĸ��궼��4������ ����.   (��3���� ... �������һ��������__VA_ARGS__)
#define LOG_INFO(...)     Utilities::CIULog::Log(Utilities::LOG_LEVEL_INFO, __FUNCSIG__,__LINE__, __VA_ARGS__)//__FUNCSIG__ ������Ԥ���� �������ַ���˭����__VA_ARGS__ �ɱ����
#define LOG_WARNING(...)   Utilities::CIULog::Log(Utilities::LOG_LEVEL_WARNING, __FUNCSIG__, __LINE__,__VA_ARGS__)
#define LOG_ERROR(...)     Utilities::CIULog::Log(Utilities::LOG_LEVEL_ERROR, __FUNCSIG__,__LINE__, __VA_ARGS__)

	class  CIULog
	{
	public:
		static bool Init(bool bToFile, bool bTruncateLongLog, PCTSTR pszLogFileName);
		static void Uninit();

		static void SetLevel(LOG_LEVEL nLevel);

		//������߳�ID�ź����ں���ǩ�����к�
		static bool Log(long nLevel, PCTSTR pszFmt, ...);
		//����߳�ID�ź����ں���ǩ�����к�
		//��4���� ����Ҫ��4������������������ĺ꣬���������滻����Ҳ���˸����� (...��ᱻ���һ���滻) �� ���������� һ��unicode PCTSTR pszFmt �汾һ��char * PCSTR pszFmt�汾  
		static bool Log(long nLevel, PCSTR pszFunctionSig, int nLineNo, PCTSTR pszFmt, ...);		//ע��:pszFunctionSig����ΪAnsic�汾
		static bool Log(long nLevel, PCSTR pszFunctionSig, int nLineNo, PCSTR pszFmt, ...);
	private:
		CIULog() = delete;
		~CIULog() = delete;

		CIULog(const CIULog& rhs) = delete;
		CIULog& operator=(const CIULog& rhs) = delete;

		static void GetTime(char* pszTime, int nTimeStrLength);

	private:
		static bool		        m_bToFile;			    //��־д���ļ�����д������̨  
		static HANDLE	        m_hLogFile;
		static bool             m_bTruncateLongLog;     //����־�Ƿ�ض�
		static LOG_LEVEL        m_nLogLevel;            //��־����
	};



	/**
	* @class	EncodeConvert
	* @brief    ��ͬ����֮���ת���� EncodeConvert
	* @author   Zhww
	*/

	class EncodeConvert
	{
	public:
		//===BEGIN: ע�⣺����6������,��Ҫ���ⲿ�ͷŷ��ص��ַ���ָ�룬��������ڴ�й¶
		static wchar_t* EncodeConvert::AnsiToUnicode(const char* lpszStr);
		static char* UnicodeToAnsi(const wchar_t* lpszStr);
		static char* AnsiToUtf8(const char* lpszStr);
		static char* Utf8ToAnsi(const char* lpszStr);
		static char* UnicodeToUtf8(const wchar_t* lpszStr);
		static wchar_t* Utf8ToUnicode(const char* lpszStr);
		//===END: ע�⣺����6������,��Ҫ���ⲿ�ͷŷ��ص��ַ���ָ�룬��������ڴ�й¶

		//===BEGIN: ���º�����һ����������Ҫת����Դ�ַ���ָ�룬�ڶ��������Ǵ��ת�����Ŀ�껺����ָ�룬������������Ŀ�껺�����Ĵ�С
		static bool EncodeConvert::AnsiToUnicode(const char* lpszAnsi, wchar_t* lpszUnicode, int nLen);
		static bool UnicodeToAnsi(const wchar_t* lpszUnicode, char* lpszAnsi, int nLen);
		static bool AnsiToUtf8(const char* lpszAnsi, char* lpszUtf8, int nLen);
		static bool Utf8ToAnsi(const char* lpszUtf8, char* lpszAnsi, int nLen);
		static bool UnicodeToUtf8(const wchar_t* lpszUnicode, char* lpszUtf8, int nLen);
		static bool Utf8ToUnicode(const char* lpszUtf8, wchar_t* lpszUnicode, int nLen);
		//===END: ���º�����һ����������Ҫת����Դ�ַ���ָ�룬�ڶ��������Ǵ��ת�����Ŀ�껺����ָ�룬������������Ŀ�껺�����Ĵ�С

		static std::wstring EncodeConvert::AnsiToUnicode(const std::string& strAnsi);
		static std::string UnicodeToAnsi(const std::wstring& strUnicode);
		static std::string AnsiToUtf8(const std::string& strAnsi);
		static std::string Utf8ToAnsi(const std::string& strUtf8);
		static std::string UnicodeToUtf8(const std::wstring& strUnicode);
		static std::wstring Utf8ToUnicode(const std::string& strUtf8);

	private:
		EncodeConvert() = delete; //ɾ�����캯���޷�ʵ����
		~EncodeConvert() = delete;

		EncodeConvert(const EncodeConvert& rhs) = delete;//C++ 11���÷���ֹ ��������
		EncodeConvert& operator=(const EncodeConvert& rhs) = delete;//��ֹ��ֵ�ȺŲ���
	};
	std::wstring UnicodeToHexStr(const WCHAR* lpStr, BOOL bDblSlash);
	//����2������  char  ��wchar
	BOOL my_isalnum(unsigned char cChar);		// ���ָ���ַ��Ƿ�����ĸ(A-Z��a-z)������(0-9)
	BOOL my_isalnum(wchar_t cChar);				// ���ָ���ַ��Ƿ�����ĸ(A-Z��a-z)������(0-9)


	BYTE ASC2BYTE(BYTE asc1, BYTE asc2);
	INT16U CRC16(unsigned char* pushMsg, unsigned short usDataLen,unsigned char mode);
	//ֻ�е�8���á�return check & 0x00ff 
	//INT16U Checksum_XOR(unsigned char* buf, INT16U len);	//���У�飬�����ֽ����

	unsigned char calcXOR(const unsigned char* pData, int iLength);


	bool  IsChinese(CString Cstr);

}
