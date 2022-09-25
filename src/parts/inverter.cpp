#include "inverter.h"
#include "circuit.h"

void Inverter::evaluate()
{
	circuit->mark_dirty_at(position.right());
	Part* host_part = circuit->find_part_at(position.left());

	// No host part, default to on
	if (host_part == nullptr)
	{
		state = true;
		return;
	}

	state = !host_part->state;
}