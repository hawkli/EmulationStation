#include "FileSorts.h"

#include "utils/StringUtil.h"

namespace FileSorts
{
	const FileData::SortType typesArr[] = {
		FileData::SortType(&compareName, true, u8"文件名,升序"),
		FileData::SortType(&compareName, false, u8"文件名,降序"),

		FileData::SortType(&compareRating, true, u8"评分,升序"),
		FileData::SortType(&compareRating, false, u8"评分,降序"),

		FileData::SortType(&compareTimesPlayed, true, u8"游戏次数,升序"),
		FileData::SortType(&compareTimesPlayed, false, u8"游戏次数,降序"),

		FileData::SortType(&compareLastPlayed, true, u8"上次游戏,升序"),
		FileData::SortType(&compareLastPlayed, false, u8"上次游戏,降序"),

		FileData::SortType(&compareNumPlayers, true, u8"玩家数量,升序"),
		FileData::SortType(&compareNumPlayers, false, u8"玩家数量,降序"),

		FileData::SortType(&compareReleaseDate, true, u8"发布年份,升序"),
		FileData::SortType(&compareReleaseDate, false, u8"发布年份,降序"),

		FileData::SortType(&compareGenre, true, u8"类别,升序"),
		FileData::SortType(&compareGenre, false, u8"类别,降序"),

		FileData::SortType(&compareDeveloper, true, u8"研发厂商,升序"),
		FileData::SortType(&compareDeveloper, false, u8"研发厂商,降序"),

		FileData::SortType(&comparePublisher, true, u8"发行厂商,升序"),
		FileData::SortType(&comparePublisher, false, u8"发行厂商,降序"),

		FileData::SortType(&compareSystem, true, u8"系统,升序"),
		FileData::SortType(&compareSystem, false, u8"系统,降序")
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
