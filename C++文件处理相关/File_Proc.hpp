//����һ����ר������ʵ���ļ��������ع���
//===================================================================
#ifndef FILE_PROC_HPP
#define FILE_PROC_HPP

#include <iostream>
#include <windows.h>
#include <stdio.h>
#include <vector>

using std::cout;
using std::cin;

#define MAX_PATH 260  //��ļ����������ַ���

typedef struct
{
	char *name[MAX_PATH];
}FileVector;

template <class Type>
class File_Proc{
public:
	//���캯��
	File_Proc(){};
	//�鿴�ļ��Ƿ���ڲ��õ��ļ���ϢFindFileData,szName���ļ�·����
	int FileExist(LPSTR szName, WIN32_FIND_DATAA *FindFileData = NULL);
	//�����õ�·��path_name�£���չ��ΪfilterExt�������ļ�files[0..nCount-1],�����ļ�����nCount
	int BrowPathFiles(char *path_name, char *filterExt, std::vector<FileVector> &files);  //files�д洢��ֻ��·����������·��
	//�����õ��ļ���·���µ��������ļ��в�����files(ֻ���ļ�����û��·��)
	int BrowPathDirectory(char *path_name, std::vector<FileVector> files);
	//����ָ���ļ�fname��һ��Ϊtxt���е����ݵ�content��,flag��ʾ����ѡ�õ�ģʽ
	int load(char *fname, Type *content , int flag);
	//��������
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
	//��ƴ�ӵõ�����չ����·����path_ext_name�ַ���
	strcpy(path_ext_name, path_name); strcat(path_ext_name, "*.*");
	WIN32_FIND_DATAA FileData;
	HANDLE hSearch;
	int nCount = 0;
	BOOL fFinished = FALSE;
	char fname[512], ext_name[512];
	str_no_cap(filterExt);
	// Start searching for path_ext_name files in the current directory. 
	hSearch = FindFirstFileA(path_ext_name, &FileData);//��������·���µĵ�һ���ļ�FileData.cFileName
	if (hSearch != INVALID_HANDLE_VALUE)
	{ //
		//strcpy(files[nCount],FileData.cFileName);nCount++;//��ӵ�һ���ļ�
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
		{ //MessageBoxA(NULL,"Couldn't find next file.","��ã�",MB_OK);
		}
		// Close the search handle. 
		if (!FindClose(hSearch))
		{
			//	MessageBoxA(NULL,"Couldn't close search handle.","��ã�",MB_OK);
		}
	}
	else//����û�ҵ���·���µĵ�һ���ļ�,��û�ҵ��κ��ļ�
	{ //MessageBoxA(NULL,"No file found.","��ã�",MB_OK);
	}

	return files.size();
}

template<class Type>
int File_Proc<Type>::BrowPathDirectory(char *path_name, std::vector<FileVector> &files)
{
	/*	std::vector<FileVector> files;*/
	char path_ext_name[MAX_PATH];
	//��ƴ�ӵõ�����չ����·����path_ext_name�ַ���
	strcpy(path_ext_name, path_name);
	strcat(path_ext_name, "*.*");
	WIN32_FIND_DATAA FileData;
	HANDLE hSearch;
	int nCount = 0;
	BOOL fFinished = FALSE;
	char fname[512], ext_name[512];
	// Start searching for path_ext_name files in the current directory. 
	hSearch = FindFirstFileA(path_ext_name, &FileData);//��������·���µĵ�һ���ļ�FileData.cFileName
	if (hSearch != INVALID_HANDLE_VALUE)
	{ //
		//strcpy(files[nCount],FileData.cFileName);nCount++;//��ӵ�һ���ļ�
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
		if (GetLastError() != ERROR_NO_MORE_FILES){ MessageBoxA(NULL, "Couldn't find next file.", "��ã�", MB_OK); }
		// Close the search handle. 
		if (!FindClose(hSearch))
		{
			MessageBoxA(NULL, "Couldn't close search handle.", "��ã�", MB_OK);
		}
	}
	else//����û�ҵ���·���µĵ�һ���ļ�,��û�ҵ��κ��ļ�
	{
		MessageBoxA(NULL, "No file found.", "��ã�", MB_OK);
	}
	return files.size();
}

template <class Type>
int File_Proc<Type>::load(char *fname, Type *content , int flag)
{
	//����txt�����ݵĸ�ʽ�޸Ĵ��룬����fscanf���룻

}


#endif