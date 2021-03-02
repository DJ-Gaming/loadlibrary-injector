#include "mem.h"

const char* dll = "csgo-cheat.dll"; // replace with your dll

int main()
{
	int Pid = ProcessID("csgo.exe");
	HANDLE Proc = OpenProcess(PROCESS_ALL_ACCESS, FALSE, Pid);

	//credit: Daniel Krupiñski
	LPVOID ntOpenFile = GetProcAddress(LoadLibraryW(L"ntdll"), "NtOpenFile");
	if (ntOpenFile) {
		char originalBytes[5];
		memcpy(originalBytes, ntOpenFile, 5);
		WriteProcessMemory(Proc, ntOpenFile, originalBytes, 5, NULL);
	}

	inject(Pid, dll);
	std::cout << "Successfully Injected " << dll << "!";
	Sleep(3000);
	exit(0);
}