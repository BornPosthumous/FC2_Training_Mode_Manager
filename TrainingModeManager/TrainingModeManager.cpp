/**
* \file
* The most simple example.
*
*/
#define _WINSOCKAPI_    // stops windows.h including winsock.h
#include <windows.h>
#include <fstream>
#include <vector>

//#include "libs/json/json.hpp"
#include "json_parsers.h"
#include "zip_handler.h"
#include "url.h"
#include "fsys_handler.h"

#define CPPHTTPLIB_OPENSSL_SUPPORT
#include "httplib.h"
#include <wchar.h>
// Directory making
#include <direct.h>

// Filesystem
#include <filesystem> 
#include <stdio.h>
#include <curses.h>

#include <iostream>
#include <windows.h>
#include <typeinfo>
using nlohmann::json;
using namespace training_mode_list_json;
using namespace local_training_mode_listing_json;
using namespace latest_training_mode_release_json;
using latest_training_mode_release_json::releases_response;

struct get_training_mode_listing_retVals {
	int status;
	std::vector<training_mode_listing> training_mode_listing_obj;
	int error;
};

get_training_mode_listing_retVals get_training_mode_listing() {
	std::string training_mode_list_location = "https://raw.githubusercontent.com";
	// HTTPS
	httplib::Client cli(training_mode_list_location.c_str());

	auto res = cli.Get("/BornPosthumous/VSAV_Training/autoup/training_modes.json");
	if (res) {
		// The request body is just a string of text
		std::string training_mode_listing_body = res->body;
		// The status is a status code e.g. 200
		int training_mode_listing_status = res->status;
		std::cout << training_mode_listing_body << std::endl;
		json parsed_training_mode_listing = json::parse(training_mode_listing_body);

		std::vector<training_mode_listing> training_mode_listing_obj = parsed_training_mode_listing.get<std::vector<training_mode_listing>>();

		return get_training_mode_listing_retVals {
			training_mode_listing_status,
			training_mode_listing_obj,
			-1
		};
	}
	else {
		std::vector<training_mode_listing> training_mode_listing_obj;
		return get_training_mode_listing_retVals {
			0,
			training_mode_listing_obj,
			-1
		};
	}
}

struct get_latest_training_mode_release_retVals {
	int status;
	releases_response respdata;
	int error;
};

get_latest_training_mode_release_retVals get_latest_training_mode_release() {
	std::string training_mode_list_location = "https://api.github.com";
	// HTTPS
	httplib::Client cli(training_mode_list_location.c_str());

	auto res = cli.Get("https://api.github.com/repos/BornPosthumous/VSAV_TRAINING/releases/latest");
	if (res) {
		// The request body is just a string of text
		std::string training_mode_release_body = res->body;
		// The status is a status code e.g. 200
		int training_mode_release_status = res->status;
		//std::cout << training_mode_release_status << std::endl;
		//std::cout << training_mode_release_body << std::endl;

		json release = json::parse(training_mode_release_body);

		latest_training_mode_release_json::releases_response training_mode_release_obj = release.get<latest_training_mode_release_json::releases_response>();

		return get_latest_training_mode_release_retVals {
			training_mode_release_status,
			training_mode_release_obj,
			//parsed_person,
			-1
		};
	}
	else {
		latest_training_mode_release_json::releases_response no_data;
		return get_latest_training_mode_release_retVals {
			0,
			no_data,
			-1
		};
	}
}

struct get_training_mode_releases_retVal {
	int status;
	std::vector<training_mode_releases_json::releases_response> respdata;
	int error;
};
void print_releases(training_mode_releases_json::releases_response value) {
	std::cout << value.name << "\n";
	std::cout << value.tag_name << "\n";
	std::cout << value.created_at << "\n";
	std::cout << value.url << "\n";
	std::cout << value.zipball_url << "\n\n";
}

void iterate_release_list(std::vector<training_mode_releases_json::releases_response> training_mode_listing_body) {
	std::for_each(std::begin(training_mode_listing_body), std::end(training_mode_listing_body), print_releases);
}

get_training_mode_releases_retVal get_training_mode_releases() {
	std::string training_mode_list_location = "https://api.github.com";
	// HTTPS
	httplib::Client cli(training_mode_list_location.c_str());

	auto res = cli.Get("https://api.github.com/repos/BornPosthumous/VSAV_Training/releases");
	if (res) {
		// The request body is just a string of text
		std::string training_mode_release_body = res->body;
		// The status is a status code e.g. 200
		int training_mode_release_status = res->status;
		//dbg
		//std::cout << training_mode_release_status << std::endl;
		//std::cout << training_mode_release_body << std::endl;

		json release = json::parse(training_mode_release_body);

		std::vector<training_mode_releases_json::releases_response> training_mode_release_obj = release.get<std::vector<training_mode_releases_json::releases_response>>();

		return get_training_mode_releases_retVal{
			training_mode_release_status,
			training_mode_release_obj,
			-1
		};
	}
	else {
		std::vector<training_mode_releases_json::releases_response> no_data;
		return get_training_mode_releases_retVal {
			0,
			no_data,
			-1
		};
	}
}
auto download_release(latest_training_mode_release_json::releases_response release_obj, std::string dest_filename) {
	// HTTPS
	httplib::Client cli("https://api.github.com");
	// This request will be a 302 which redirects to their cdn
	auto res = cli.Get("/repos/BornPosthumous/VSAV_Training/zipball/v2");
	if (res) {
		if (res->status == 302) {
			std::string location = res->get_header_value("location");
			uriparser::Uri::parsed_uri parsed_redirect_location = uriparser::Uri::get_parsed_uri(location);
			std::string scheme = parsed_redirect_location.scheme;
			std::string host = parsed_redirect_location.host;
			std::string parsed_path = parsed_redirect_location.path;
			std::string client_string = scheme + "://" + host;
			std::cout << client_string << std::endl;
			std::cout << parsed_path << std::endl;

			httplib::Client cli2(client_string.c_str());
			std::ofstream fs;

			std::string body;
			auto res = cli2.Get(parsed_path.c_str(),
				[&](const char* data, size_t data_length) {
					//fs1.write(data, data_length);
					body.append(data, data_length);
					return true;
				});

			try {
				fs.open(dest_filename, std::fstream::binary);
				fs << body;
				fs.close();
			}
			catch (std::ifstream::failure e) {
				std::cerr << "Exception opening/reading/closing file\n";
			}
		}
		else {
			// The request body is just a string of text
			std::string training_mode_release_body = res->body;
			// The status is a status code e.g. 200
			int training_mode_release_status = res->status;
			std::cout << training_mode_release_status << std::endl;
			std::cout << training_mode_release_body << std::endl;

		}
	}
}



void print_game_names(training_mode_list_json::training_mode_listing value) {
}

void display_game_list(std::vector<training_mode_listing> training_mode_listing_body) {
	clear();
	printw("Hello, please choose your training mode\n\n");
	int j = 0;
	std::for_each(std::begin(training_mode_listing_body), std::end(training_mode_listing_body), [&j](auto value) {
		auto built_str =  std::to_string(j) + ". " + value.game + "\n\n";
		mvprintw(2 + j , 2, built_str.c_str());
		j++;
	});
	printw("Press the number for which you would like to retrieve");

	char ch = NULL;
	while (1) {
		ch = getch();
		if (ch == '1') {
			printw("Go Back");
			break;
		}
		else if (ch == '2') {

		}
		else if (ch == '3') {}

		else if (ch == '4') {
			endwin();
			exit(1);
		}
	}
}
struct game_with_training_mode {
	std::string game;
	training_mode_list_json::training_mode_listing training_mode_listing_obj;
};

void load_training_mode(game_with_training_mode game_info) {
	auto training_mode_listing_obj = game_info.training_mode_listing_obj;
	std::string game_name = game_info.game;
	std::filesystem::path parent_path = std::filesystem::current_path().parent_path();
	// The training mode manager will live in its own directory inside of the fbneo folder. The parent dir should be the fbneo path
	//std::string script_path = parent_path.string(); 
	std::string fbneo_path = "D:\\emu\\fc2\\emulator\\fbneo";
	std::string open_script_command = training_mode_listing_obj.script_entry;
	std::string savestate = "savestates\\" + game_name + "_fbneo.fs";
	std::string command = game_name + " " + savestate + " " + fbneo_path + open_script_command;
	std::wstring printerName;
	printerName.assign(command.begin(), command.end());

	ShellExecute(NULL, LPCWSTR(L"open"), LPCWSTR(L"D:\\emu\\fc2\\emulator\\fbneo\\fcadefbneo.exe"), LPCWSTR(printerName.c_str()), NULL, SW_SHOWDEFAULT);
	printw(("Loaded : " + game_name + "\n Press any button to go back to the training mode list").c_str());
	char ch = NULL;
	ch = getch();
	//std::string exe_path = FSYSHandler::get_current_exe_path();
	return;
}
std::vector<game_with_training_mode> prepare_game_list_for_loading(std::vector<training_mode_list_json::training_mode_listing> training_mode_listing_obj) {
	int num_training_modes = training_mode_listing_obj.size();
	std::vector<game_with_training_mode> list_of_games;
	for (int i = 0; i < num_training_modes; i++) {
		int num_supported_games = training_mode_listing_obj[i].supported_roms.size();
		for (int j = 0; j < num_supported_games; j++) {
			game_with_training_mode game_entry {
				training_mode_listing_obj[i].supported_roms[j],
				training_mode_listing_obj[i]
			};
			list_of_games.push_back(game_entry);
		}
	}
	return list_of_games;
}
void main_menu();
void game_list_menu(std::vector<training_mode_list_json::training_mode_listing> training_mode_listing_obj) {
	clear();
	printw("Hello, please type the number for the game you'd like to train in and hit enter: \n\nThis will not work if you don't have the ROM (duh!) \n\n");
	//int num_training_modes = training_mode_listing_obj.size();

	//for (int i = 0; i < num_training_modes; i++) {
	//	std::string built_str = std::to_string(i + 1) + ". " + training_mode_listing_obj[i].game;
	//	mvprintw(i + 1, 1, built_str.c_str());
	//}
	auto gamelist = prepare_game_list_for_loading(training_mode_listing_obj);
	int gamelist_size = gamelist.size();

	for (int i = 0; i < gamelist_size; i++) {
		std::string built_str = std::to_string(i + 1) + ". " + gamelist[i].game;
		mvprintw(i + 4, 1, built_str.c_str());
	}

	char* inp = new char[10];
	while (1) {
		//ch = getch();
		getstr(inp);
		clear();
		int inp_num = atoi(inp);
		bool in_range = (inp_num < gamelist_size) && (inp_num > 0);
		if (in_range == 1) {
			clear();
			load_training_mode(gamelist[inp_num - 1]);
		}
		else if (inp_num == 0) {
			endwin();
			main_menu();
		}
		else {
			clear();
			printw("Invalid number, press any key to go back to the training mode listing.");
			getch();
			game_list_menu(training_mode_listing_obj);
		}
	}
}
void main_menu() {
	initscr();
	noecho();
	curs_set(FALSE);
	cbreak();

	if (has_colors() == FALSE)
	{
		endwin();
		printf("Your terminal does not support color\n");
		exit(1);
	}
	start_color();
	init_pair(1, COLOR_RED, COLOR_BLACK);
	init_pair(2, COLOR_GREEN, COLOR_BLACK);
	init_pair(3, COLOR_BLUE, COLOR_BLACK);
	init_pair(4, COLOR_YELLOW, COLOR_BLACK);

	attron(COLOR_PAIR(1));
	mvprintw(1, 1, "1. Play Training Mode");
	attroff(COLOR_PAIR(1));

	attron(COLOR_PAIR(2));
	mvprintw(2, 1, "2. Update Training Modes");
	attroff(COLOR_PAIR(2));

	attron(COLOR_PAIR(3));
	mvprintw(3, 1, "3. Download New Training Modes");
	attroff(COLOR_PAIR(3));

	attron(COLOR_PAIR(4));
	mvprintw(4, 1, "4. Exit");
	attroff(COLOR_PAIR(4));

	char ch = NULL;
	while (1) {
		ch = getch();
		if (ch == '1') {
			auto [
				training_mode_listing_status,
					training_mode_listing_obj,
					e
			] = get_training_mode_listing();

			game_list_menu(training_mode_listing_obj);
		}
		else if (ch == '2') {
			auto [
				training_mode_listing_status,
					training_mode_listing_obj,
					e
			] = get_training_mode_listing();
				display_game_list(training_mode_listing_obj);
				game_list_menu(training_mode_listing_obj);
		}
		else if (ch == '3') {}

		else if (ch == '4') {
			endwin();
			exit(1);
		}
	}

	refresh();
	getch();
	endwin();
}

std::wstring ExePath() {
	TCHAR buffer[MAX_PATH] = { 0 };
	GetModuleFileName(NULL, buffer, MAX_PATH);
	std::wstring::size_type pos = std::wstring(buffer).find_last_of(L"\\/");
	return std::wstring(buffer).substr(0, pos);
}

struct get_training_mode_commits_retVal {
	int status;
	std::vector<training_mode_commits::commits_response> respdata;
	int error;
};
get_training_mode_commits_retVal get_training_mode_last_commit(std::string repo_path ) {
	std::string training_mode_list_location = "https://api.github.com";
	std::string url = "https://api.github.com/repos/" + repo_path + "/commits";

	httplib::Client cli(training_mode_list_location.c_str());
	auto res = cli.Get(url.c_str());
	if (res) {
		// The request body is just a string of text
		std::string training_mode_commits_body = res->body;
		// The status is a status code e.g. 200
		int training_mode_commits_status = res->status;
		//std::cout << training_mode_commits_status << std::endl;
		//std::cout << training_mode_commits_body << std::endl;

		json release = json::parse(training_mode_commits_body);

		std::vector<training_mode_commits::commits_response> training_mode_commits_obj = release.get<std::vector<training_mode_commits::commits_response>>();
		std::cout << training_mode_commits_obj[0].sha << std::endl;

		return get_training_mode_commits_retVal{
			training_mode_commits_status,
			training_mode_commits_obj,
			//parsed_person,
			-1
		};
	}
	else {
		std::vector<training_mode_commits::commits_response> no_data;
		return get_training_mode_commits_retVal{
			0,
			no_data,
			-1
		};
	}
}

std::vector<local_training_mode_listing_json::local_training_mode_listing> create_or_load_config(std::vector<training_mode_listing> training_mode_listing_obj) {
	int num_training_modes = training_mode_listing_obj.size();
	std::vector<local_training_mode_listing_json::local_training_mode_listing> local_listing;
	for (int i = 0; i < num_training_modes; i++) {
		json patch = R"({
				"installed": true,
				"installed_version": ""
            })"_json;
		json tmp = training_mode_listing_obj[i];
		tmp.merge_patch(patch);
		local_training_mode_listing_json::local_training_mode_listing upd = tmp.get<local_training_mode_listing_json::local_training_mode_listing>();
		local_listing.push_back(tmp);
	};
	return local_listing;
}

void check_for_training_mode_update(training_mode_manager_cfg_json::training_mode_manager_cfg training_mode_config) {
	auto installed_training_modes = training_mode_config.installed_training_modes;
	for (int i = 0; i > installed_training_modes.size(); i++) {
		std::cout << installed_training_modes[i].repo_base_url << std::endl;
	}
}

int main(int, char**)
{
	//try
	//{
	//	std::ifstream training_mode_list_file("training_mode_list.json");
	//	json jf = json::parse(training_mode_list_file);
	//	std::vector<training_mode_list_json::training_mode_listing> training_mode_list = jf.get<std::vector<training_mode_list_json::training_mode_listing>>();
	//	// Using for_each algorithm, using a function or functor:
	//	std::for_each(std::begin(training_mode_list), std::end(training_mode_list), print_game_names);
	//}
	//catch (json::parse_error& e)
	//{
	//	// output exception information
	//	std::cout << "message: " << e.what() << '\n'
	//		<< "exception id: " << e.id << '\n'
	//		<< "byte position of error: " << e.byte << std::endl;
	//}
	// Convert JSON --> Person
	auto [
		training_mode_listing_status,
		training_mode_listing_obj,
		e
	] = get_training_mode_listing();
	//auto [
	//	training_mode_release_listing_status,
	//	training_mode_release_listing_obj,
	//	error
	//] = get_training_mode_releases();

	//iterate_release_list(training_mode_release_listing_obj);
	//auto [
	//	training_mode_release_status,
	//	training_mode_release_obj,
	//	errorcode
	//] = get_latest_training_mode_release();

	//std::string dest_zip = "confetti.zip";
	//std::string dest_dir = "dest";
	//download_release(training_mode_release_obj, dest_zip);
	//if (FSYSHandler::dir_exists("../dest/dest") == 0) {
	//	FSYSHandler::delete_directory("../dest");
	//}
	//else {
	//	printf("Nah fam");
	//};
	//std::string folder_name = ZipHandler::get_folder_name_from_archive(dest_zip.c_str());

	//ZipHandler::extract_zip_archive(dest_zip.c_str());
	//ZipHandler::extract_zip_archive(dest_zip.c_str());
	//bool dest = std::filesystem::create_directory(dest_dir);
	//std::filesystem::copy(folder_name, dest_dir, std::filesystem::copy_options::recursive);


	//get_training_mode_last_commit();

	int ex = FSYSHandler::file_exists("cfg.json");
	//std::string version = "tmp_version";
	std::string version = "9ff0f9195856354a4d466717a0116546e12ea49b";
	if (ex == false) {
		auto training_mode_cfg_obj= create_or_load_config(training_mode_listing_obj);
		training_mode_manager_cfg_json::training_mode_manager_cfg tm_config {
			false,
			version,
			training_mode_cfg_obj
		};
		std::ofstream out("cfg.json");
		json o_config = tm_config;
		out << o_config;
		out.close();
	}
	else {
		std::ifstream training_mode_list_file("cfg.json");
		json jf = json::parse(training_mode_list_file);

		auto training_mode_config = jf.get<training_mode_manager_cfg_json::training_mode_manager_cfg>();
		std::string cur_version = training_mode_config.version;

		std::string training_mode_manager_repo = "BornPosthumous/VSAV_Training";
		get_training_mode_commits_retVal commits = get_training_mode_last_commit(training_mode_manager_repo);
		std::string latest_ver = commits.respdata[0].sha;
		std::cout << "eq?" << (latest_ver == cur_version) << std::endl;

		if (latest_ver == cur_version) {

			std::cout << "does not need update" << std::endl;
			check_for_training_mode_update(training_mode_config);

		}
		else {
			std::cout << "needs update" << "current: " << cur_version << "latest" << latest_ver << std::endl;
		}
		std::getchar();

	}

	main_menu();

	std::getchar();

	return 0;
}
