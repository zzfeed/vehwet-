#include "pch.h"
#include "htdModule.h"

void htdModule::HideDll(HMODULE _hMod)
{
    PPEB _peb;
    _asm
    {
        mov eax, fs: [0x30]
        mov _peb, eax
    }

    PPEB_LDR_DATA          Pldr = _peb->Ldr;
    PLIST_ENTRY            FIRST = &(Pldr->InMemoryOrderModuleList);
    PLIST_ENTRY            CURLST = FIRST->Flink;
    PLDR_DATA_TABLE_ENTRYA lModule;
    PLDR_DATA_TABLE_ENTRYA lDis{};
    int StructDiff = (int)&lDis->InMemoryOrderLinks;
    do
    {
        lModule = (PLDR_DATA_TABLE_ENTRYA)((unsigned)CURLST - StructDiff);
        if (lModule->DllBase == _hMod)
        {

            lModule->InLoadOrderModuleList.Blink->Flink = lModule->InLoadOrderModuleList.Flink;
            lModule->InLoadOrderModuleList.Flink->Blink = lModule->InLoadOrderModuleList.Blink;

            lModule->InMemoryOrderLinks.Blink->Flink = lModule->InMemoryOrderLinks.Flink;
            lModule->InMemoryOrderLinks.Flink->Blink = lModule->InMemoryOrderLinks.Blink;

            lModule->InInitializationOrderModuleList.Blink->Flink = lModule->InInitializationOrderModuleList.Flink;
            lModule->InInitializationOrderModuleList.Flink->Blink = lModule->InInitializationOrderModuleList.Blink;

        }
        CURLST = CURLST->Flink;
    } while (FIRST != CURLST);

    //È¥µôPEÌØÕ÷
    IMAGE_DOS_HEADER   dosH{};
    IMAGE_NT_HEADERS   ntH{};
    PIMAGE_DOS_HEADER  dosHeader = (PIMAGE_DOS_HEADER)_hMod;
    PIMAGE_NT_HEADERS  ntHeader = (PIMAGE_NT_HEADERS)(dosHeader->e_lfanew + (unsigned)_hMod);
    DWORD dOld;
    VirtualProtect(dosHeader, sizeof(dosH), PAGE_EXECUTE_READWRITE, &dOld);
    VirtualProtect(ntHeader, sizeof(ntH), PAGE_EXECUTE_READWRITE, &dOld);
    memcpy(dosHeader, &dosH, sizeof(dosH));
    memcpy(ntHeader, &ntH, sizeof(ntH));


}

void htdModule::HideDll(wchar_t* dllName)
{
	HMODULE hMod = GetModuleHandleW(dllName);
    if(hMod) HideDll(hMod);
}
