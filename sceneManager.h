#pragma once
#include "scene.h"

class SceneManager
{
public:
	SceneManager();
	~SceneManager();
	void update();
	void init();
	void close();
	Scene* getScene();

private:
	Scene* scene;
};