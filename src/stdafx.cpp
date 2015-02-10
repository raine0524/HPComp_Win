// stdafx.cpp : 只包括标准包含文件的源文件
// librecord.pch 将成为预编译头
// stdafx.obj 将包含预编译类型信息

#include "stdafx.h"

unsigned long get_file_len(const char *filename)
{
	//如果文件不存在，返回0;
	if (!_access(filename, 0) == 0)
	{
		return 0;
	}

	struct stat buf;
	if(stat(filename, &buf)<0)
	{
		return 0;
	}
	return (unsigned long)(buf.st_size/1024.0);
}

unsigned long GetFileLen(const char* lpszFileName, int nMode)
{

#ifdef WIN32
	__int64 iLength = 0; 

	HANDLE hFile  = INVALID_HANDLE_VALUE; 

	hFile = CreateFileA(lpszFileName, 
		GENERIC_READ| GENERIC_WRITE, 
		FILE_SHARE_READ | FILE_SHARE_WRITE, 
		NULL, OPEN_EXISTING, FILE_FLAG_RANDOM_ACCESS, NULL);

	if (hFile == INVALID_HANDLE_VALUE)
		return (unsigned long)(iLength/1024.0);


	LARGE_INTEGER   li;   
	if (GetFileSizeEx(hFile, &li))   
		iLength = li.QuadPart;

	CloseHandle(hFile);
	if (nMode==0)
		return (unsigned long)(iLength/1024.0);
	else
		return (unsigned long)(iLength);
#else
	return get_file_len(lpszFileName);
#endif

}

#ifdef WIN32

unsigned long get_disk_free( const string& strPath)
{
	USES_CONVERSION;
	unsigned _int64 i64Free = 0;

	unsigned _int64 i64FreeBytesToCaller;
	unsigned _int64 i64TotalBytes;
	unsigned _int64 i64FreeBytes;

	string strDisk = strPath.substr( 0, 3);
	BOOL fResult = GetDiskFreeSpaceEx (
		A2W(strDisk.c_str()),
		(PULARGE_INTEGER)&i64FreeBytesToCaller,
		(PULARGE_INTEGER)&i64TotalBytes,
		(PULARGE_INTEGER)&i64FreeBytes);

	if ( fResult )
	{
		i64Free = (unsigned _int64)(i64FreeBytesToCaller / Msize);
	}
	return (unsigned long)i64Free;
}

#else

unsigned long get_disk_free( const string& strPath)
{
	long long llFreeM = 0;

	long long llFreeSize = 0;

	struct statfs fs;
	if (statfs( strPath.c_str(), &fs)<0)
	{
		printf("statfs( strPath = %s.\n",strPath.c_str());
		return 0;
	}

	// 应该统一为可用空间才行！
	llFreeSize = (long long)fs.f_bavail * fs.f_bsize;
	llFreeM = llFreeSize / Msize;
	//printf("llFreeM = %d\n",llFreeM);
	return llFreeM;
}

#endif

CString GetIniName()
{
	return _T("d:\\HPCompSvr\\HPCompServer.ini");
}

CString GetCompVersion()
{
	CString strVersion = CA2W(AVCON_SERVER_VERSION);
	strVersion += _T(".1209");		//日期字段
	return strVersion;
}