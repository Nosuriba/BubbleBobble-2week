#pragma once

#include <memory>
#include "../Input.h"

class Scene
{
public:
	Scene();
	virtual ~Scene();
	
	virtual void Init() = 0;
	virtual void Update(const Input& p) = 0;
};

