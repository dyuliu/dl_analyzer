
#pragma once

#include <string>
#include <vector>

namespace analyzer {

	class filesystem {
	public:
		// static helper functions to access file system

		// path functions
		static std::string get_file_name(std::string strPath);
		static std::string get_file_name_without_extension(std::string strPath);
		static std::string get_directory_name(std::string strPath);
		static std::string get_extension(std::string strPath);

		// directory functions
		static std::vector<std::string> get_files(const char *strFolder, const char *strFilter, bool bAllDirectories);
		static std::vector<std::string> get_directories(const char *strFolder, const char *strFilter, bool bAllDirectories);
		static std::string get_current_directory();
		static bool exist(const char *strPath);
		static bool create_directory(const char *strPath);

	private:
		static std::vector<std::string> get_files_in_one_folder(const char *strFolder, const char *strFilter);
		static std::vector<std::string> get_directory_in_one_folder(const char *strFolder, const char *strFilter);
	};
}