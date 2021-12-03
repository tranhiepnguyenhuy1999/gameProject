#include "Quadtree.h"

Quadtree::Quadtree()
{
}


Quadtree::~Quadtree()
{
}

void Quadtree::Clear()
{
	// Clear all nodes
	if (m_nodes)
	{
		for (int i = 0; i < 4; i++)
		{
			m_nodes[i]->Clear();
			delete m_nodes[i];
		}
		delete[] m_nodes;
	}

	// Clear current Quadtree
	m_objects_list.clear();
	delete m_region;
}
bool Quadtree::IsContain(CGameObject* entity)
{
	float l, t, r, b;

	entity->GetBoundingBox(l, t, r, b);

	return !(r < m_region->X ||
		t < m_region->Y ||
		l > m_region->X + m_region->Width ||
		b > m_region->Y + m_region->Height);
}
void Quadtree::Split()
{
	m_nodes = new Quadtree*[4];

	m_nodes[0] = new Quadtree(m_level + 1,
		new Rect(m_region->X, m_region->Y, m_region->Width / 2, m_region->Height / 2));
	m_nodes[1] = new Quadtree(m_level + 1,
		new Rect(m_region->X + m_region->Width / 2, m_region->Y, m_region->Width / 2, m_region->Height / 2));
	m_nodes[2] = new Quadtree(m_level + 1,
		new Rect(m_region->X, m_region->Y + m_region->Height / 2, m_region->Width / 2, m_region->Height / 2));
	m_nodes[3] = new Quadtree(m_level + 1,
		new Rect(m_region->X + m_region->Width / 2, m_region->Y + m_region->Height / 2, m_region->Width / 2, m_region->Height / 2));
}

void Quadtree::Insert(CGameObject* entity)
{
	// Insert entity into corresponding nodes
	if (m_nodes)
	{
		if (m_nodes[0]->IsContain(entity))
			m_nodes[0]->Insert(entity);
		if (m_nodes[1]->IsContain(entity))
			m_nodes[1]->Insert(entity);
		if (m_nodes[2]->IsContain(entity))
			m_nodes[2]->Insert(entity);
		if (m_nodes[3]->IsContain(entity))
			m_nodes[3]->Insert(entity);

		return; // Return here to ignore rest.
	}

	// Insert entity into current quadtree
	if (this->IsContain(entity))
		m_objects_list.push_back(entity);

	// Split and move all objects in list into it’s corresponding nodes
	if (m_region->Width > SCREEN_WIDTH)
	{
		Split();

		while (!m_objects_list.empty())
		{
			if (m_nodes[0]->IsContain(m_objects_list.back()))
				m_nodes[0]->Insert(m_objects_list.back());
			if (m_nodes[1]->IsContain(m_objects_list.back()))
				m_nodes[1]->Insert(m_objects_list.back());
			if (m_nodes[2]->IsContain(m_objects_list.back()))
				m_nodes[2]->Insert(m_objects_list.back());
			if (m_nodes[3]->IsContain(m_objects_list.back()))
				m_nodes[3]->Insert(m_objects_list.back());

			// delete object from parent Quadtree
			m_objects_list.pop_back();
		}
	}
}
void Quadtree::Retrieve(vector<LPGAMEOBJECT> return_objects_list, CGameObject* entity)
{
	if (m_nodes)
	{
		if (m_nodes[0]->IsContain(entity))
			m_nodes[0]->Retrieve(return_objects_list, entity);
		if (m_nodes[1]->IsContain(entity))
			m_nodes[1]->Retrieve(return_objects_list, entity);
		if (m_nodes[2]->IsContain(entity))
			m_nodes[2]->Retrieve(return_objects_list, entity);
		if (m_nodes[3]->IsContain(entity))
			m_nodes[3]->Retrieve(return_objects_list, entity);

		return; // Return here to ignore rest.
	}

	// Add all entities in current region into return_objects_list
	if (this->IsContain(entity))
	{
		for (auto i = m_objects_list.begin(); i != m_objects_list.end(); i++)
		{
			if (entity != *i)
				return_objects_list.push_back(*i);
		}
	}
}