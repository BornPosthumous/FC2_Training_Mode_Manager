#pragma once
#include <string>
// Libarchive
#include <archive.h>
#include <archive_entry.h>

class ZipHandler {
public:
	static std::string get_folder_name_from_archive(const char* filename);
	static int copy_data(struct archive* ar, struct archive* aw);
	static void extract_zip_archive(const char* filename);
};
