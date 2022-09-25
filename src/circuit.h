#pragma once
#include "array.h"
#include "parts/node.h"
#include "parts/inverter.h"
#include "parts/connection.h"
#include "parts/node_network.h"

class Circuit
{
public:
	void step();
	void draw();

	void draw_wire(Point from, Point to, u32 color);

	template<typename T>
	T* add_part(Point point)
	{
		T* part = new T();
		part->circuit = this;
		part->position = point;
		part->state = false;

		part->on_create();

		parts.add(part);
		mark_dirty(part);
		return part;
	}

	void delete_part(Part* part)
	{
		parts.remove(part);
		mark_dirty_at(part->position.right());

		part->on_delete();
		delete part;
	}

	Part* find_part_at(Point point)
	{
		for(Part* part : parts)
		{
			if (part->position == point)
				return part;
		}

		return nullptr;
	}
	template<typename T>
	T* find_part_at(Point point)
	{
		Part_Type* type = T::static_type();

		for(Part* part : parts)
		{
			if (part->type != type)
				continue;

			if (part->position == point)
				return (T*)part;
		}

		return nullptr;
	}

	void mark_dirty_at(Point point)
	{
		Part* part = find_part_at(point);
		if (part)
			mark_dirty(part);
	}
	void mark_dirty(Part* part)
	{
		evaluation_queue.add(part);
	}

	Node_Network* add_network()
	{
		Node_Network* network = new Node_Network();
		network->circuit = this;

		networks.add(network);
		return network;
	}
	void delete_network(Node_Network* network)
	{
		networks.remove(network);
		delete network;
	}

	Array<Part*> parts;
	Array<Part*> evaluation_queue;

	Array<Node_Network*> networks;
};