#pragma once
#include <map>

#include "MinHook.h"
#include "Memory.h"

class Hook
{
	Hook();
public:
	typedef void(WINAPI* AVKeyItem)(uint64_t key, bool isDown);
	static Hook* getHook();
	std::map<uint64_t, bool> keys;
	std::map<uint64_t, bool> Mouses;
	void Install();
	void Disable();
	void setcunstomWRLDKB(float value);
	void setcunstomWRLDKB(float valuex, float valuey);
private:

};
