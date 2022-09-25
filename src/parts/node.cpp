#include "node.h"
#include "inverter.h"
#include "circuit.h"
#include <stdio.h>

void Node::on_create()
{
	network = circuit->add_network();
	network->add_node(this);
}

void Node::on_delete()
{
	network->remove_node(this);
}

void Node::evaluate()
{
	network->evaluate();
}