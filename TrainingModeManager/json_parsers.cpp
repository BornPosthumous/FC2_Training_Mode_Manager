//#include "json_parsers.h"

#include <string>
#include <vector>
#include "json.h"
#include "json_parsers.h"
using nlohmann::json;

#include <cctype>
#include <sstream>
std::string escape_string(std::string  your_string) {
	// s is our escaped output string
	std::string s = "";
	// loop through all characters
	for (char c : your_string)
	{
		// check if a given character is printable
		// the cast is necessary to avoid undefined behaviour
		if (isprint((unsigned char)c))
			s += c;
		else
		{
			std::stringstream stream;
			// if the character is not printable
			// we'll convert it to a hex string using a stringstream
			// note that since char is signed we have to cast it to unsigned first
			stream << std::hex << (unsigned int)(unsigned char)(c);
			std::string code = stream.str();
			s += std::string("\\x") + (code.size() < 2 ? "0" : "") + code;
			// alternatively for URL encodings:
			//s += std::string("%")+(code.size()<2?"0":"")+code;
		}
	}
	return s;
}
namespace latest_training_mode_release_json {
	void to_json(json& j, const latest_training_mode_release_json::releases_response& p) {
		j = json{ {"url", p.url}, {"zipball_url", p.zipball_url }, {"body", p.body} };
	}

	void from_json(const json& j, latest_training_mode_release_json::releases_response& p) {
		j.at("url").get_to(p.url);
		j.at("zipball_url").get_to(p.zipball_url);
		j.at("body").get_to(p.body);
	}
}

namespace training_mode_releases_json {
	void to_json(json& j, const training_mode_releases_json::releases_response& p) {
		j = json{ 
			{"url", p.url}, 
			{"zipball_url", p.zipball_url }, 
			{"body", p.body},
			{"name", p.name},
			{"tag_name", p.tag_name},
			{"created_at", p.created_at}
		};
	}

	void from_json(const json& j, training_mode_releases_json::releases_response& p) {
		j.at("url").get_to(p.url);
		j.at("zipball_url").get_to(p.zipball_url);
		j.at("body").get_to(p.body);
		j.at("name").get_to(p.name);
		j.at("tag_name").get_to(p.tag_name);
		j.at("created_at").get_to(p.created_at);
	}
}
namespace training_mode_list_json {
	void to_json(json& j, const training_mode_list_json::training_mode_listing& t) {
		j = json{ 
			{"game", t.game},
			{"description", t.description},
			{"author", t.author},
			{"latest_url", t.latest_url},
			{"releases_url", t.releases_url},
			{"key", t.key},
			{"script_entry", t.script_entry},
			{"supported_roms", t.supported_roms},
			{"savestate", t.savestate},
			{"repo_base_url", t.repo_base_url }

		};
	}

	void from_json(const json& j, training_mode_list_json::training_mode_listing& t) {
		j.at("game").get_to(t.game);
		j.at("description").get_to(t.description);
		j.at("author").get_to(t.author);
		j.at("latest_url").get_to(t.latest_url);
		j.at("releases_url").get_to(t.releases_url);
		j.at("key").get_to(t.key);
		j.at("supported_roms").get_to(t.supported_roms);
		j.at("script_entry").get_to(t.script_entry);
		j.at("savestate").get_to(t.savestate);
		j.at("repo_base_url").get_to(t.repo_base_url);
	}
};

namespace local_training_mode_listing_json {
	void to_json(json& j, const local_training_mode_listing& t) {
		j = json{
			{"game", t.game},
			{"description", t.description},
			{"author", t.author},
			{"latest_url", t.latest_url},
			{"releases_url", t.releases_url},
			{"installed", t.installed},
			{"installed_version", t.installed_version },
			{"key", t.key},
			{"script_entry", t.script_entry},
			{"supported_roms", t.supported_roms},
			{"repo_base_url", t.repo_base_url },
			{"savestate", t.savestate}
		};
	}

	void from_json(const json& j, local_training_mode_listing& t) {
		j.at("game").get_to(t.game);
		j.at("description").get_to(t.description);
		j.at("author").get_to(t.author);
		j.at("latest_url").get_to(t.latest_url);
		j.at("releases_url").get_to(t.releases_url);
		j.at("installed").get_to(t.installed);
		j.at("installed_version").get_to(t.installed_version);
		j.at("key").get_to(t.key);
		j.at("supported_roms").get_to(t.supported_roms);
		j.at("script_entry").get_to(t.script_entry);
		j.at("repo_base_url").get_to(t.repo_base_url);
		j.at("savestate").get_to(t.savestate);

	}
}

namespace training_mode_commits {

	void to_json(nlohmann::json& j, const commits_response& t) {
		j = json {
			{"sha", t.sha},
		};
	}

	void from_json(const nlohmann::json& j, commits_response& t) {
		j.at("sha").get_to(t.sha);
	}

}


namespace training_mode_manager_cfg_json {

	void to_json(nlohmann::json& j, const training_mode_manager_cfg& t) {
		j = json{
			{"first_run", t.first_run},
			{"version", t.version},
			{"installed_training_modes", t.installed_training_modes }
		};
	};

	void from_json(const nlohmann::json& j, training_mode_manager_cfg& t) {
		j.at("first_run").get_to(t.first_run);
		j.at("installed_training_modes").get_to(t.installed_training_modes);

	};

}