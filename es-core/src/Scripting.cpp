#include "Scripting.h"
#include "Log.h"
#include "platform.h"
#include "utils/FileSystemUtil.h"

namespace Scripting
{
	void fireEvent(const std::string& eventName)
	{
		fireEvent(eventName, "", "");
	}

	void fireEvent(const std::string& eventName, const std::string& arg)
	{
		fireEvent(eventName, arg, "");
	}

	void fireEvent(const std::string& eventName, const std::string& arg1, const std::string& arg2)
	{
		LOG(LogDebug) << "fireEvent: " << eventName << " " << arg1 << " " << arg2;

		const std::string path = Utils::FileSystem::getHomePath() + "/.emulationstation/scripts/" + eventName;

		if(!Utils::FileSystem::exists(path))
			return;

		std::list<std::string> scripts = Utils::FileSystem::getDirContent(path);

		for(std::list<std::string>::const_iterator it = scripts.cbegin(); it != scripts.cend(); ++it) {
			// append folder to path
			std::string script = *it + " " + arg1 + " " + arg2;
			LOG(LogDebug) << "  executing: " << script;
			runSystemCommand(script);
		}
	}

} // Scripting::