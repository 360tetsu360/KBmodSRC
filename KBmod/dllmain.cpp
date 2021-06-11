#include <Windows.h>
#include <string>
#include <vector>

#include "Mem.h"
#include "Logger.h"
#include "Hooks.h"
#include "framework.h"

HMODULE hMod = 0;
std::vector<std::string> split(std::string str, char del) {
    int first = 0;
    int last = str.find_first_of(del);

    std::vector<std::string> result;

    while (first < str.size()) {
        std::string subStr(str, first, last - first);

        result.push_back(subStr);

        first = last + 1;
        last = str.find_first_of(del, first);

        if (last == std::string::npos) {
            last = str.size();
        }
    }

    return result;
}

float StdStringToFloat(const std::string& x, bool* is_float) {
    char* not_float_x;
    const float result = std::strtof(x.c_str(), &not_float_x);
    const size_t length = std::strlen(not_float_x);
    if (length == 0) {
        if (result == HUGE_VALF || result == -HUGE_VALF) {
            *is_float = false;
            return result;
        }
        *is_float = true;
        return result;
    }

    bool are_all_space = true;
    for (size_t i = 0; i < length; ++i) {
        if (not_float_x[i] != ' ') {
            are_all_space = false;
            break;
        }
    }

    if (are_all_space) {
        *is_float = true;
        return result;
    }
    *is_float = false;
    return result;

}

void Command(TextHolder* cmd, bool* cancel) {
    std::string cmdstr = cmd->getText();
    if (cmdstr[0] == '.') {
        if (cmdstr[1] != ' ' && cmdstr.size() >= 2) {
            std::string _Commandstr(cmdstr, 1, cmdstr.size());
            std::vector<std::string> args = split(_Commandstr, ' ');
            if (args[0] == "wKb") {
                if (args.size() == 2) {
                    bool isFloat = false;
                    float result = StdStringToFloat(args[1], &isFloat);
                    if (isFloat) {
                        Hook::getHook()->setcunstomWRLDKB(result);
                    }
                    std::string resul = std::string("/tellraw @s {\"rawtext\":[{\"text\":\"World kb set to ")+std::to_string(result)+std::string("\"}]}");
                    cmd->setText(resul);
                }
                else if (args.size() == 3)
                {
                    bool isFloatx = false;
                    float resultx = StdStringToFloat(args[1], &isFloatx);
                    bool isFloaty = false;
                    float resulty = StdStringToFloat(args[2], &isFloaty);
                    if (isFloatx && isFloaty) {
                        Hook::getHook()->setcunstomWRLDKB(resultx, resulty);
                    }
                    std::string resul = std::string("/tellraw @s {\"rawtext\":[{\"text\":\"World kb set to ") + std::to_string(resultx) +std::string(" ") + std::to_string(resulty) + std::string("\"}]}");
                    cmd->setText(resul);
                }
            }
            if (args[0] == "delay") {
                if (args.size() == 2) {//Minecraft.Windows.exe + 1A281D7
                    uintptr_t Address = Mem::getMem()->BaseAddress + 0x1D4DAAB;
                    int num = atoi(args[1].c_str());
                    if (num <= 255) {
                        byte newdelay = num;
                        WriteProcessMemory(Mem::getMem()->hProcess, (BYTE*)Address, &newdelay, sizeof(newdelay), nullptr);
                        std::string resul = std::string("/tellraw @s {\"rawtext\":[{\"text\":\"request --OK\"}]}");
                        cmd->setText(resul);
                    }
                    else {
                        std::string resul = std::string("/tellraw @s {\"rawtext\":[{\"text\":\"request failed\"}]}");
                        cmd->setText(resul);
                    }
                }
            }
            else {
                std::string resul = std::string("/tellraw @s {\"rawtext\":[{\"text\":\"README.md!!!!\"}]}");
                cmd->setText(resul);
            }
        }

    }
}

int Loaded() {
    Hook::getHook()->Install();
    return 0;
}

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
        hMod = hModule;
        CreateThread(nullptr, 0, (LPTHREAD_START_ROUTINE)Loaded, hModule, 0, nullptr);
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}