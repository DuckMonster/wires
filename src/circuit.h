#pragma once
#include "array.h"
#include "parts/node.h"
#include "parts/inverter.h"
#include "parts/connection.h"

class Circuit
{
public:
	void step();
	void draw();

	void draw_wire(i32 x1, i32 y1, i32 x2, i32 y2);

	Node* add_node(i32 x, i32 y)
	{
		Node* node = &nodes.add_default();
		init_part(node, x, y);

		return node;
	}
	Node* find_node_at(i32 x, i32 y)
	{
		for(Node& node : nodes)
		{
			if (node.x == x && node.y == y)
				return &node;
		}

		return nullptr;
	}

	void connect_nodes(Node* a, Node* b)
	{
		Connection* connection = &connections.add_default();
		connection->a = a;
		connection->b = b;
	}

	Inverter* add_inverter(i32 x, i32 y)
	{
		Inverter* inv = &inverters.add_default();
		init_part(inv, x, y);

		return inv;
	}
	Inverter* find_inverter_at(i32 x, i32 y)
	{
		for(Inverter& inv : inverters)
		{
			if (inv.x == x && inv.y == y)
				return &inv;
		}

		return nullptr;
	}

	Part* find_part_at(i32 x, i32 y)
	{
		for(Node& node : nodes)
		{
			if (node.x == x && node.y == y)
				return &node;
		}

		for(Inverter& inv : inverters)
		{
			if (inv.x == x && inv.y == y)
				return &inv;
		}

		return nullptr;
	}
	bool is_occupied(i32 x, i32 y) { return find_part_at(x, y) != nullptr; }

	void mark_dirty(Part* part)
	{
		evaluation_queue.add(part);
	}
	void mark_dirty_at(i32 x, i32 y)
	{
		Part* part = find_part_at(x, y);
		if (!part)
			return;

		evaluation_queue.add(part);
	}

private:
	void init_part(Part* part, i32 x, i32 y)
	{
		part->circuit = this;
		part->x = x;
		part->y = y;
		part->state = false;

		mark_dirty(part);
	}

	Array<Node> nodes;
	Array<Connection> connections;
	Array<Inverter> inverters;

	Array<Part*> evaluation_queue;
};