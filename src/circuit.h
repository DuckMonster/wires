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

	void draw_wire(Point from, Point to);

	template<typename T>
	T* add_part(Point point)
	{
		T* part = new T();
		part->circuit = this;
		part->position = point;
		part->state = false;

		parts.add(part);
		return part;
	}

	void delete_part(Part* part)
	{
		parts.remove(part);
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

	void mark_dirty(Part* part)
	{
		evaluation_queue.add(part);
	}

private:
	Array<Part*> parts;
	Array<Part*> evaluation_queue;
};