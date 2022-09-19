#include "inverter.h"
#include "circuit.h"

void Inverter::evaluate()
{
	circuit->mark_dirty_at(x + 1, y);
	Part* host_part = circuit->find_part_at(x - 1, y);

	// No host part, default to on
	if (host_part == nullptr)
	{
		state = true;
		return;
	}

	state = !host_part->state;
	circuit->mark_dirty_at(x + 1, y);
}