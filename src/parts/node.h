#pragma once
#include "part.h"
#include "array.h"

class Node_Network;

class Node : public Part
{
	PART_TYPE(Node, '*')

public:
	void on_create() override;
	void on_delete() override;
	void evaluate() override;

	Node_Network* network;
};