#pragma once
class Circuit;

class Part
{
public:
	Circuit* circuit;
	i32 x;
	i32 y;

	bool state;

	virtual void evaluate() { }
};