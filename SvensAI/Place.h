#pragma once
#include <string>

class Place
{
public:
	enum Type {
		Restaurant = 0,
		Work,
		Home,
		Toilet,
		Crossroad
	};

	Place(Type type);

	Type m_type;
};

