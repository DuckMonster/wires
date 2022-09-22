#pragma once
#include "part.h"

class Inverter : public Part
{
	PART_TYPE(Inverter, '>')

	void evaluate() override;
};