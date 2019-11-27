#include "Event.h"

Event::Event()
{
}

Event::~Event()
{
	if (!_listaction.empty())
		_listaction.clear();
}

void Event::fireEvent(EventArg* e)
{
	if (_listaction.empty())
		return;
	for (auto action : _listaction)
		(*action)(e);
}

void Event::operator+=(EventFunction pFunction)
{
	_listaction.push_back(pFunction);
}

void Event::operator-=(EventFunction pFunction)
{
	auto it = find(_listaction.begin(), _listaction.end(), pFunction);
	if (it == _listaction.end())
		throw new exception("Element to remove not found");
	_listaction.erase(it);
}
