#include "node_network.h"
#include "inverter.h"
#include "circuit.h"

void Node_Network::add_node(Node* node)
{
	nodes.add(node);
	node->network = this;
}

void Node_Network::add_connection(Node* src, Node* new_node)
{
	if (new_node->network)
	{
		if (new_node->network != this)
			merge_with(new_node->network);

		connections.add(Connection(src, new_node));
	}
	else
	{
		new_node->network = this;

		nodes.add(new_node);
		connections.add(Connection(src, new_node));
	}
}

void Node_Network::remove_node(Node* node)
{
	nodes.remove_swap(node);

	for(i32 i = connections.count() - 1; i >= 0; --i)
	{
		if (connections[i].contains(node))
			connections.remove_at_swap(i);
	}

	if (nodes.count() == 0)
	{
		circuit->delete_network(this);
		return;
	}

	split_net();
}

void Node_Network::merge_with(Node_Network* other)
{
	for(auto* node : other->nodes)
		node->network = this;

	nodes.append(other->nodes);
	connections.append(other->connections);

	circuit->delete_network(other);
}

void Node_Network::split_net()
{
	Array<Node*> net_nodes;
	Array<Connection> net_connections;

	while(true)
	{
		net_nodes.empty();
		net_connections.empty();

		circuit->mark_dirty(nodes[0]);
		gather_net(nodes[0], net_nodes, net_connections);
		if (net_nodes.count() == nodes.count())
			break;

		Node_Network* new_network = circuit->add_network();
		new_network->nodes = net_nodes;
		new_network->connections = net_connections;

		for(Node* node : net_nodes)
		{
			node->network = new_network;
			nodes.remove_swap(node);
		}
		for(Connection& connection : net_connections)
		{
			connections.remove_swap(connection);
		}
	}
}

void Node_Network::gather_net(Node* root, Array<Node*>& out_nodes, Array<Connection>& out_connections)
{
	if (out_nodes.contains(root))
		return;

	out_nodes.add(root);
	for(auto& connection : connections)
	{
		if (connection.contains(root))
		{
			out_connections.add(connection);
			gather_net(connection.other(root), out_nodes, out_connections);
		}
	}
}

void Node_Network::evaluate()
{
	bool has_active_node = false;
	for(Node* node : nodes)
	{
		Inverter* inv = circuit->find_part_at<Inverter>(node->position.left());
		if (!inv)
			continue;

		if (inv->state)
		{
			has_active_node = true;
			break;
		}
	}

	state = has_active_node;
	for(Node* node : nodes)
	{
		node->state = state;
		circuit->mark_dirty_at(node->position.right());
	}
}