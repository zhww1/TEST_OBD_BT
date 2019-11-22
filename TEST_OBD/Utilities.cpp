#include "stdafx.h"
#include "Utilities.h"

namespace Utilities{


	CPath::CPath(void)
	{
	}
	CPath::~CPath(void)
	{
	}
	// 获取应用程序执行路径
	tstring CPath::GetAppPath()
	{
		TCHAR szPath[MAX_PATH] = { 0 };
		::GetModuleFileName(NULL, szPath, MAX_PATH);

		tstring strPath(szPath);
		for (int nPos = (int)strPath.size() - 1; nPos >= 0; --nPos)
		{
			TCHAR cChar = strPath[nPos];
			if (_T('\\') == cChar || _T('/') == cChar)
				return strPath.substr(0, nPos + 1);
		}
		return strPath;
	}

	std::string CPath::GetAppPathAscii()
	{
		char szPath[MAX_PATH] = { 0 };
		::GetModuleFileNameA(NULL, szPath, MAX_PATH);

		std::string strPath(szPath);
		for (int nPos = (int)strPath.size() - 1; nPos >= 0; --nPos)
		{
			char cChar = strPath[nPos];
			if ('\\' == cChar || '/' == cChar)
				return strPath.substr(0, nPos + 1);
		}
		return strPath;
	}

	// 获取应用程序当前目录
	tstring CPath::GetCurDir()
	{
		TCHAR szCurDir[MAX_PATH] = { 0 };
		::GetCurrentDirectory(MAX_PATH, szCurDir);

		DWORD dwLen = _tcslen(szCurDir);
		if (dwLen <= 0)
			return _T("");

		TCHAR cLastChar = szCurDir[dwLen - 1];
		if (cLastChar != _T('\\') && cLastChar != _T('/'))
			_tcscat(szCurDir, _T("\\"));

		return szCurDir;
	}

	// 获取当前系统的临时文件夹的路径
	tstring CPath::GetTempPath()
	{
		TCHAR szTempPath[MAX_PATH] = { 0 };
		::GetTempPath(MAX_PATH, szTempPath);

		DWORD dwLen = _tcslen(szTempPath);
		if (dwLen <= 0)
			return _T("");

		TCHAR cLastChar = szTempPath[dwLen - 1];
		if (cLastChar != _T('\\') && cLastChar != _T('/'))
			_tcscat(szTempPath, _T("\\"));

		return szTempPath;
	}

	// 获取当前系统的临时文件夹的路径下的唯一命名的临时文件名(全路径)
	tstring CPath::GetTempFileName(LPCTSTR lpszFileName)
	{
		return GetRandomFileName(GetTempPath().c_str(), lpszFileName);
	}

	// 获取随机文件名(全路径)
	tstring CPath::GetRandomFileName(LPCTSTR lpszPath, LPCTSTR lpszFileName)
	{
		tstring strPath, strFileName, strExtFileName, strFullPath;
		TCHAR szBuf[MAX_PATH] = { 0 };

		if (!IsDirectoryExist(lpszPath))
			strPath = GetAppPath();
		else
			strPath = lpszPath;

		strFileName = GetFileNameWithoutExtension(lpszFileName);
		strExtFileName = GetExtension(lpszFileName);

		for (int i = 2; i < 10000; i++)
		{
			if (strExtFileName.empty())
			{
				strFullPath = strPath;
				strFullPath += strFileName;
				wsprintf(szBuf, _T("%d"), i);
				strFullPath += szBuf;
			}
			else
			{
				strFullPath = strPath;
				strFullPath += strFileName;
				wsprintf(szBuf, _T("%d."), i);
				strFullPath += szBuf;
				strFullPath += strExtFileName;
			}

			if (!IsFileExist(strFullPath.c_str()))
				return strFullPath;
		}

		return _T("");
	}

	// 检测指定路径是否目录
	BOOL CPath::IsDirectory(LPCTSTR lpszPath)
	{
		if (NULL == lpszPath || NULL == *lpszPath)
			return FALSE;

		DWORD dwAttr = ::GetFileAttributes(lpszPath);
		return (((dwAttr != 0xFFFFFFFF) && (dwAttr & FILE_ATTRIBUTE_DIRECTORY)) ? TRUE : FALSE);
	}

	// 检测指定文件是否存在
	BOOL CPath::IsFileExist(LPCTSTR lpszFileName)
	{
		if (NULL == lpszFileName || NULL == *lpszFileName)
			return FALSE;

		DWORD dwAttr = ::GetFileAttributes(lpszFileName);
		return (((dwAttr != 0xFFFFFFFF) && (!(dwAttr & FILE_ATTRIBUTE_DIRECTORY))) ? TRUE : FALSE);
	}

	// 检测指定目录是否存在
	BOOL CPath::IsDirectoryExist(LPCTSTR lpszPath)
	{
		if (NULL == lpszPath || NULL == *lpszPath)
			return FALSE;

		DWORD dwAttr = ::GetFileAttributes(lpszPath);
		return (((dwAttr != 0xFFFFFFFF) && (dwAttr & FILE_ATTRIBUTE_DIRECTORY)) ? TRUE : FALSE);
	}

	BOOL CPath::CreateDirectory(LPCTSTR lpPathName, LPSECURITY_ATTRIBUTES lpSecurityAttributes)
	{
		TCHAR cPath[MAX_PATH] = { 0 };
		TCHAR cTmpPath[MAX_PATH] = { 0 };
		TCHAR* lpPos = NULL;
		TCHAR cTmp = _T('\0');

		if (NULL == lpPathName || NULL == *lpPathName)
			return FALSE;

		_tcsncpy(cPath, lpPathName, MAX_PATH);

		for (int i = 0; i < (int)_tcslen(cPath); i++)
		{
			if (_T('\\') == cPath[i])
				cPath[i] = _T('/');
		}

		lpPos = _tcschr(cPath, _T('/'));
		while (lpPos != NULL)
		{
			if (lpPos == cPath)
			{
				lpPos++;
			}
			else
			{
				cTmp = *lpPos;
				*lpPos = _T('\0');
				_tcsncpy(cTmpPath, cPath, MAX_PATH);
				int res =::CreateDirectory(cTmpPath, lpSecurityAttributes);
				*lpPos = cTmp;
				lpPos++;
			}
			lpPos = _tcschr(lpPos, _T('/'));
		}

		return TRUE;
	}

	// 获取指定路径的根目录信息
	tstring CPath::GetPathRoot(LPCTSTR lpszPath)
	{
		if (NULL == lpszPath || NULL == *lpszPath)
			return _T("");

		tstring strPath(lpszPath);

		tstring::iterator iter;
		for (iter = strPath.begin(); iter < strPath.end(); iter++)
		{
			if (_T('\\') == *iter)
				*iter = _T('/');
		}

		tstring::size_type nPos = strPath.find(_T('/'));
		if (nPos != tstring::npos)
			strPath = strPath.substr(0, nPos + 1);

		return strPath;
	}

	// 返回指定路径字符串的目录信息
	tstring CPath::GetDirectoryName(LPCTSTR lpszPath)
	{
		if (NULL == lpszPath || NULL == *lpszPath)
			return _T("");

		tstring strPath(lpszPath);
		for (int nPos = (int)strPath.size() - 1; nPos >= 0; --nPos)
		{
			TCHAR cChar = strPath[nPos];
			if (_T('\\') == cChar || _T('/') == cChar)
				return strPath.substr(0, nPos + 1);
		}
		return strPath;
	}

	// 返回指定路径字符串的文件名和扩展名
	tstring CPath::GetFileName(LPCTSTR lpszPath)
	{
		if (NULL == lpszPath || NULL == *lpszPath)
			return _T("");

		tstring strPath(lpszPath);
		for (int nPos = (int)strPath.size() - 1; nPos >= 0; --nPos)
		{
			TCHAR cChar = strPath[nPos];
			if (_T('\\') == cChar || _T('/') == cChar)
				return strPath.substr(nPos + 1);
		}
		return strPath;
	}

	// 返回不具有扩展名的路径字符串的文件名
	tstring CPath::GetFileNameWithoutExtension(LPCTSTR lpszPath)
	{
		if (NULL == lpszPath || NULL == *lpszPath)
			return _T("");

		tstring strPath(lpszPath);
		for (int nPos = (int)strPath.size() - 1; nPos >= 0; --nPos)
		{
			TCHAR cChar = strPath[nPos];
			if (_T('\\') == cChar || _T('/') == cChar)
			{
				strPath = strPath.substr(nPos + 1);
				break;
			}
		}

		int nPos = strPath.rfind(_T('.'));
		if (nPos != tstring::npos)
			strPath = strPath.substr(0, nPos);

		return strPath;
	}

	// 返回指定的路径字符串的扩展名
	tstring CPath::GetExtension(LPCTSTR lpszPath)
	{
		if (NULL == lpszPath || NULL == *lpszPath)
			return _T("");

		tstring strPath(lpszPath);

		tstring::size_type nPos = strPath.rfind(_T('.'));
		if (nPos != tstring::npos)
			return strPath.substr(nPos + 1);
		else
			return _T("");
	}

	// 根据指定的相对路径获取绝对路径
	tstring CPath::GetFullPath(LPCTSTR lpszPath)
	{
		if (NULL == lpszPath || NULL == *lpszPath)
			return _T("");

		tstring strPath(lpszPath);

		TCHAR cFirstChar = strPath.at(0);
		TCHAR cSecondChar = strPath.at(1);

		if (cFirstChar == _T('\\') || cFirstChar == _T('/'))
		{
			tstring strCurDir = GetAppPath();
			tstring strRootPath = GetPathRoot(strCurDir.c_str());
			return strRootPath + strPath;
		}
		else if (::IsCharAlpha(cFirstChar) && cSecondChar == _T(':'))
		{
			return strPath;
		}
		else
		{
			tstring strCurDir = GetAppPath();
			return strCurDir + strPath;
		}
	}

	long CPath::GetFileSize(LPCTSTR lpszPath)
	{
		if (lpszPath == NULL)
			return 0;

		WIN32_FIND_DATA fd = { 0 };

		HANDLE hFindFile = ::FindFirstFile(lpszPath, &fd);
		if (hFindFile == INVALID_HANDLE_VALUE)
			return 0;

		//TODO: 不考虑大于4G以上的文件
		long nFileSize = (long)fd.nFileSizeLow;
		::FindClose(hFindFile);

		return nFileSize;
	}




	CIniFile::CIniFile()
	{

	}

	CIniFile::~CIniFile()
	{

	}

	UINT CIniFile::ReadInt(LPCTSTR lpAppName, LPCTSTR lpKeyName, long nDefault, LPCTSTR lpFileName)
	{
		return ::GetPrivateProfileInt(lpAppName, lpKeyName, nDefault, lpFileName);
	}

	BOOL CIniFile::ReadString(LPCTSTR lpAppName, LPCTSTR lpKeyName, LPCTSTR lpDefault, LPTSTR lpReturnedString, DWORD nSize, LPCTSTR lpFileName)
	{
		::GetPrivateProfileString(lpAppName, lpKeyName, lpDefault, lpReturnedString, nSize, lpFileName);
		return TRUE;
	}

	BOOL CIniFile::WriteInt(LPCTSTR lpAppName, LPCTSTR lpKeyName, long nValue, LPCTSTR lpFileName)
	{
		TCHAR szValue[32] = { 0 };

		_ltow(nValue, szValue, 10);
		return ::WritePrivateProfileString(lpAppName, lpKeyName, szValue, lpFileName);
	}

	BOOL CIniFile::WriteString(LPCTSTR lpAppName, LPCTSTR lpKeyName, LPCTSTR lpString, LPCTSTR lpFileName)
	{
		return ::WritePrivateProfileString(lpAppName, lpKeyName, lpString, lpFileName);
	}




#ifndef LOG_OUTPUT
#define LOG_OUTPUT
#endif

#define MAX_LINE_LENGTH 256

	bool CIULog::m_bToFile = false;

	bool CIULog::m_bTruncateLongLog = false;
	HANDLE CIULog::m_hLogFile = INVALID_HANDLE_VALUE;
	LOG_LEVEL CIULog::m_nLogLevel = LOG_LEVEL_INFO;

	bool CIULog::Init(bool bToFile, bool bTruncateLongLog, PCTSTR pszLogFileName)
	{
#ifdef LOG_OUTPUT
		m_bToFile = bToFile;
		m_bTruncateLongLog = bTruncateLongLog;

		if (pszLogFileName == NULL || pszLogFileName[0] == NULL)
			return FALSE;

		TCHAR szHomePath[MAX_PATH] = { 0 };
		::GetModuleFileName(NULL, szHomePath, MAX_PATH);
		for (int i = _tcslen(szHomePath); i >= 0; --i)
		{
			if (szHomePath[i] == _T('\\'))
			{
				szHomePath[i] = _T('\0');
				break;
			}
		}

		TCHAR szLogDirectory[MAX_PATH] = { 0 };
		_stprintf_s(szLogDirectory, _T("%s\\Logs\\"), szHomePath);

		DWORD dwAttr = ::GetFileAttributes(szLogDirectory);
		if (!((dwAttr != 0xFFFFFFFF) && (dwAttr & FILE_ATTRIBUTE_DIRECTORY)))
		{
			TCHAR cPath[MAX_PATH] = { 0 };
			TCHAR cTmpPath[MAX_PATH] = { 0 };
			TCHAR* lpPos = NULL;
			TCHAR cTmp = _T('\0');

			_tcsncpy_s(cPath, szLogDirectory, MAX_PATH);

			for (int i = 0; i < (int)_tcslen(cPath); i++)
			{
				if (_T('\\') == cPath[i])
					cPath[i] = _T('/');
			}

			lpPos = _tcschr(cPath, _T('/'));
			while (lpPos != NULL)
			{
				if (lpPos == cPath)
				{
					lpPos++;
				}
				else
				{
					cTmp = *lpPos;
					*lpPos = _T('\0');
					_tcsncpy_s(cTmpPath, cPath, MAX_PATH);
					::CreateDirectory(cTmpPath, NULL);
					*lpPos = cTmp;
					lpPos++;
				}
				lpPos = _tcschr(lpPos, _T('/'));
			}
		}

		//m_hLogFile = ::CreateFile(pszLogFileName, GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ, NULL, CREATE_NEW, FILE_ATTRIBUTE_NORMAL, NULL);
		m_hLogFile = ::CreateFile(pszLogFileName, GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ, NULL, OPEN_ALWAYS, NULL, NULL);
		if (m_hLogFile == INVALID_HANDLE_VALUE)
			return false;

#endif // end LOG_OUTPUT

		return true;
	}

	void CIULog::Uninit()
	{
#ifdef LOG_OUTPUT
		if (m_hLogFile != INVALID_HANDLE_VALUE)
		{
			::CloseHandle(m_hLogFile);
			m_hLogFile = INVALID_HANDLE_VALUE;
		}
#endif //end LOG_OUTPUT
	}

	void CIULog::SetLevel(LOG_LEVEL nLevel)
	{
		m_nLogLevel = nLevel;
	}

	bool CIULog::Log(long nLevel, PCTSTR pszFmt, ...)
	{
#ifdef LOG_OUTPUT
		if (nLevel < m_nLogLevel)
			return false;

		char szTime[64] = { 0 };
		GetTime(szTime, ARRAYSIZE(szTime));
		std::string strDebugInfo(szTime);

		std::string strLevel("[INFO]");
		if (nLevel == LOG_LEVEL_WARNING)
			strLevel = "[Warning]";
		else if (nLevel == LOG_LEVEL_ERROR)
			strLevel = "[Error]";

		strDebugInfo += strLevel;

		//当前线程信息
		char szThreadID[32] = { 0 };
		DWORD dwThreadID = ::GetCurrentThreadId();
		//将[ThreadID: %u] 放到数组中
		sprintf_s(szThreadID, ARRAYSIZE(szThreadID), "[ThreadID: %u]", dwThreadID);//%u 无符号十进制数字
		strDebugInfo += szThreadID;

		//log正文
		std::wstring strLogMsg;
		va_list ap;
		va_start(ap, pszFmt);
		int nLogMsgLength = _vsctprintf(pszFmt, ap);
		//容量必须算上最后一个\0
		if ((int)strLogMsg.capacity() < nLogMsgLength + 1)
		{
			strLogMsg.resize(nLogMsgLength + 1);
		}
		_vstprintf_s((TCHAR*)strLogMsg.data(), strLogMsg.capacity(), pszFmt, ap);
		va_end(ap);

		//string内容正确但length不对，恢复一下其length
		std::wstring strMsgFormal;
		strMsgFormal.append(strLogMsg.c_str(), nLogMsgLength);

		//如果日志开启截断，长日志只取前MAX_LINE_LENGTH个字符
		if (m_bTruncateLongLog)
			strMsgFormal = strMsgFormal.substr(0, MAX_LINE_LENGTH);

		std::string strLogMsgAscii;
		strLogMsgAscii = Utilities::EncodeConvert::UnicodeToAnsi(strMsgFormal);

		strDebugInfo += strLogMsgAscii;
		strDebugInfo += "\r\n";

		if (m_bToFile)
		{
			if (m_hLogFile == INVALID_HANDLE_VALUE)
				return false;

			::SetFilePointer(m_hLogFile, 0, NULL, FILE_END);
			DWORD dwBytesWritten = 0;
			::WriteFile(m_hLogFile, strDebugInfo.c_str(), strDebugInfo.length(), &dwBytesWritten, NULL);
			::FlushFileBuffers(m_hLogFile);
			return true;
		}

		::OutputDebugStringA(strDebugInfo.c_str());

#endif // end LOG_OUTPUT

		return true;
	}

	bool CIULog::Log(long nLevel, PCSTR pszFunctionSig, int nLineNo, PCTSTR pszFmt, ...)
	{
#ifdef LOG_OUTPUT
		if (nLevel < m_nLogLevel)
			return false;

		//时间
		char szTime[64] = { 0 };
		GetTime(szTime, ARRAYSIZE(szTime));
		std::string strDebugInfo(szTime);

		//错误级别
		std::string strLevel("[INFO]");
		if (nLevel == LOG_LEVEL_WARNING)
			strLevel = "[Warning]";
		else if (nLevel == LOG_LEVEL_ERROR)
			strLevel = "[Error]";

		strDebugInfo += strLevel;

		//当前线程信息
		char szThreadID[32] = { 0 };
		DWORD dwThreadID = ::GetCurrentThreadId();
		sprintf_s(szThreadID, ARRAYSIZE(szThreadID), "[ThreadID: %u]", dwThreadID);
		strDebugInfo += szThreadID;

		//函数签名
		char szFuncSig[512] = { 0 };
		sprintf_s(szFuncSig, "[%s:%d]", pszFunctionSig, nLineNo);
		strDebugInfo += szFuncSig;

		//log正文
		std::wstring strLogMsg;
		va_list ap;
		va_start(ap, pszFmt);
		int nLogMsgLength = _vsctprintf(pszFmt, ap);
		//容量必须算上最后一个\0 
		if ((int)strLogMsg.capacity() < nLogMsgLength + 1)
		{
			strLogMsg.resize(nLogMsgLength + 1);
		}
		_vstprintf_s((TCHAR*)strLogMsg.data(), strLogMsg.capacity(), pszFmt, ap);
		va_end(ap);

		//string内容正确但length不对，恢复一下其length
		std::wstring strMsgFormal;
		strMsgFormal.append(strLogMsg.c_str(), nLogMsgLength);

		//如果日志开启截断，长日志只取前MAX_LINE_LENGTH个字符
		if (m_bTruncateLongLog)
			strMsgFormal = strMsgFormal.substr(0, MAX_LINE_LENGTH);

		std::string strLogMsgAscii;
		strLogMsgAscii = Utilities::EncodeConvert::UnicodeToAnsi(strMsgFormal);

		strDebugInfo += strLogMsgAscii;
		strDebugInfo += "\r\n";

		if (m_bToFile)
		{
			if (m_hLogFile == INVALID_HANDLE_VALUE)
				return false;

			::SetFilePointer(m_hLogFile, 0, NULL, FILE_END);
			DWORD dwBytesWritten = 0;
			::WriteFile(m_hLogFile, strDebugInfo.c_str(), strDebugInfo.length(), &dwBytesWritten, NULL);
			::FlushFileBuffers(m_hLogFile);
			return true;
		}

		::OutputDebugStringA(strDebugInfo.c_str());

#endif // end LOG_OUTPUT

		return true;
	}

//	bool CIULog::Log(long nLevel, PCSTR pszFunctionSig, int nLineNo, PCSTR pszFmt, ...)
//	{
//#ifdef LOG_OUTPUT
//		if (nLevel < m_nLogLevel)
//			return false;
//
//		//时间
//		char szTime[64] = { 0 };
//		GetTime(szTime, ARRAYSIZE(szTime));
//		std::string strDebugInfo(szTime);
//
//		//错误级别
//		std::string strLevel("[INFO]");
//		if (nLevel == LOG_LEVEL_WARNING)
//			strLevel = "[Warning]";
//		else if (nLevel == LOG_LEVEL_ERROR)
//			strLevel = "[Error]";
//
//		strDebugInfo += strLevel;
//
//		//当前线程信息
//		char szThreadID[32] = { 0 };
//		DWORD dwThreadID = ::GetCurrentThreadId();
//		sprintf_s(szThreadID, ARRAYSIZE(szThreadID), "[ThreadID: %u]", dwThreadID);
//		strDebugInfo += szThreadID;
//
//		//函数签名
//		char szFuncSig[512] = { 0 };
//		sprintf_s(szFuncSig, "[%s:%d]", pszFunctionSig, nLineNo);
//		strDebugInfo += szFuncSig;
//
//		//日志正文
//		std::string strLogMsg;
//		va_list ap;
//		va_start(ap, pszFmt);
//		int nLogMsgLength = _vscprintf(pszFmt, ap);
//		//容量必须算上最后一个\0
//		if ((int)strLogMsg.capacity() < nLogMsgLength + 1)
//		{
//			strLogMsg.resize(nLogMsgLength + 1);
//		}
//		vsprintf_s((char*)strLogMsg.data(), strLogMsg.capacity(), pszFmt, ap);
//		va_end(ap);
//
//		//string内容正确但length不对，恢复一下其length
//		std::string strMsgFormal;
//		strMsgFormal.append(strLogMsg.c_str(), nLogMsgLength);
//
//		//如果日志开启截断，长日志只取前MAX_LINE_LENGTH个字符
//		if (m_bTruncateLongLog)
//			strMsgFormal = strMsgFormal.substr(0, MAX_LINE_LENGTH);
//
//		strDebugInfo += strMsgFormal;
//		strDebugInfo += "\r\n";
//
//		if (m_bToFile)
//		{
//			if (m_hLogFile == INVALID_HANDLE_VALUE)
//				return false;
//
//			::SetFilePointer(m_hLogFile, 0, NULL, FILE_END);
//			DWORD dwBytesWritten = 0;
//			::WriteFile(m_hLogFile, strDebugInfo.c_str(), strDebugInfo.length(), &dwBytesWritten, NULL);
//			::FlushFileBuffers(m_hLogFile);
//			return true;
//		}
//
//		::OutputDebugStringA(strDebugInfo.c_str());
//
//#endif // end LOG_OUTPUT
//
//		return true;
//	}

	void CIULog::GetTime(char* pszTime, int nTimeStrLength)
	{
		SYSTEMTIME st = { 0 };
		::GetLocalTime(&st);
		sprintf_s(pszTime, nTimeStrLength, "[%04d-%02d-%02d %02d:%02d:%02d:%04d]", st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond, st.wMilliseconds);
	}



	wchar_t* EncodeConvert::AnsiToUnicode(const char* lpszStr)
	{
		wchar_t* lpUnicode;
		int nLen;

		if (NULL == lpszStr)
			return NULL;

		nLen = ::MultiByteToWideChar(CP_ACP, 0, lpszStr, -1, NULL, 0);
		if (0 == nLen)
			return NULL;

		lpUnicode = new wchar_t[nLen + 1];
		if (NULL == lpUnicode)
			return NULL;

		memset(lpUnicode, 0, sizeof(wchar_t)* (nLen + 1));
		nLen = ::MultiByteToWideChar(CP_ACP, 0, lpszStr, -1, lpUnicode, nLen);
		if (0 == nLen)
		{
			delete[]lpUnicode;
			return NULL;
		}

		return lpUnicode;
	}

	char* EncodeConvert::UnicodeToAnsi(const wchar_t* lpszStr)

	{
		char* lpAnsi;
		int nLen;

		if (NULL == lpszStr)
			return NULL;

		nLen = ::WideCharToMultiByte(CP_ACP, 0, lpszStr, -1, NULL, 0, NULL, NULL);
		if (0 == nLen)
			return NULL;

		lpAnsi = new char[nLen + 1];
		if (NULL == lpAnsi)
			return NULL;

		memset(lpAnsi, 0, nLen + 1);
		nLen = ::WideCharToMultiByte(CP_ACP, 0, lpszStr, -1, lpAnsi, nLen, NULL, NULL);
		if (0 == nLen)
		{
			delete[]lpAnsi;
			return NULL;
		}

		return lpAnsi;
	}

	char* EncodeConvert::AnsiToUtf8(const char* lpszStr)

	{
		wchar_t* lpUnicode;
		char* lpUtf8;
		int nLen;

		if (NULL == lpszStr)
			return NULL;

		nLen = ::MultiByteToWideChar(CP_ACP, 0, lpszStr, -1, NULL, NULL);
		if (0 == nLen)
			return NULL;

		lpUnicode = new wchar_t[nLen + 1];
		if (NULL == lpUnicode)
			return NULL;

		memset(lpUnicode, 0, sizeof(wchar_t)* (nLen + 1));
		nLen = ::MultiByteToWideChar(CP_ACP, 0, lpszStr, -1, lpUnicode, nLen);
		if (0 == nLen)
		{
			delete[]lpUnicode;
			return NULL;
		}

		nLen = ::WideCharToMultiByte(CP_UTF8, 0, lpUnicode, -1, NULL, 0, NULL, NULL);
		if (0 == nLen)
		{
			delete[]lpUnicode;
			return NULL;
		}

		lpUtf8 = new char[nLen + 1];
		if (NULL == lpUtf8)
		{
			delete[]lpUnicode;
			return NULL;
		}

		memset(lpUtf8, 0, nLen + 1);
		nLen = ::WideCharToMultiByte(CP_UTF8, 0, lpUnicode, -1, lpUtf8, nLen, NULL, NULL);
		if (0 == nLen)
		{
			delete[]lpUnicode;
			delete[]lpUtf8;
			return NULL;
		}

		delete[]lpUnicode;

		return lpUtf8;
	}



	char* EncodeConvert::Utf8ToAnsi(const char* lpszStr)

	{
		wchar_t* lpUnicode;
		char* lpAnsi;
		int nLen;

		if (NULL == lpszStr)
			return NULL;

		nLen = ::MultiByteToWideChar(CP_UTF8, 0, lpszStr, -1, NULL, NULL);
		if (0 == nLen)
			return NULL;

		lpUnicode = new wchar_t[nLen + 1];
		if (NULL == lpUnicode)
			return NULL;

		memset(lpUnicode, 0, sizeof(wchar_t)* (nLen + 1));
		nLen = ::MultiByteToWideChar(CP_UTF8, 0, lpszStr, -1, lpUnicode, nLen);
		if (0 == nLen)
		{
			delete[]lpUnicode;
			return NULL;
		}

		nLen = ::WideCharToMultiByte(CP_ACP, 0, lpUnicode, -1, NULL, 0, NULL, NULL);
		if (0 == nLen)
		{
			delete[]lpUnicode;
			return NULL;
		}

		lpAnsi = new char[nLen + 1];
		if (NULL == lpAnsi)
		{
			delete[]lpUnicode;
			return NULL;
		}

		memset(lpAnsi, 0, nLen + 1);
		nLen = ::WideCharToMultiByte(CP_ACP, 0, lpUnicode, -1, lpAnsi, nLen, NULL, NULL);
		if (0 == nLen)
		{
			delete[]lpUnicode;
			delete[]lpAnsi;
			return NULL;
		}

		delete[]lpUnicode;

		return lpAnsi;
	}

	char* EncodeConvert::UnicodeToUtf8(const wchar_t* lpszStr)

	{
		char* lpUtf8;
		int nLen;

		if (NULL == lpszStr)
			return NULL;

		nLen = ::WideCharToMultiByte(CP_UTF8, 0, lpszStr, -1, NULL, 0, NULL, NULL);
		if (0 == nLen)
			return NULL;

		lpUtf8 = new char[nLen + 1];
		if (NULL == lpUtf8)
			return NULL;

		memset(lpUtf8, 0, nLen + 1);
		nLen = ::WideCharToMultiByte(CP_UTF8, 0, lpszStr, -1, lpUtf8, nLen, NULL, NULL);
		if (0 == nLen)
		{
			delete[]lpUtf8;
			return NULL;
		}

		return lpUtf8;
	}



	wchar_t* EncodeConvert::Utf8ToUnicode(const char* lpszStr)

	{
		wchar_t* lpUnicode;
		int nLen;

		if (NULL == lpszStr)
			return NULL;

		nLen = ::MultiByteToWideChar(CP_UTF8, 0, lpszStr, -1, NULL, 0);
		if (0 == nLen)
			return NULL;

		lpUnicode = new wchar_t[nLen + 1];
		if (NULL == lpUnicode)
			return NULL;

		memset(lpUnicode, 0, sizeof(wchar_t)* (nLen + 1));
		nLen = ::MultiByteToWideChar(CP_UTF8, 0, lpszStr, -1, lpUnicode, nLen);
		if (0 == nLen)
		{
			delete[]lpUnicode;
			return NULL;
		}

		return lpUnicode;
	}

	bool EncodeConvert::AnsiToUnicode(const char* lpszAnsi, wchar_t* lpszUnicode, int nLen)
	{

		int nRet = ::MultiByteToWideChar(CP_ACP, 0, lpszAnsi, -1, lpszUnicode, nLen);
		return (0 == nRet) ? FALSE : TRUE;
	}



	bool EncodeConvert::UnicodeToAnsi(const wchar_t* lpszUnicode, char* lpszAnsi, int nLen)
	{
		int nRet = ::WideCharToMultiByte(CP_ACP, 0, lpszUnicode, -1, lpszAnsi, nLen, NULL, NULL);
		return (0 == nRet) ? FALSE : TRUE;
	}

	bool EncodeConvert::AnsiToUtf8(const char* lpszAnsi, char* lpszUtf8, int nLen)
	{
		wchar_t* lpszUnicode = EncodeConvert::AnsiToUnicode(lpszAnsi);
		if (NULL == lpszUnicode)
			return FALSE;

		int nRet = EncodeConvert::UnicodeToUtf8(lpszUnicode, lpszUtf8, nLen);

		delete[]lpszUnicode;

		return (0 == nRet) ? FALSE : TRUE;
	}


	bool EncodeConvert::Utf8ToAnsi(const char* lpszUtf8, char* lpszAnsi, int nLen)

	{
		wchar_t* lpszUnicode = EncodeConvert::Utf8ToUnicode(lpszUtf8);
		if (NULL == lpszUnicode)
			return FALSE;

		int nRet = UnicodeToAnsi(lpszUnicode, lpszAnsi, nLen);

		delete[]lpszUnicode;

		return (0 == nRet) ? FALSE : TRUE;
	}



	bool EncodeConvert::UnicodeToUtf8(const wchar_t* lpszUnicode, char* lpszUtf8, int nLen)
	{
		int nRet = ::WideCharToMultiByte(CP_UTF8, 0, lpszUnicode, -1, lpszUtf8, nLen, NULL, NULL);
		return (0 == nRet) ? FALSE : TRUE;
	}

	bool EncodeConvert::Utf8ToUnicode(const char* lpszUtf8, wchar_t* lpszUnicode, int nLen)
	{
		int nRet = ::MultiByteToWideChar(CP_UTF8, 0, lpszUtf8, -1, lpszUnicode, nLen);
		return (0 == nRet) ? FALSE : TRUE;
	}


	std::wstring EncodeConvert::AnsiToUnicode(const std::string& strAnsi)
	{
		std::wstring strUnicode;

		wchar_t* lpszUnicode = EncodeConvert::AnsiToUnicode(strAnsi.c_str());
		if (lpszUnicode != NULL)
		{
			strUnicode = lpszUnicode;
			delete[]lpszUnicode;
		}

		return strUnicode;
	}

	std::string EncodeConvert::UnicodeToAnsi(const std::wstring& strUnicode)
	{
		std::string strAnsi;

		char* lpszAnsi = UnicodeToAnsi(strUnicode.c_str());
		if (lpszAnsi != NULL)
		{
			strAnsi = lpszAnsi;
			delete[]lpszAnsi;
		}

		return strAnsi;
	}

	std::string EncodeConvert::AnsiToUtf8(const std::string& strAnsi)
	{
		std::string strUtf8;

		char* lpszUtf8 = AnsiToUtf8(strAnsi.c_str());
		if (lpszUtf8 != NULL)
		{
			strUtf8 = lpszUtf8;
			delete[]lpszUtf8;
		}

		return strUtf8;
	}
	std::string EncodeConvert::Utf8ToAnsi(const std::string& strUtf8)
	{
		std::string strAnsi;

		char* lpszAnsi = Utf8ToAnsi(strUtf8.c_str());
		if (lpszAnsi != NULL)
		{
			strAnsi = lpszAnsi;
			delete[]lpszAnsi;
		}

		return strAnsi;
	}



	std::string EncodeConvert::UnicodeToUtf8(const std::wstring& strUnicode)
	{
		std::string strUtf8;

		char* lpszUtf8 = EncodeConvert::UnicodeToUtf8(strUnicode.c_str());
		if (lpszUtf8 != NULL)
		{
			strUtf8 = lpszUtf8;
			delete[]lpszUtf8;
		}

		return strUtf8;
	}

	std::wstring EncodeConvert::Utf8ToUnicode(const std::string& strUtf8)
	{
		std::wstring strUnicode;

		wchar_t* lpszUnicode = EncodeConvert::Utf8ToUnicode(strUtf8.c_str());
		if (lpszUnicode != NULL)
		{
			strUnicode = lpszUnicode;
			delete[]lpszUnicode;
		}

		return strUnicode;
	}

	std::wstring UnicodeToHexStr(const WCHAR* lpStr, BOOL bDblSlash)
	{
		static const WCHAR* lpHexStr = _T("0123456789abcdef");
		std::wstring strRet = _T("");
		WCHAR* lpSlash;
		if (NULL == lpStr || NULL == *lpStr)
			return strRet;

		lpSlash = (bDblSlash ? _T("\\\\u") : _T("\\u"));

		for (int i = 0; i < (int)wcslen(lpStr); i++)
		{
			if (my_isalnum((WCHAR)lpStr[i]))	// 检测指定字符是否是字母(A-Z，a-z)或数字(0-9)
			{
				strRet += lpStr[i];
			}
			else
			{
				CHAR* lpChar = (CHAR*)&lpStr[i];

				strRet += lpSlash;
				strRet += lpHexStr[((unsigned char)(*(lpChar + 1)) >> 4) & 0x0f];
				strRet += lpHexStr[(unsigned char)(*(lpChar + 1)) & 0x0f];
				strRet += lpHexStr[((unsigned char)(*lpChar) >> 4) & 0x0f];
				strRet += lpHexStr[(unsigned char)(*lpChar) & 0x0f];
			}
		}
		return strRet;
	}


	BOOL my_isalnum(wchar_t cChar)
	{

		return ((cChar | 0x20) - L'a') < 26u || (cChar - L'0') < 10u;

	}

	BOOL my_isalnum(unsigned char cChar)
	{
		return ((cChar | 0x20) - 'a') < 26u || (cChar - '0') < 10u;
	}



	BYTE ASC2BYTE(BYTE asc1, BYTE asc2)
	{
		if (asc1 >= '0'&&asc1 <= '9')
			asc1 = (asc1 - '0');
		else if (asc1 >= 'a'&&asc1 <= 'f')
			asc1 = (asc1 - 'a' + 10);
		else if (asc1 >= 'A'&&asc1 <= 'F')
			asc1 = (asc1 - 'A' + 10);
		if (asc2 >= '0'&&asc2 <= '9')
			asc2 = (asc2 - '0');
		else if (asc2 >= 'a'&&asc2 <= 'f')
			asc2 = (asc2 - 'a' + 10);
		else if (asc2 >= 'A'&&asc2 <= 'F')
			asc2 = (asc2 - 'A' + 10);
		return (asc1 * 16 + asc2);
	}




#define uchar unsigned char
//#define uint unsigned int
//#define ulong unsigned long
//#define  ADDR_VER   0x00


	static uchar const auchCRCHi[] = {
		0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81,
		0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0,
		0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01,
		0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41,
		0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81,
		0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0,
		0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01,
		0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,
		0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81,
		0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0,
		0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01,
		0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
		0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81,
		0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0,
		0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01,
		0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
		0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81,
		0x40
	};


	static uchar const auchCRCLo[] = {
		0x00, 0xC0, 0xC1, 0x01, 0xC3, 0x03, 0x02, 0xC2, 0xC6, 0x06, 0x07, 0xC7, 0x05, 0xC5, 0xC4,
		0x04, 0xCC, 0x0C, 0x0D, 0xCD, 0x0F, 0xCF, 0xCE, 0x0E, 0x0A, 0xCA, 0xCB, 0x0B, 0xC9, 0x09,
		0x08, 0xC8, 0xD8, 0x18, 0x19, 0xD9, 0x1B, 0xDB, 0xDA, 0x1A, 0x1E, 0xDE, 0xDF, 0x1F, 0xDD,
		0x1D, 0x1C, 0xDC, 0x14, 0xD4, 0xD5, 0x15, 0xD7, 0x17, 0x16, 0xD6, 0xD2, 0x12, 0x13, 0xD3,
		0x11, 0xD1, 0xD0, 0x10, 0xF0, 0x30, 0x31, 0xF1, 0x33, 0xF3, 0xF2, 0x32, 0x36, 0xF6, 0xF7,
		0x37, 0xF5, 0x35, 0x34, 0xF4, 0x3C, 0xFC, 0xFD, 0x3D, 0xFF, 0x3F, 0x3E, 0xFE, 0xFA, 0x3A,
		0x3B, 0xFB, 0x39, 0xF9, 0xF8, 0x38, 0x28, 0xE8, 0xE9, 0x29, 0xEB, 0x2B, 0x2A, 0xEA, 0xEE,
		0x2E, 0x2F, 0xEF, 0x2D, 0xED, 0xEC, 0x2C, 0xE4, 0x24, 0x25, 0xE5, 0x27, 0xE7, 0xE6, 0x26,
		0x22, 0xE2, 0xE3, 0x23, 0xE1, 0x21, 0x20, 0xE0, 0xA0, 0x60, 0x61, 0xA1, 0x63, 0xA3, 0xA2,
		0x62, 0x66, 0xA6, 0xA7, 0x67, 0xA5, 0x65, 0x64, 0xA4, 0x6C, 0xAC, 0xAD, 0x6D, 0xAF, 0x6F,
		0x6E, 0xAE, 0xAA, 0x6A, 0x6B, 0xAB, 0x69, 0xA9, 0xA8, 0x68, 0x78, 0xB8, 0xB9, 0x79, 0xBB,
		0x7B, 0x7A, 0xBA, 0xBE, 0x7E, 0x7F, 0xBF, 0x7D, 0xBD, 0xBC, 0x7C, 0xB4, 0x74, 0x75, 0xB5,
		0x77, 0xB7, 0xB6, 0x76, 0x72, 0xB2, 0xB3, 0x73, 0xB1, 0x71, 0x70, 0xB0, 0x50, 0x90, 0x91,
		0x51, 0x93, 0x53, 0x52, 0x92, 0x96, 0x56, 0x57, 0x97, 0x55, 0x95, 0x94, 0x54, 0x9C, 0x5C,
		0x5D, 0x9D, 0x5F, 0x9F, 0x9E, 0x5E, 0x5A, 0x9A, 0x9B, 0x5B, 0x99, 0x59, 0x58, 0x98, 0x88,
		0x48, 0x49, 0x89, 0x4B, 0x8B, 0x8A, 0x4A, 0x4E, 0x8E, 0x8F, 0x4F, 0x8D, 0x4D, 0x4C, 0x8C,
		0x44, 0x84, 0x85, 0x45, 0x87, 0x47, 0x46, 0x86, 0x82, 0x42, 0x43, 0x83, 0x41, 0x81, 0x80,
		0x40
	};




/**
 ******************************************************************************
 * @file Utilities.cpp
 * @author lich
 * @version
 * @date 四月 2018
 * @brief
 ******************************************************************************
 * @attention
 *
 * @Contact: 
 ******************************************************************************
 *
 */
	INT16U CRC16(unsigned char* pushMsg, unsigned short usDataLen,unsigned char mode)
	{

		unsigned char uchCRCHi = 0xFF;
		unsigned char uchCRCLo = 0xFF;
		unsigned int uIndex;
		while (usDataLen--)
		{
			uIndex = uchCRCHi ^ *pushMsg++;
			uchCRCHi = uchCRCLo ^ auchCRCHi[uIndex];
			uchCRCLo = auchCRCLo[uIndex];
		}
		//自动增加校验
		 if (mode == 1)
		 {
			 *pushMsg++ = uchCRCHi;
			 *pushMsg++ = uchCRCLo;
		 }
		return (uchCRCHi << 8 | uchCRCLo);
	}

	unsigned char calcXOR(const unsigned char* pData, int iLength)
	{
		unsigned char Res = pData[0];
		for (int i = 1; i < iLength; ++i) {
			Res ^= pData[i];
		}
		return Res;
	}

	bool IsChinese(CString Cstr)
	{
		int nLen = Cstr.GetLength();
		unsigned char ch1, ch2;
		for (int i = 0; i != nLen; ++i)
		{
			ch1 = Cstr[i];
			if (ch1 >= 0xA0)
			{
				return true;
			}
			else
			{
				continue;
			}
		}
		return false;
	}

	//INT16U Checksum_XOR(unsigned char* buf, INT16U len)
	//{
	//	unsigned char check = 0;
	//	unsigned char checkTemp = 0;
	//	check = *buf;
	//	buf++;
	//	checkTemp = *buf;
	//	buf++;
	//	check = check^checkTemp;
	//	len -= 2;
	//	while (len--){
	//		check = check ^ (*buf++);
	//	}
	//	return (check)& 0x00ff;
	//}

}

