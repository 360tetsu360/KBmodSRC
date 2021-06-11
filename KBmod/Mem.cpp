#include "Mem.h"

Mem* instance;

Mem::Mem() {
    DWORD procId = this->GetProcId(L"Minecraft.Windows.exe");
    this->BaseAddress = this->GetModuleBaseAddress(procId, L"Minecraft.Windows.exe");
    if (BaseAddress != NULL) {
        this->hProcess = OpenProcess(PROCESS_ALL_ACCESS, NULL, procId);
    }
}

Mem* Mem::getMem() {
    if (!instance) {
        instance = new Mem();
    }
    return instance;
}

uintptr_t Mem::GetProcId(const wchar_t* procname) {
    return GetCurrentProcessId();
}
uintptr_t Mem::GetModuleBaseAddress(DWORD procId, const wchar_t* modName) {
    uintptr_t modBaseAddr = 0;
    HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE | TH32CS_SNAPMODULE32, procId);
    if (hSnap != INVALID_HANDLE_VALUE)
    {
        MODULEENTRY32 modEntry;
        modEntry.dwSize = sizeof(modEntry);
        if (Module32First(hSnap, &modEntry))
        {
            do
            {
                if (!_wcsicmp(modEntry.szModule, modName))
                {
                    modBaseAddr = (uintptr_t)modEntry.modBaseAddr;
                    break;
                }
            } while (Module32Next(hSnap, &modEntry));
        }
    }
    CloseHandle(hSnap);
    return modBaseAddr;
}
uintptr_t Mem::FindAddress(HANDLE hProc, uintptr_t ptr, std::vector<unsigned int> offsets) {
    uintptr_t addr = ptr;
    for (unsigned int i = 0; i < offsets.size(); ++i)
    {
        ReadProcessMemory(hProc, (BYTE*)addr, &addr, sizeof(addr), 0);
        addr += offsets[i];
    }
    return addr;
}
uintptr_t Mem::findSig(long long rangeStart, long long rangeEnd, const char* pattern) {
    const char* pat = pattern;
    long long firstMatch = 0;
    for (long long pCur = rangeStart; pCur < rangeEnd; pCur++) {
        if (!*pat) return firstMatch;
        if (*(PBYTE)pat == '\?' || *(BYTE*)pCur == getByte(pat)) {
            if (!firstMatch) firstMatch = pCur;
            if (!pat[2]) return firstMatch;
            if (*(PWORD)pat == '\?\?' || *(PBYTE)pat != '\?') pat += 3;
            else pat += 2;
        }
        else {
            pat = pattern;
            firstMatch = 0;
        }
    }
    return 0;
}
uintptr_t Mem::findSig(const char* pattern) {
    return findSig(getModuleBase(), getBaseModuleEnd(), pattern);
}
long long Mem::getModuleBase() {
    return (long long)GetModuleHandleA(nullptr);
}
HMODULE Mem::getModuleBaseHandle() {
    return GetModuleHandleA(nullptr);
}
long long Mem::getBaseModuleSize() {
    MODULEINFO info;
    GetModuleInformation(GetCurrentProcess(), getModuleBaseHandle(), &info, sizeof(info));
    return info.SizeOfImage;
}
long long Mem::getBaseModuleEnd() {
    return getModuleBase() + getBaseModuleSize();
}
uintptr_t Mem::Read(std::uintptr_t ptrAddress) const {
    uintptr_t ret = 0;
    if (ReadProcessMemory(hProcess, (BYTE*)ptrAddress, &ret, sizeof(uintptr_t), nullptr)) {
        return ret;
    }
    else {
        return 0;
    }
}
