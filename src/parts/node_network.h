#pragma once
#include "node.h"

class Circuit;

class Node_Network
{
public:
	Circuit* circuit;

	struct Connection
	{
		Node* a = nullptr;
		Node* b = nullptr;

		Connection() {}
		Connection(Node* a, Node* b) : a(a), b(b) {}

		bool contains(Node* node) { return a == node || b == node; }
		Node* other(Node* first)
		{
			if (first == a)
				return b;
			if (first == b)
				return a;

			return nullptr;
		}

		bool operator==(const Connection& rhs) const
		{
			return (a == rhs.a && b == rhs.b) ||
				(a == rhs.b && b == rhs.a);
		}
		bool operator!=(const Connection& rhs) const { return !(*this == rhs); }
	};

	void add_node(Node* node);
	void add_connection(Node* src, Node* new_node);
	void remove_node(Node* node);

	void merge_with(Node_Network* other);
	void split_net();
	void gather_net(Node* root, Array<Node*>& out_nodes, Array<Connection>& out_connections);

	void evaluate();

	Array<Node*> nodes;
	Array<Connection> connections;

	bool state = false;
};