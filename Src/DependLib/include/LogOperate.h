////////////////////////////////////////////////////////////////////////////
//	File:		LogOperate.h
//	Version:	1.0.0.0
//	Created:	2010-06-10
//
//	Log file rename and backup
////////////////////////////////////////////////////////////////////////////

#ifndef _LOG_OPERATE_H_
#define _LOG_OPERATE_H_

#include <afx.h>
#include <io.h>
#include <direct.h>
#include <iostream>
#include <time.h>
#include <string>
#include <sstream>
using namespace std;

//////////////////////////////////////////////////////////////////////
// 声明部分

#define _MAX_FILE_PATH 1024                                // 支持的最大文件路径名
#define MAX_MAINTENANCE_LOG_NUM 1024                       // 可维护最大文件数1024

char     g_szInitDir[_MAX_FILE_PATH];                           // 日志路径名
char     g_szFileName[MAX_MAINTENANCE_LOG_NUM][_MAX_FILE_PATH]; // 存放文件名数组
time_t   g_szFileCreateTime[MAX_MAINTENANCE_LOG_NUM];      // 文件创建时间与文件名一一对应

int      gnSaveIndex;     // 保存索引号
int      gnMaxSaveNum;    // 最大保存数

// 文件转储外部调用输入文件名，匹配规则，保存数目
bool FileConveySave(const char *dir,const char *filespec,const int nSaveNum);

// 文件重命名，输入原文件绝对路径，目标文件文件名称
bool FileReName(const char *pszSrcFileName, const char* pszDesFileName);

//////////////////////////////////////////////////////////////////////////////
// 实现部分

/************************ Function **********************************
* 函 数 名：sort
* 函数功能：升序排列，按照文件创建时间进行排序
* 日    期：2007-4-10 13:03:39
********************************************************************/
void sort()
{
	time_t tem;
	char temfilename[_MAX_FILE_PATH];
	memset(temfilename, 0x00, _MAX_FILE_PATH);

	for(int i = 0; i < gnSaveIndex - 1; i++)
		for(int j = i + 1; j < gnSaveIndex; j++)
		{
			if(g_szFileCreateTime[i] > g_szFileCreateTime[j])
			{
				tem = g_szFileCreateTime[j];
				g_szFileCreateTime[j] = g_szFileCreateTime[i];
				g_szFileCreateTime[i] = tem;
				strncpy(temfilename, g_szFileName[j], _MAX_FILE_PATH);
				strncpy(g_szFileName[j],g_szFileName[i], _MAX_FILE_PATH);
				strncpy(g_szFileName[i], temfilename, _MAX_FILE_PATH);
			}
		}
}

/************************ Function **********************************
* 函 数 名：SetInitDir
* 函数功能：设置遍历路径, 检查路径合法性
* 日    期：2007-4-10 11:20:25
********************************************************************/
bool SetInitDir(const char *dir)
{
	int nLen = 0;

	//判断目录是否存在
	if (_access(dir,0) != 0)
	{
		return false;
	}
	
	// 保存目录路径
	memset(g_szInitDir, 0x00, _MAX_FILE_PATH);
	memcpy(g_szInitDir, dir, strlen(dir));

	//如果目录的最后一个字母不是'\',则在最后加上一个'\'
	nLen=strlen(g_szInitDir);
	if (g_szInitDir[nLen - 1] != '\\')
	{
		strcat(g_szInitDir,"\\");
	}

	return true;
}


/************************ Function **********************************
* 函 数 名：ProcessFile
* 函数功能：将遍历到的文件保存
* 日    期：2007-4-10 13:09:33
********************************************************************/
bool ProcessFile(const char *filename,time_t createtime)
{
	// 把文件名保存到数组中，判断如果文件个数多于须保存个数，
	// 安照文件生成的时间对文件进行排序，删除最早生成的文件
	// 遍历返回的是最先生产的文件，故这里不需要进行排序
	// 返回是以文件的创建时间为依据，最先创建的最先返回
	time_t filecreatetime;

	filecreatetime = createtime;
	// 小于保存数目时不删除
	//printf("%s\n",filename);
	if (gnSaveIndex < MAX_MAINTENANCE_LOG_NUM)
	{
		_snprintf(g_szFileName[gnSaveIndex],(MAX_MAINTENANCE_LOG_NUM - 1), "%s", filename);
		g_szFileCreateTime[gnSaveIndex] = filecreatetime;
		gnSaveIndex++;
	}
	else // 超过最大维护数，先排序删除最早创建的日志
	{
		// 排序
		sort();
		// 删除文件
		_unlink(g_szFileName[0]);
		_snprintf(g_szFileName[0], (MAX_MAINTENANCE_LOG_NUM - 1), "%s", filename);
		g_szFileCreateTime[0] = filecreatetime;
	}

	return true;
}

/************************ Function **********************************
* 函 数 名：BrowseDir
* 函数功能：遍历文件以及子文件夹，这里不处理子文件夹
* 参数说明：const char *dir;          // 遍历文件路径  
            const char *filespec;     // 变量规则
* 日    期：2007-4-10 12:57:32
********************************************************************/
bool BrowseDir(const char *dir,const char *filespec)
{
	// 遍历文件
	CFileFind logFind;
	ostringstream ostrDir;
	ostrDir<<dir<<"\\"<<filespec;

	try
	{
		// 查找第一个文件
		BOOL IsFinded=(BOOL)logFind.FindFile(ostrDir.str().c_str()); 
		while(IsFinded) 
		{  
			IsFinded=(BOOL)logFind.FindNextFile();
			if(!logFind.IsDots()) 
			{ 
				char foundFileName[_MAX_FILE_PATH]; 
				memset(foundFileName, 0, _MAX_FILE_PATH);

				CString strFileName = logFind.GetFileName();
				strncpy(foundFileName, strFileName.GetBuffer(_MAX_FILE_PATH), (_MAX_FILE_PATH - 1)); 
				strFileName.ReleaseBuffer();

				if(logFind.IsDirectory()) // 如果是目录不处理
				{ 
					continue;
				} 
				else // 查找到文件
				{ 
					char filename[_MAX_FILE_PATH];
					memset(filename, 0x00, _MAX_FILE_PATH);
					strcpy(filename,dir);
					strcat(filename,foundFileName);

					CTime lastWriteTime;
					logFind.GetLastWriteTime(lastWriteTime);
					if (!ProcessFile(filename, (time_t)lastWriteTime.GetTime()))
						return false;
				} 
			} 
		} 
		logFind.Close(); 
	}
	catch (...)
	{
		return false;
	}
	
	return true;
}

/************************ Function **********************************
* 函 数 名：FileConveySave
* 函数功能：实现文件转储功能
* 参数说明：const char *dir;      // 转储文件目录
            const char *filespec; // 转储文件规则
			const int nSaveNum;   // 保存文件数目
* 日    期：2007-4-10 11:08:36
********************************************************************/
bool FileConveySave(const char *dir,const char *filespec,const int nSaveNum)
{
	// 初始化
	memset(g_szFileName, 0x00, sizeof(g_szFileName));
	memset(g_szFileCreateTime, 0x00, sizeof(g_szFileCreateTime));
	gnSaveIndex = 0;

	bool bRes = false;

	gnMaxSaveNum = nSaveNum;

	// 设置遍历目录
	bRes = SetInitDir(dir);
	if (!bRes)
	{
		// 转储失败
		return false;
	}
	
	// 遍历文件,并且将多余的文件删除掉(即大于保存数的文件)
	bRes = BrowseDir(g_szInitDir, filespec);
	if (!bRes)
	{
		// 遍历失败
		return false;
	}

	// 排序
	if (gnSaveIndex > nSaveNum)
	{
		sort();

		// 循环删除多余旧文件
		for(int i = 0; i < gnSaveIndex - nSaveNum; i++)
		{
			_unlink(g_szFileName[i]);
		}
	}

	return true;
}

/************************ Function **********************************
* 函 数 名：FileReName
* 函数功能：文件重命名
* 参数说明：const char *pszSrcFileName; // 源文件绝对路径 
            const char* pszDesFileName; // 目标文件名 
* 日    期：2007-4-10 11:27:25
********************************************************************/
bool FileReName(const char *pszSrcFileName, const char* pszDesFileName)
{
	int  nLen = 0;
	char szPath[_MAX_FILE_PATH];
	char szDir[_MAX_FILE_PATH];
	char szDrive[_MAX_FILE_PATH];
	char szDesPath[_MAX_FILE_PATH];

	memset(szPath, 0x00, _MAX_FILE_PATH);
	memset(szDir, 0x00, _MAX_FILE_PATH);
	memset(szDrive, 0x00, _MAX_FILE_PATH);
	memset(szDesPath, 0x00, _MAX_FILE_PATH);

	// 分离文件路径
	_splitpath(pszSrcFileName, szDrive, szDir, NULL, NULL);

	// 获得目标文件路径
	_snprintf(szPath, (_MAX_FILE_PATH - 1), "%s%s",szDrive,szDir);

	//如果目录的最后一个字母不是'\',则在最后加上一个'\'
	nLen=strlen(szPath);
	if (szPath[nLen - 1] != '\\')
	{
		strcat(szPath,"\\");
	}

	// 获取当前系统时间
	SYSTEMTIME oCurrTime;
	GetLocalTime(&oCurrTime);
	char szCurrTime[32];
	memset(szCurrTime, 0x00, 32);
	_snprintf(szCurrTime, 31, "%02d-%02d-%02d-%02d-%02d-%02d", oCurrTime.wYear, oCurrTime.wMonth,
		oCurrTime.wDay, oCurrTime.wHour, oCurrTime.wMinute, oCurrTime.wSecond);

	_snprintf(szDesPath, 1023, "%s%s.%s.txt", szPath, pszDesFileName,szCurrTime);

	// 重命名文件名
	rename(pszSrcFileName, szDesPath);

	return true; 
}
#endif
 