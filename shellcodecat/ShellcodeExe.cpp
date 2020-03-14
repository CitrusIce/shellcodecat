#include "stdafx.h"
#include "ShellcodeExe.h"
#include <Windows.h>
typedef int (WINAPI *ShellcodeEntry)(void);
void ShellcodeExe(void* pData, size_t dataSize)
{
	char* buf = (char*)VirtualAlloc(0, dataSize, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);
	::RtlCopyMemory(buf, pData, dataSize);
	ShellcodeEntry entry = (ShellcodeEntry)buf;
	entry();
}
