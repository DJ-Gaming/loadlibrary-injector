#include "mem.h"

const char* dll = "csgo-cheat.dll"; // replace with your dll

int main()
{
	char restoreBytes[5];
	int Pid = ProcessID("csgo.exe");
	HANDLE Proc = OpenProcess(PROCESS_ALL_ACCESS, FALSE, Pid);

	//credit: Daniel Krupiñski
	LPVOID ntOpenFile = GetProcAddress(LoadLibraryW(L"ntdll"), "NtOpenFile");
	if (ntOpenFile) {
		char originalBytes[5];
		memcpy(originalBytes, ntOpenFile, 5);
		ReadProcessMemory(Proc, ntOpenFile, restoreBytes, 5, NULL);
		WriteProcessMemory(Proc, ntOpenFile, originalBytes, 5, NULL);
	}

	inject(Pid, dll);
	WriteProcessMemory(Proc, ntOpenFile, restoreBytes, 0, NULL);
	std::cout << "Successfully Injected " << dll << "!";
	Sleep(3000);
	exit(0);
}
