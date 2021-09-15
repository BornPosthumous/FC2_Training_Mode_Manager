#pragma once
#include <string>
#include <vector>
#include "libs/json/json.hpp"


namespace training_mode_list_json {
	struct training_mode_listing {
		std::string game;
		std::string description;
		std::string latest_url;
		std::string author;
		std::string script_entry;
		std::string savestate;
		std::string releases_url;
		std::string key;
		std::string repo_base_url;
		std::vector<std::string> supported_roms;
		bool use_release;
		bool use_savestate;

	};
	void to_json(nlohmann::json& j, const training_mode_listing& t);

	void from_json(const nlohmann::json& j, training_mode_listing& t);
};

namespace local_training_mode_listing_json {
	using namespace training_mode_list_json;
	struct local_training_mode_listing {
		bool installed;
		std::string installed_version;
		std::string game;
		std::string description;
		std::string latest_url;
		std::string author;
		std::string script_entry;
		std::string savestate;
		std::string releases_url;
		std::string key;
		std::string repo_base_url;
		std::vector<std::string> supported_roms;
		bool use_release;
		bool use_savestate;
	};
	void to_json(nlohmann::json& j, const local_training_mode_listing& t);

	void from_json(const nlohmann::json& j, local_training_mode_listing& t);
};

namespace latest_training_mode_release_json {
	struct releases_response {
		std::string url;
		std::string zipball_url;
		std::string body;
	};
	void to_json(nlohmann::json& j, const releases_response& t);

	void from_json(const nlohmann::json& j, releases_response& t);
};

namespace training_mode_releases_json {
	struct releases_response {
		std::string url;
		std::string zipball_url;
		std::string body;
		std::string name;
		std::string tag_name;
		std::string created_at;
	};
	void to_json(nlohmann::json& j, const releases_response& t);

	void from_json(const nlohmann::json& j, releases_response& t);
};

namespace training_mode_commits {
	struct commits_response {
		std::string sha;
	};
	void to_json(nlohmann::json& j, const commits_response& t);

	void from_json(const nlohmann::json& j, commits_response& t);

}

namespace training_mode_manager_cfg_json {
	struct training_mode_manager_cfg {
		bool first_run;
		std::string version;
		std::vector<local_training_mode_listing_json::local_training_mode_listing> installed_training_modes;
	};
	void to_json(nlohmann::json& j, const training_mode_manager_cfg& t);

	void from_json(const nlohmann::json& j, training_mode_manager_cfg& t);

}
