#pragma once
#include <Windows.h>
#include <string>
#include <fstream>
#include <sstream>
#include <Lmcons.h>

#define DST_LEN (256)

class Logger
{
	Logger();
public:
	static Logger* getLogger();
	std::string GetPath();
	void Write(const std::string& toWrite);
	void Write(uintptr_t toWrite, bool hex);
	void Write(float toWrite);
	void Write(void* toWrite, bool hex);

	void WriteLine(const std::string& toWrite);
	void WriteLine();
private:

};
