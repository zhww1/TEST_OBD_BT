
/**
 ******************************************************************************
 * @file		Utilities.h
 * @author		Zhww
 * @version		V1.0
 * @date		三月 2018
 * @brief
 ******************************************************************************
 * @attention
 * 不应该使用难懂的技巧性很高的语句，除非很有必要时。
 * 高技巧的语句不等于高效率的程序，实际上程序的效率关键在于算法。
 *  如 
 * //*pu8Data++ +=1；
 * //*p ++pu8Data +=1;
 * //应该分别改为如下：
 * //*pu8Data +=1;    pu8Data++;// C此二语句共那个呢相当于"*pu8Data++ +1;"
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



	//封装了模块路径类 CPath
	class CPath
	{
	public:
		CPath(void);
		~CPath(void);

		static tstring GetAppPath();												// 获取应用程序执行路径
		static std::string GetAppPathAscii();										// 获取应用程序执行路径
		static tstring GetCurDir();													// 获取应用程序当前目录
		static tstring GetTempPath();												// 获取当前系统的临时文件夹的路径
		static tstring GetTempFileName(LPCTSTR lpszFileName);						// 获取当前系统的临时文件夹的路径下的唯一命名的临时文件名(全路径)
		static tstring GetRandomFileName(LPCTSTR lpszPath, LPCTSTR lpszFileName);	// 获取随机文件名(全路径)

		static BOOL IsDirectory(LPCTSTR lpszPath);									// 检测指定路径是否目录
		static BOOL IsFileExist(LPCTSTR lpszFileName);								// 检测指定文件是否存在
		static BOOL IsDirectoryExist(LPCTSTR lpszPath);								// 检测指定目录是否存在
		static BOOL CreateDirectory(LPCTSTR lpPathName, LPSECURITY_ATTRIBUTES lpSecurityAttributes = NULL);

		static tstring GetPathRoot(LPCTSTR lpszPath);								// 获取指定路径的根目录信息
		static tstring GetDirectoryName(LPCTSTR lpszPath);							// 返回指定路径字符串的目录信息
		static tstring GetFileName(LPCTSTR lpszPath);								// 返回指定路径字符串的文件名和扩展名
		static tstring GetFileNameWithoutExtension(LPCTSTR lpszPath);				// 返回不具有扩展名的路径字符串的文件名
		static tstring GetExtension(LPCTSTR lpszPath);								// 返回指定的路径字符串的扩展名
		static tstring GetFullPath(LPCTSTR lpszPath);								// 根据指定的相对路径获取绝对路径
		static long    GetFileSize(LPCTSTR lpszPath);								// 获取文件大小
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

	//注意：如果打印的日志信息中有中文，则格式化字符串要用_T()宏包裹起来，(...)可变参数
	//e.g. LOG_INFO(_T("GroupID=%u, GroupName=%s, GroupName=%s."), lpGroupInfo->m_nGroupCode, lpGroupInfo->m_strAccount.c_str(), lpGroupInfo->m_strName.c_str());
//不用一下三个宏 因为这三个宏打印的信息太全面了我们不需要
	// 不论用哪个宏都掉4参数的 函数.   (这3个点 ... 就是最后一个参数了__VA_ARGS__)
#define LOG_INFO(...)     Utilities::CIULog::Log(Utilities::LOG_LEVEL_INFO, __FUNCSIG__,__LINE__, __VA_ARGS__)//__FUNCSIG__ 编译器预定义 函数名字符串谁掉了__VA_ARGS__ 可变参数
#define LOG_WARNING(...)   Utilities::CIULog::Log(Utilities::LOG_LEVEL_WARNING, __FUNCSIG__, __LINE__,__VA_ARGS__)
#define LOG_ERROR(...)     Utilities::CIULog::Log(Utilities::LOG_LEVEL_ERROR, __FUNCSIG__,__LINE__, __VA_ARGS__)

	class  CIULog
	{
	public:
		static bool Init(bool bToFile, bool bTruncateLongLog, PCTSTR pszLogFileName);
		static void Uninit();

		static void SetLevel(LOG_LEVEL nLevel);

		//不输出线程ID号和所在函数签名、行号
		static bool Log(long nLevel, PCTSTR pszFmt, ...);
		//输出线程ID号和所在函数签名、行号
		//掉4参数 至少要给4个参数（而我们上面的宏，被个参数替换掉，也给了个参数 (...点会被最后一个替换) ） 这俩是重载 一个unicode PCTSTR pszFmt 版本一个char * PCSTR pszFmt版本  
		static bool Log(long nLevel, PCSTR pszFunctionSig, int nLineNo, PCTSTR pszFmt, ...);		//注意:pszFunctionSig参数为Ansic版本
		static bool Log(long nLevel, PCSTR pszFunctionSig, int nLineNo, PCSTR pszFmt, ...);
	private:
		CIULog() = delete;
		~CIULog() = delete;

		CIULog(const CIULog& rhs) = delete;
		CIULog& operator=(const CIULog& rhs) = delete;

		static void GetTime(char* pszTime, int nTimeStrLength);

	private:
		static bool		        m_bToFile;			    //日志写入文件还是写到控制台  
		static HANDLE	        m_hLogFile;
		static bool             m_bTruncateLongLog;     //长日志是否截断
		static LOG_LEVEL        m_nLogLevel;            //日志级别
	};



	/**
	* @class	EncodeConvert
	* @brief    不同编码之间的转换类 EncodeConvert
	* @author   Zhww
	*/

	class EncodeConvert
	{
	public:
		//===BEGIN: 注意：以下6个函数,需要在外部释放返回的字符串指针，否则会有内存泄露
		static wchar_t* EncodeConvert::AnsiToUnicode(const char* lpszStr);
		static char* UnicodeToAnsi(const wchar_t* lpszStr);
		static char* AnsiToUtf8(const char* lpszStr);
		static char* Utf8ToAnsi(const char* lpszStr);
		static char* UnicodeToUtf8(const wchar_t* lpszStr);
		static wchar_t* Utf8ToUnicode(const char* lpszStr);
		//===END: 注意：以下6个函数,需要在外部释放返回的字符串指针，否则会有内存泄露

		//===BEGIN: 以下函数第一个参数是需要转换的源字符串指针，第二个参数是存放转换后的目标缓冲区指针，第三个参数是目标缓冲区的大小
		static bool EncodeConvert::AnsiToUnicode(const char* lpszAnsi, wchar_t* lpszUnicode, int nLen);
		static bool UnicodeToAnsi(const wchar_t* lpszUnicode, char* lpszAnsi, int nLen);
		static bool AnsiToUtf8(const char* lpszAnsi, char* lpszUtf8, int nLen);
		static bool Utf8ToAnsi(const char* lpszUtf8, char* lpszAnsi, int nLen);
		static bool UnicodeToUtf8(const wchar_t* lpszUnicode, char* lpszUtf8, int nLen);
		static bool Utf8ToUnicode(const char* lpszUtf8, wchar_t* lpszUnicode, int nLen);
		//===END: 以下函数第一个参数是需要转换的源字符串指针，第二个参数是存放转换后的目标缓冲区指针，第三个参数是目标缓冲区的大小

		static std::wstring EncodeConvert::AnsiToUnicode(const std::string& strAnsi);
		static std::string UnicodeToAnsi(const std::wstring& strUnicode);
		static std::string AnsiToUtf8(const std::string& strAnsi);
		static std::string Utf8ToAnsi(const std::string& strUtf8);
		static std::string UnicodeToUtf8(const std::wstring& strUnicode);
		static std::wstring Utf8ToUnicode(const std::string& strUtf8);

	private:
		EncodeConvert() = delete; //删除构造函数无法实例化
		~EncodeConvert() = delete;

		EncodeConvert(const EncodeConvert& rhs) = delete;//C++ 11的用法禁止 拷贝构造
		EncodeConvert& operator=(const EncodeConvert& rhs) = delete;//禁止赋值等号操作
	};
	std::wstring UnicodeToHexStr(const WCHAR* lpStr, BOOL bDblSlash);
	//重载2个函数  char  与wchar
	BOOL my_isalnum(unsigned char cChar);		// 检测指定字符是否是字母(A-Z，a-z)或数字(0-9)
	BOOL my_isalnum(wchar_t cChar);				// 检测指定字符是否是字母(A-Z，a-z)或数字(0-9)


	BYTE ASC2BYTE(BYTE asc1, BYTE asc2);
	INT16U CRC16(unsigned char* pushMsg, unsigned short usDataLen,unsigned char mode);
	//只有低8可用。return check & 0x00ff 
	//INT16U Checksum_XOR(unsigned char* buf, INT16U len);	//异或校验，所有字节异或

	unsigned char calcXOR(const unsigned char* pData, int iLength);


	bool  IsChinese(CString Cstr);

}
