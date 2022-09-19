#include "node.h"
#include "inverter.h"
#include "circuit.h"

void Node::evaluate()
{
	circuit->mark_dirty_at(x + 1, y);

	Inverter* inv = circuit->find_inverter_at(x - 1, y);
	if (!inv)
	{
		state = false;
		return;
	}

	state = inv->state;
}