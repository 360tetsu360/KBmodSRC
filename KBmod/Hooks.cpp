#include <stdint.h>

#include "Hooks.h"
#include "framework.h"
#include "Logger.h"
#include "Mem.h"
#include "Math.h"
#include "TextHolder.h"
#include "framework.h"

Hook* instance;
vec2_t kbM;
vec2_t WRLDkbM;

Hook::Hook() {

}
Hook* Hook::getHook() {
	if (!instance) {
		instance = new Hook();
	}
	return instance;
}
float getAnglex(vec2_t origin, vec2_t target) {
	float xD = target.x - origin.x;
	float zD = target.y - origin.y;
	double radian = atan2(xD, zD);
	float degree = radian * 180 / PI;
	return -degree;
}
const char* tes = "48 89 5C 24 10 48 89 74 24 18 57 48 83 EC 60 48 8B D9 48 83 B9 A8 0A";
const char* te = "48 89 5C 24 18 55 56 57 41 54 41 55 41 56 41 57 48 8D AC 24 E0 FE FF FF 48 81 EC 20 02 00 00 48 8B 05 DA 97 E8 02 48 33";
uintptr_t kbFuncAddr = 0;
typedef void(__fastcall* WRLDKnock)(int64_t* arg1, float arg2, int arg3, int32_t arg4, int32_t arg5, float arg6, int32_t arg7);
WRLDKnock _WRLDKnock;
void WRLDKnockCallback(int64_t* arg1, float arg2, int arg3, float arg4, float arg5, float arg6, float arg7) {
	int velocityOffset = *reinterpret_cast<int*>(kbFuncAddr + 0xF7);
	vec3_t* entvelocity = reinterpret_cast<vec3_t*>((uintptr_t)arg1 + velocityOffset);
	vec2_t testkb = vec2_t(arg4, -arg5);
	float dig = getAnglex(vec2_t(0, 0), testkb);
	double  radian = dig * PI / 180;

	float radius = WRLDkbM.x;

	entvelocity->x = sin(radian) * radius;
	entvelocity->y = WRLDkbM.y;
	entvelocity->z = cos(radian) * radius;
}
typedef void(__fastcall* ChatHook)(void*, void*);
ChatHook _ChatHook;
void ChatCallback(void* a1, void* a2) {
	//Logger::getLogger()->Write(a1, true);
	bool cancelSend = false;
	TextHolder* messageHolder = reinterpret_cast<TextHolder*>(a2);
	//Command(messageHolder, &cancelSend);
	if (!cancelSend) _ChatHook(a1, messageHolder);
}
typedef void(__fastcall* CCHook)(void*);
CCHook _CCHook;
void CCCallback(void* arg1) {
	bool cancelSend = false;
	uintptr_t rdi_1 = (uintptr_t)arg1 + 0xa98;
	uintptr_t rdx_3 = rdi_1;
	TextHolder* mbCmd = reinterpret_cast<TextHolder*>(rdx_3);
	Command(mbCmd, &cancelSend);
	//Logger::getLogger()->WriteLine(mbCmd->getText());
	if (!cancelSend) _CCHook(arg1);
}

void Hook::Install() {
	MH_Initialize();
	uintptr_t sigAddr = Mem::getMem()->findSig(te);
	if (sigAddr) {
		if (MH_CreateHook((void*)sigAddr, &ChatCallback, reinterpret_cast<LPVOID*>(&_ChatHook)) == MH_OK) {
			MH_EnableHook((void*)sigAddr);
		}
		else {
		}
	}
	else {
	}
	sigAddr = Mem::getMem()->findSig(tes);
	Logger::getLogger()->Write(sigAddr,true);
	if (sigAddr) {
		if (MH_CreateHook((void*)sigAddr, &CCCallback, reinterpret_cast<LPVOID*>(&_CCHook)) == MH_OK) {
			MH_EnableHook((void*)sigAddr);
		}
		else {
		}
	}
	else {
	}
	kbFuncAddr = Mem::getMem()->BaseAddress + 0x1D4E1B0;
	if (kbFuncAddr) {
		if (MH_CreateHook((void*)kbFuncAddr, &WRLDKnockCallback, reinterpret_cast<LPVOID*>(&_WRLDKnock)) == MH_OK) {
			MH_EnableHook((void*)kbFuncAddr);
		}
		else {
		}
	}
	kbM = vec2_t(1, 1);
	WRLDkbM = vec2_t(0.4f, 0.5f);
}
void Hook::Disable() {
	uintptr_t sigAddr2 = Mem::getMem()->BaseAddress + 0x1A27280;
	MH_RemoveHook((LPVOID)sigAddr2);
	MH_Uninitialize();
}
void Hook::setcunstomWRLDKB(float value) {
	WRLDkbM.x = value;
	WRLDkbM.y = value;
}
void Hook::setcunstomWRLDKB(float valuex, float valuey) {
	WRLDkbM.x = valuex;
	WRLDkbM.y = valuey;
}