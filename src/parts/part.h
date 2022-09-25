#pragma once
class Circuit;

struct Part_Type
{
	Part_Type(const char* name) : name(name) {}
	const char* name;
};

#define PART_TYPE(cls, chr)\
public:\
static Part_Type* static_type()\
{\
	static Part_Type type = Part_Type(#cls);\
	return &type;\
}\
cls() : Part(static_type(), chr) {}\
private:

class Part
{
public:
	Part(Part_Type* type, char chr) : type(type), chr(chr) {}

	Part_Type* type;
	char chr;

	Circuit* circuit;
	Point position;

	bool state;

	virtual void on_create() {}
	virtual void on_delete() {}
	virtual void evaluate() { }
};