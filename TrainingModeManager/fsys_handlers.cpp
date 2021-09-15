#include <fsys_handler.h>

#include <string>
#include <direct.h>
#include <filesystem>
#include <iostream>
// For directory checking
#include <sys/types.h>
#include <sys/stat.h>

int FSYSHandler::file_exists(std::string path) {
	struct stat info;
	auto pathname = path.c_str();
	if (std::filesystem::exists(pathname)) {
		return true;
	}
	else {
		return false;
	}
}

int FSYSHandler::dir_exists(std::string path) {
	struct stat info;
	auto pathname = path.c_str();
	if (stat(pathname, &info) != 0) {
		printf("cannot access %s\n", pathname);
		return 1;
	} else if (info.st_mode & S_IFDIR){  // S_ISDIR() doesn't exist on my windows 
		printf("%s is a directory\n", pathname);
		return 0;
	}
	else {
		printf("%s is no directory\n", pathname);
		return 0;
	}
}
int FSYSHandler::make_directory(std::string path) {
	// mkdir returns 0 if the directory was created
	// on an error it returns -1 
	if (_mkdir(path.c_str()) == 0)
	{
		printf("Directory '%s' was successfully created\n", path.c_str());
		return 0;
	}
	else
		printf("Problem creating directory: '%s'", path.c_str());
	return -1;
}

int FSYSHandler::delete_directory(std::string path) {
	//true if the file was deleted, false if it did not exist
	if (std::filesystem::remove_all(path) == 1)
	{
		printf("Directory '%s' was successfully removed\n", path.c_str());
		return 0;
	}
	else
		printf("Problem removing directory: '%s'", path.c_str());
	return -1;
}
int FSYSHandler::move_directory(const char* filename, const char* path) {
	try {
		std::string newPath = std::string(path) + std::string(filename);
		std::filesystem::rename(filename, newPath);
	}
	catch (std::filesystem::filesystem_error& e) {
		std::cout << e.what() << '\n';
	}
	return 0;
}
std::string FSYSHandler::get_current_exe_path() {

	std::filesystem::path cwd = std::filesystem::current_path();
	printf(cwd.string().c_str());

	return cwd.string().c_str();

}