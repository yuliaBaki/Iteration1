#include "State.h"
#include "../util/helper.h"

State::State(Game* parent):
	m_parent(parent)
{
	id = pointer_to_string(this);
}

std::string State::getId()
{
	return id;
}
