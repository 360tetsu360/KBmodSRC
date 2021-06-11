#pragma once
#include <Windows.h>
#include <string>
#include <vector>
#include <TlHelp32.h>
#include <Psapi.h>
#define INRANGE(x,a,b)	(x >= a && x <= b)
#define getBits( x )	(INRANGE((x&(~0x20)),'A','F') ? ((x&(~0x20)) - 'A' + 0xa) : (INRANGE(x,'0','9') ? x - '0' : 0))
#define getByte( x )	(getBits(x[0]) << 4 | getBits(x[1]))

class Mem
{
	Mem();
public:
	static Mem* getMem();
	uintptr_t BaseAddress;
	HANDLE hProcess;
	uintptr_t GetProcId(const wchar_t* procname);
	uintptr_t GetModuleBaseAddress(DWORD procId, const wchar_t* modName);
	uintptr_t FindAddress(HANDLE hProc, uintptr_t ptr, std::vector<unsigned int> offsets);
	uintptr_t findSig(long long rangeStart, long long rangeEnd, const char* pattern);
	uintptr_t findSig(const char* pattern);
	long long getModuleBase();
	HMODULE getModuleBaseHandle();
	long long getBaseModuleSize();
	long long getBaseModuleEnd();
	uintptr_t Read(std::uintptr_t ptrAddress) const;

private:

};