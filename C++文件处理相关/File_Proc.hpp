//定义一个类专门用来实现文件处理的相关功能
//===================================================================
#ifndef FILE_PROC_HPP
#define FILE_PROC_HPP

#include <iostream>
#include <windows.h>
#include <stdio.h>
#include <vector>

using std::cout;
using std::cin;

#define MAX_PATH 260  //最长文件名包含的字符数

typedef struct
{
	char *name[MAX_PATH];
}FileVector;

template <class Type>
class File_Proc{
public:
	//构造函数
	File_Proc(){};
	//查看文件是否存在并得到文件信息FindFileData,szName是文件路径名
	int FileExist(LPSTR szName, WIN32_FIND_DATAA *FindFileData = NULL);
	//遍历得到路径path_name下，扩展名为filterExt的所有文件files[0..nCount-1],返回文件个数nCount
	int BrowPathFiles(char *path_name, char *filterExt, std::vector<FileVector> &files);  //files中存储的只有路径，不包含路径
	//遍历得到文件夹路径下的所有子文件夹并存在files(只有文件名，没有路径)
	int BrowPathDirectory(char *path_name, std::vector<FileVector> files);
	//加载指定文件fname（一般为txt）中的内容到content中,flag表示加载选用的模式
	int load(char *fname, Type *content , int flag);
	//析构函数
	~File_Proc(){};
private:
	//
};

template <class Type>
int File_Proc<Type>::FileExist(LPSTR szName, WIN32_FIND_DATAA *FindFileData = NULL)
{
	char fname[512];
	strcpy(fname, szName);
	int len = strlen(szName);
	while (szName[len - 1] == '\\' || szName[len - 1] == '/')
		len--;
	fname[len] = '\0';
	WIN32_FIND_DATAA _FindFileData;
	HANDLE hFind;
	hFind = FindFirstFileA(fname, FindFileData ? FindFileData : &_FindFileData);
	return (hFind != INVALID_HANDLE_VALUE);
}

template <class Type>
int File_Proc<Type>::BrowPathFiles(char *path_name, char *filterExt, std::vector<FileVector> &files)
{
	char path_ext_name[MAX_PATH];
	//先拼接得到带扩展名的路径名path_ext_name字符串
	strcpy(path_ext_name, path_name); strcat(path_ext_name, "*.*");
	WIN32_FIND_DATAA FileData;
	HANDLE hSearch;
	int nCount = 0;
	BOOL fFinished = FALSE;
	char fname[512], ext_name[512];
	str_no_cap(filterExt);
	// Start searching for path_ext_name files in the current directory. 
	hSearch = FindFirstFileA(path_ext_name, &FileData);//先搜索本路径下的第一个文件FileData.cFileName
	if (hSearch != INVALID_HANDLE_VALUE)
	{ //
		//strcpy(files[nCount],FileData.cFileName);nCount++;//添加第一个文件
		while (FindNextFileA(hSearch, &FileData))
			if ((FileData.dwFileAttributes & 0xff) != FILE_ATTRIBUTE_DIRECTORY)
			{
			FileVector afile;
			ExtractFileExt(FileData.cFileName, fname, ext_name);
			str_no_cap(ext_name);
			if (strcmp(ext_name, ".") && (strstr(filterExt, ext_name) || strstr(filterExt, "*.*")))
			{
				strcpy(afile.name, FileData.cFileName);
				files.push_back(afile);
			}
			}
		if (GetLastError() != ERROR_NO_MORE_FILES)
		{ //MessageBoxA(NULL,"Couldn't find next file.","你好！",MB_OK);
		}
		// Close the search handle. 
		if (!FindClose(hSearch))
		{
			//	MessageBoxA(NULL,"Couldn't close search handle.","你好！",MB_OK);
		}
	}
	else//否则没找到本路径下的第一个文件,即没找到任何文件
	{ //MessageBoxA(NULL,"No file found.","你好！",MB_OK);
	}

	return files.size();
}

template<class Type>
int File_Proc<Type>::BrowPathDirectory(char *path_name, std::vector<FileVector> &files)
{
	/*	std::vector<FileVector> files;*/
	char path_ext_name[MAX_PATH];
	//先拼接得到带扩展名的路径名path_ext_name字符串
	strcpy(path_ext_name, path_name);
	strcat(path_ext_name, "*.*");
	WIN32_FIND_DATAA FileData;
	HANDLE hSearch;
	int nCount = 0;
	BOOL fFinished = FALSE;
	char fname[512], ext_name[512];
	// Start searching for path_ext_name files in the current directory. 
	hSearch = FindFirstFileA(path_ext_name, &FileData);//先搜索本路径下的第一个文件FileData.cFileName
	if (hSearch != INVALID_HANDLE_VALUE)
	{ //
		//strcpy(files[nCount],FileData.cFileName);nCount++;//添加第一个文件
		while (FindNextFileA(hSearch, &FileData))
			if ((FileData.dwFileAttributes & 0xff) == FILE_ATTRIBUTE_DIRECTORY)
			{
			FileVector afile;
			ExtractFileExt(FileData.cFileName, fname, ext_name);
			str_no_cap(ext_name);
			if (strcmp(ext_name, ".") && strcmp(ext_name, ".."))
			{
				strcpy(afile.name, FileData.cFileName);
				files.push_back(afile);
			}
			}
		if (GetLastError() != ERROR_NO_MORE_FILES){ MessageBoxA(NULL, "Couldn't find next file.", "你好！", MB_OK); }
		// Close the search handle. 
		if (!FindClose(hSearch))
		{
			MessageBoxA(NULL, "Couldn't close search handle.", "你好！", MB_OK);
		}
	}
	else//否则没找到本路径下的第一个文件,即没找到任何文件
	{
		MessageBoxA(NULL, "No file found.", "你好！", MB_OK);
	}
	return files.size();
}

template <class Type>
int File_Proc<Type>::load(char *fname, Type *content , int flag)
{
	//根据txt中内容的格式修改代码，进行fscanf输入；

}


#endif