#include "FileSorts.h"

#include "utils/StringUtil.h"

namespace FileSorts
{
	const FileData::SortType typesArr[] = {
		FileData::SortType(&compareName, true, "文件名,升序"),
		FileData::SortType(&compareName, false, "文件名,降序"),

		FileData::SortType(&compareRating, true, "评分,升序"),
		FileData::SortType(&compareRating, false, "评分,降序"),

		FileData::SortType(&compareTimesPlayed, true, "游戏时长,升序"),
		FileData::SortType(&compareTimesPlayed, false, "游戏时长,降序"),

		FileData::SortType(&compareLastPlayed, true, "上次游戏,升序"),
		FileData::SortType(&compareLastPlayed, false, "上次游戏,降序"),

		FileData::SortType(&compareNumPlayers, true, "玩家数量,升序"),
		FileData::SortType(&compareNumPlayers, false, "玩家数量,降序"),

		FileData::SortType(&compareReleaseDate, true, "发布年份,升序"),
		FileData::SortType(&compareReleaseDate, false, "发布年份,降序"),

		FileData::SortType(&compareGenre, true, "类别,升序"),
		FileData::SortType(&compareGenre, false, "类别,降序"),

		FileData::SortType(&compareDeveloper, true, "研发厂商,升序"),
		FileData::SortType(&compareDeveloper, false, "研发厂商,降序"),

		FileData::SortType(&comparePublisher, true, "发行厂商,升序"),
		FileData::SortType(&comparePublisher, false, "发行厂商,降序"),

		FileData::SortType(&compareSystem, true, "系统,升序"),
		FileData::SortType(&compareSystem, false, "系统,降序")
	};

	const std::vector<FileData::SortType> SortTypes(typesArr, typesArr + sizeof(typesArr)/sizeof(typesArr[0]));

	//returns if file1 should come before file2
	bool compareName(const FileData* file1, const FileData* file2)
	{
		// we compare the actual metadata name, as collection files have the system appended which messes up the order
		std::string name1 = Utils::String::toUpper(file1->metadata.get("sortname"));
		std::string name2 = Utils::String::toUpper(file2->metadata.get("sortname"));
		if(name1.empty()){
			name1 = Utils::String::toUpper(file1->metadata.get("name"));
		}
		if(name2.empty()){
			name2 = Utils::String::toUpper(file2->metadata.get("name"));
		}
		return name1.compare(name2) < 0;
	}

	bool compareRating(const FileData* file1, const FileData* file2)
	{
		return file1->metadata.getFloat("rating") < file2->metadata.getFloat("rating");
	}

	bool compareTimesPlayed(const FileData* file1, const FileData* file2)
	{
		//only games have playcount metadata
		if(file1->metadata.getType() == GAME_METADATA && file2->metadata.getType() == GAME_METADATA)
		{
			return (file1)->metadata.getInt("playcount") < (file2)->metadata.getInt("playcount");
		}

		return false;
	}

	bool compareLastPlayed(const FileData* file1, const FileData* file2)
	{
		// since it's stored as an ISO string (YYYYMMDDTHHMMSS), we can compare as a string
		// as it's a lot faster than the time casts and then time comparisons
		return (file1)->metadata.get("lastplayed") < (file2)->metadata.get("lastplayed");
	}

	bool compareNumPlayers(const FileData* file1, const FileData* file2)
	{
		return (file1)->metadata.getInt("players") < (file2)->metadata.getInt("players");
	}

	bool compareReleaseDate(const FileData* file1, const FileData* file2)
	{
		// since it's stored as an ISO string (YYYYMMDDTHHMMSS), we can compare as a string
		// as it's a lot faster than the time casts and then time comparisons
		return (file1)->metadata.get("releasedate") < (file2)->metadata.get("releasedate");
	}

	bool compareGenre(const FileData* file1, const FileData* file2)
	{
		std::string genre1 = Utils::String::toUpper(file1->metadata.get("genre"));
		std::string genre2 = Utils::String::toUpper(file2->metadata.get("genre"));
		return genre1.compare(genre2) < 0;
	}

	bool compareDeveloper(const FileData* file1, const FileData* file2)
	{
		std::string developer1 = Utils::String::toUpper(file1->metadata.get("developer"));
		std::string developer2 = Utils::String::toUpper(file2->metadata.get("developer"));
		return developer1.compare(developer2) < 0;
	}

	bool comparePublisher(const FileData* file1, const FileData* file2)
	{
		std::string publisher1 = Utils::String::toUpper(file1->metadata.get("publisher"));
		std::string publisher2 = Utils::String::toUpper(file2->metadata.get("publisher"));
		return publisher1.compare(publisher2) < 0;
	}

	bool compareSystem(const FileData* file1, const FileData* file2)
	{
		std::string system1 = Utils::String::toUpper(file1->getSystemName());
		std::string system2 = Utils::String::toUpper(file2->getSystemName());
		return system1.compare(system2) < 0;
	}
};
