#include "MetaData.h"

#include "utils/FileSystemUtil.h"
#include "Log.h"
#include <pugixml/src/pugixml.hpp>

MetaDataDecl gameDecls[] = {
	// key,         type,                   default,            statistic,  name in GuiMetaDataEd,  prompt in GuiMetaDataEd
	{"name",        MD_STRING,              "",                 false,      "名　　称",                "请输入游戏名称"},
	{"sortname",    MD_STRING,              "",                 false,      "排序名称",                "请输入排序名称"},
	{"desc",        MD_MULTILINE_STRING,    "",                 false,      "详细描述",                "请输入详细描述"},
	{"image",       MD_PATH,                "",                 false,      "图　　片",                "请输入图片路径"},
	{"video",       MD_PATH     ,           "",                 false,      "视　　频",                "请输入视频路径"},
	{"marquee",     MD_PATH,                "",                 false,      "文字滚动",                "请输入文字路径"},
	{"thumbnail",   MD_PATH,                "",                 false,      "小缩略图",                "请输入缩略图路径"},
	{"rating",      MD_RATING,              "0.000000",         false,      "评　　分",                "请输入评分"},
	{"releasedate", MD_DATE,                "not-a-date-time",  false,      "发行年份",                "请输入发行年份"},
	{"developer",   MD_STRING,              "unknown",          false,      "研发厂商",                "请输入研发厂商名称"},
	{"publisher",   MD_STRING,              "unknown",          false,      "发行厂商",                "请输入发行厂商名称"},
	{"genre",       MD_STRING,              "unknown",          false,      "类　　别",                "请输入游戏类别"},
	{"players",     MD_INT,                 "1",                false,      "玩家数量",                "请输入可支持的玩家数量"},
	{"favorite",    MD_BOOL,                "false",            false,      "喜　　欢",                "请选择是否喜欢 关/开"},
	{"hidden",      MD_BOOL,                "false",            false,      "隐　　藏",                "请选择是否隐藏 关/开" },
	{"kidgame",     MD_BOOL,                "false",            false,      "儿童游戏",                "请选择是否为儿童游戏 关/开" },
	{"playcount",   MD_INT,                 "0",                true,       "游戏次数",                "请输入已经游戏次数"},
	{"lastplayed",  MD_TIME,                "0",                true,       "上次游戏",                "请输入最后一次游戏时间"}
};
const std::vector<MetaDataDecl> gameMDD(gameDecls, gameDecls + sizeof(gameDecls) / sizeof(gameDecls[0]));

MetaDataDecl folderDecls[] = {
	{"name",        MD_STRING,              "",                 false,      "名　　称",                "请输入游戏名称"},
	{"sortname",    MD_STRING,              "",                 false,      "排序名称",                "请输入排序名称"},
	{"desc",        MD_MULTILINE_STRING,    "",                 false,      "详细描述",                "请输入详细描述"},
	{"image",       MD_PATH,                "",                 false,      "图　　片",                "请输入图片路径"},
	{"thumbnail",   MD_PATH,                "",                 false,      "小缩略图",                "请输入缩略图路径"},
	{"video",       MD_PATH     ,           "",                 false,      "视　　频",                "请输入视频路径"},
	{"marquee",     MD_PATH,                "",                 false,      "文字滚动",                "请输入文字路径"},
	{"rating",      MD_RATING,              "0.000000",         false,      "评　　分",                "请输入评分"},
	{"releasedate", MD_DATE,                "not-a-date-time",  false,      "发行年份",                "请输入发行年份"},
	{"developer",   MD_STRING,              "unknown",          false,      "研发厂商",                "请输入研发厂商名称"},
	{"publisher",   MD_STRING,              "unknown",          false,      "发行厂商",                "请输入发行厂商名称"},
	{"genre",       MD_STRING,              "unknown",          false,      "类　　别",                "请输入游戏类别"},
	{"players",     MD_INT,                 "1",                false,      "玩家数量",                "请输入可支持的玩家数量"}
};
const std::vector<MetaDataDecl> folderMDD(folderDecls, folderDecls + sizeof(folderDecls) / sizeof(folderDecls[0]));

const std::vector<MetaDataDecl>& getMDDByType(MetaDataListType type)
{
	switch(type)
	{
	case GAME_METADATA:
		return gameMDD;
	case FOLDER_METADATA:
		return folderMDD;
	}

	LOG(LogError) << "Invalid MDD type";
	return gameMDD;
}



MetaDataList::MetaDataList(MetaDataListType type)
	: mType(type), mWasChanged(false)
{
	const std::vector<MetaDataDecl>& mdd = getMDD();
	for(auto iter = mdd.cbegin(); iter != mdd.cend(); iter++)
		set(iter->key, iter->defaultValue);
}


MetaDataList MetaDataList::createFromXML(MetaDataListType type, pugi::xml_node& node, const std::string& relativeTo)
{
	MetaDataList mdl(type);

	const std::vector<MetaDataDecl>& mdd = mdl.getMDD();

	for(auto iter = mdd.cbegin(); iter != mdd.cend(); iter++)
	{
		pugi::xml_node md = node.child(iter->key.c_str());
		if(md)
		{
			// if it's a path, resolve relative paths
			std::string value = md.text().get();
			if (iter->type == MD_PATH)
			{
				value = Utils::FileSystem::resolveRelativePath(value, relativeTo, true);
			}
			mdl.set(iter->key, value);
		}else{
			mdl.set(iter->key, iter->defaultValue);
		}
	}

	return mdl;
}

void MetaDataList::appendToXML(pugi::xml_node& parent, bool ignoreDefaults, const std::string& relativeTo) const
{
	const std::vector<MetaDataDecl>& mdd = getMDD();

	for(auto mddIter = mdd.cbegin(); mddIter != mdd.cend(); mddIter++)
	{
		auto mapIter = mMap.find(mddIter->key);
		if(mapIter != mMap.cend())
		{
			// we have this value!
			// if it's just the default (and we ignore defaults), don't write it
			if(ignoreDefaults && mapIter->second == mddIter->defaultValue)
				continue;
			
			// try and make paths relative if we can
			std::string value = mapIter->second;
			if (mddIter->type == MD_PATH)
				value = Utils::FileSystem::createRelativePath(value, relativeTo, true);

			parent.append_child(mapIter->first.c_str()).text().set(value.c_str());
		}
	}
}

void MetaDataList::set(const std::string& key, const std::string& value)
{
	mMap[key] = value;
	mWasChanged = true;
}

const std::string& MetaDataList::get(const std::string& key) const
{
	return mMap.at(key);
}

int MetaDataList::getInt(const std::string& key) const
{
	return atoi(get(key).c_str());
}

float MetaDataList::getFloat(const std::string& key) const
{
	return (float)atof(get(key).c_str());
}

bool MetaDataList::isDefault()
{
	const std::vector<MetaDataDecl>& mdd = getMDD();

	for (unsigned int i = 1; i < mMap.size(); i++) {
		if (mMap.at(mdd[i].key) != mdd[i].defaultValue) return false;
	}

	return true;
}

bool MetaDataList::wasChanged() const
{
	return mWasChanged;
}

void MetaDataList::resetChangedFlag()
{
	mWasChanged = false;
}
