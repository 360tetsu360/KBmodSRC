#include "Logger.h"

Logger* instance;

Logger::Logger() {

}

auto Logger::getLogger() -> Logger* {
    if (!instance) {
        instance = new Logger();
    }
    return instance;
}

std::string Logger::GetPath() {
    TCHAR usernamed[UNLEN + 100];
    DWORD size = UNLEN + 100;
    GetUserName((TCHAR*)usernamed, &size);
    char dst[DST_LEN];
    ZeroMemory(&dst[0], DST_LEN);
    WideCharToMultiByte(CP_ACP, 0, &usernamed[0], lstrlen(usernamed), &dst[0], DST_LEN, NULL, NULL);
    char* username = (char*)dst;
    char* path1 = (char*)"C:\\Users\\";
    const char* path2 = "\\AppData\\Local\\Packages\\Microsoft.MinecraftUWP_8wekyb3d8bbwe\\RoamingState\\";
    std::string path = (std::string)path1 + (std::string)(char*)username + (std::string)path2;
    return path;
}

void Logger::Write(const std::string& toWrite) {

    std::stringstream ss;
    ss << Logger::GetPath() << "AimBot.txt";

    std::string debugFilePath = ss.str();
    std::ofstream fileStream(debugFilePath, std::ios::out | std::ios::app);
    fileStream << toWrite;
    fileStream.close();
}
void Logger::Write(uintptr_t toWrite, bool hex) {
    std::stringstream ss;
    if (hex) {
        ss << std::hex << toWrite;
    }
    else {
        ss << toWrite;
    }
    this->Write(ss.str() + "\n");
}
void Logger::Write(float toWrite) {
    this->Write(std::to_string(toWrite));

}
void Logger::Write(void* toWrite, bool hex) {
    this->Write((uintptr_t)toWrite, hex);
}

void Logger::WriteLine(const std::string& toWrite) {
    this->Write(toWrite + "\n");
}
void Logger::WriteLine() {
    this->WriteLine("");
}
