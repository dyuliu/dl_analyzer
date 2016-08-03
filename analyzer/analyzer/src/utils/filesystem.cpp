
#include <utils/filesystem.h>

#include <stdlib.h>
#include <Windows.h>
#include <io.h>

#ifndef MAX_PATH
#define MAX_PATH _MAX_PATH
#endif

namespace analyzer {
	std::string filesystem::get_file_name(std::string strPath) {
		char pDrive[MAX_PATH], pDir[MAX_PATH], pFileName[MAX_PATH], pExt[MAX_PATH];
		char pOutput[MAX_PATH];
		::_splitpath_s(strPath.c_str(), pDrive, MAX_PATH, pDir, MAX_PATH, pFileName, MAX_PATH, pExt, MAX_PATH);
		sprintf_s(pOutput, MAX_PATH, "%s%s", pFileName, pExt);
		return std::string(pOutput);
	}

	std::string filesystem::get_file_name_without_extension(std::string strPath) {
		char pDrive[MAX_PATH], pDir[MAX_PATH], pFileName[MAX_PATH], pExt[MAX_PATH];
		::_splitpath_s(strPath.c_str(), pDrive, MAX_PATH, pDir, MAX_PATH, pFileName, MAX_PATH, pExt, MAX_PATH);
		return std::string(pFileName);
	}

	std::string filesystem::get_directory_name(std::string strPath) {
		char pDrive[MAX_PATH], pDir[MAX_PATH], pFileName[MAX_PATH], pExt[MAX_PATH];
		char pOutput[MAX_PATH];
		::_splitpath_s(strPath.c_str(), pDrive, MAX_PATH, pDir, MAX_PATH, pFileName, MAX_PATH, pExt, MAX_PATH);
		sprintf_s(pOutput, MAX_PATH, "%s%s", pDrive, pDir);
		return std::string(pOutput);
	}

	std::string filesystem::get_extension(std::string strPath) {
		char pDrive[MAX_PATH], pDir[MAX_PATH], pFileName[MAX_PATH], pExt[MAX_PATH];
		::_splitpath_s(strPath.c_str(), pDrive, MAX_PATH, pDir, MAX_PATH, pFileName, MAX_PATH, pExt, MAX_PATH);
		return std::string(pExt);
	}

	std::vector<std::string> filesystem::get_files(const char *strFolder, const char *strFilter, bool bAllDirectories) {
		std::vector<std::string> vec = get_files_in_one_folder(strFolder, strFilter);
		if (bAllDirectories) {
			std::vector<std::string> vecSubFolders = get_directories(strFolder, "*", true);
			for (size_t i = 0; i < vecSubFolders.size(); i++) {
				std::vector<std::string> vecFiles = get_files_in_one_folder(vecSubFolders[i].c_str(), strFilter);
				for (size_t j = 0; j < vecFiles.size(); j++)
					vec.push_back(vecFiles[j]);
			}
		}
		return vec;
	}

	std::vector<std::string> filesystem::get_directories(const char *strFolder, const char *strFilter, bool bAllDirectories) {
		std::vector<std::string> vec = get_directory_in_one_folder(strFolder, strFilter);
		if (vec.size() == 0)
			return vec;
		if (bAllDirectories) {
			std::vector<std::string> vecSubFolder;
			for (size_t i = 0; i < vec.size(); i++) {
				std::vector<std::string> vecSub = get_directories(vec[i].c_str(), strFilter, bAllDirectories);
				for (size_t j = 0; j < vecSub.size(); j++) {
					vecSubFolder.push_back(vecSub[j]);
				}
			}
			for (size_t i = 0; i < vecSubFolder.size(); i++)
				vec.push_back(vecSubFolder[i]);
		}
		return vec;
	}

	std::string filesystem::get_current_directory() {
		char strPath[MAX_PATH];
		::GetCurrentDirectoryA(MAX_PATH, strPath);
		return std::string(strPath);
	}

	bool filesystem::exist(const char *strPath) {
		return (_access(strPath, 0) == 0);
	}

	bool filesystem::create_directory(const char *strPath) {
		if (exist(strPath))
			return false;
		char strFolder[MAX_PATH] = { 0 };
		size_t len = strlen(strPath);
		for (size_t i = 0; i <= len; i++) {
			if (strPath[i] == '\\' || strPath[i] == '/' || strPath[i] == '\0') {
				if (!exist(strFolder)) {
					if (::CreateDirectoryA(strFolder, NULL) == 0)
						return false;
				}
			}
			strFolder[i] = strPath[i];
		}
		return true;
	}

	std::vector<std::string> filesystem::get_files_in_one_folder(const char *strFolder, const char *strFilter) {
		std::vector<std::string> vec;
		char strFile[MAX_PATH] = { '\0' };
		sprintf_s(strFile, MAX_PATH, "%s\\%s", strFolder, strFilter);
		WIN32_FIND_DATAA FindFileData;
		HANDLE hFind = ::FindFirstFileA(strFile, &FindFileData);
		if (INVALID_HANDLE_VALUE == hFind)
			return vec;
		do {
			if (FILE_ATTRIBUTE_DIRECTORY == (FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
				continue;
			char strName[MAX_PATH];
			sprintf_s(strName, MAX_PATH, "%s\\%s", strFolder, FindFileData.cFileName);
			vec.push_back(strName);
		} while (::FindNextFileA(hFind, &FindFileData));
		::FindClose(hFind);
		return vec;
	}

	std::vector<std::string> filesystem::get_directory_in_one_folder(const char *strFolder, const char *strFilter) {
		std::vector<std::string> vec;
		char strFile[MAX_PATH] = { '\0' };
		sprintf_s(strFile, MAX_PATH, "%s\\%s", strFolder, strFilter);
		WIN32_FIND_DATAA FindFileData;
		HANDLE hFind = ::FindFirstFileA(strFile, &FindFileData);
		if (INVALID_HANDLE_VALUE == hFind)
			return vec;
		do {
			if (FILE_ATTRIBUTE_DIRECTORY != (FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
				continue;
			if (strcmp(FindFileData.cFileName, ".") == 0 || strcmp(FindFileData.cFileName, "..") == 0)
				continue;
			char strName[MAX_PATH];
			sprintf_s(strName, MAX_PATH, "%s\\%s", strFolder, FindFileData.cFileName);
			vec.push_back(strName);
		} while (::FindNextFileA(hFind, &FindFileData));
		::FindClose(hFind);
		return vec;
	}
}