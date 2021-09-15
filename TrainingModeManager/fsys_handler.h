#pragma once
#include <string>

class FSYSHandler {
public:
	static int file_exists(std::string pathname);
	static int dir_exists(std::string pathname);
	static int make_directory(std::string path);
	static int delete_directory(std::string path);
	static int move_directory(const char* filename, const char* path);
	static std::string get_current_exe_path();
};