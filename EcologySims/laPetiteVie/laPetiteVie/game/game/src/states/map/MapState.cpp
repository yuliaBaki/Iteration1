#include "MapState.h"

MapState::MapState(Game* parent):
	State(parent)
{
}

std::string MapState::getType()
{
	return "map";
}

MapState::~MapState()
{
	if (m_scene)
		delete m_scene;
}
