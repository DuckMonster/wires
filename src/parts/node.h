#pragma once
#include "part.h"

class Node : public Part
{
	PART_TYPE(Node, '*')

	void evaluate() override;
};