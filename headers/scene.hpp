#ifndef SCENE_H
#define SCENE_H

#include <vector>

#include "entity.hpp"

class Scene {
private:
	std::vector<Entity> entities;

public:
	void add(Entity& entity);
};

#endif