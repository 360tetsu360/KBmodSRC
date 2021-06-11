#include "Hooks.h"
#include "framework.h"
#include "Logger.h"
#include <stdint.h>
#include "Mem.h"
#include "Math.h"

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

typedef void(__fastcall* WRLDKnock)(int64_t* arg1, float arg2, int arg3, int32_t arg4, int32_t arg5, float arg6, int32_t arg7);
WRLDKnock _WRLDKnock;
void WRLDKnockCallback(int64_t* arg1, float arg2, int arg3, float arg4, float arg5, float arg6, float arg7) {

}


void Hook::Install() {
	MH_Initialize();
	uintptr_t sigAddr = Mem::getMem()->BaseAddress + 0x1A28270;
	if (sigAddr) {
		if (MH_CreateHook((void*)sigAddr, &WRLDKnockCallback, reinterpret_cast<LPVOID*>(&_WRLDKnock)) == MH_OK) {
			MH_EnableHook((void*)sigAddr);
		}
		else {
		}
	}
	kbM = vec2_t(1, 1);
	WRLDkbM = vec2_t(0.5f, 0.35f);
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