#pragma once
#include "GameObject.h"
#include "Rect.h"
#define SCREEN_WIDTH 320

class Quadtree
{
private:
	int            m_level;
	Rect*          m_region;
	vector<LPGAMEOBJECT> m_objects_list;
	Quadtree**     m_nodes;

	bool           IsContain(CGameObject* entity);
	void           Split();
public:
	Quadtree();
	Quadtree(int level, Rect* region) {
		m_level = level;
		m_region = region;
	};
	~Quadtree();

	void        Clear();
	void        Insert(CGameObject* entity);
	void		Retrieve(vector<LPGAMEOBJECT> return_objects_list, CGameObject* entity);
};

