#include "StateManager.h"

namespace RessourceManager {
	void StateManager::removeUnused()
	{
		for (auto i = p_states.begin(); i != p_states.end();)
			if (i->second.use_count() == 1)
				i = p_states.erase(i);
			else
				++i;
	}

	std::unordered_map<std::string, std::shared_ptr<State>> StateManager::p_states;
}
