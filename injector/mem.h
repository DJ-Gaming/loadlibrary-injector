#pragma once
#include <iostream>
#include <Windows.h>
#include <TlHelp32.h>
#include <chrono>
#include <thread>
#include <libloaderapi.h>

bool inject(DWORD pid, const char* dll)
{
	if (pid == 0) {
		std::cout << "Unable to find PID.";
		return FALSE;
	}

	char myDLL[MAX_PATH];
	GetFullPathName(dll, MAX_PATH, myDLL, 0);

	HANDLE hProcess = OpenProcess(PROCESS_CREATE_THREAD | PROCESS_QUERY_INFORMATION | PROCESS_VM_READ | PROCESS_VM_WRITE | PROCESS_VM_OPERATION, FALSE, pid);

	LPVOID allocatedMem = VirtualAllocEx(hProcess, NULL, sizeof(myDLL), MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);
	WriteProcessMemory(hProcess, allocatedMem, myDLL, sizeof(myDLL), NULL);

	CreateRemoteThread(hProcess, 0, 0, (LPTHREAD_START_ROUTINE)LoadLibrary, allocatedMem, 0, 0);

	CloseHandle(hProcess);

	return TRUE;
}

DWORD ProcessID(const char* ProcessName)
{
	//Create a snapshot of all running processes
	HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);
	if (hSnapshot == INVALID_HANDLE_VALUE) return false;

	//Used to store the process info in the loop
	PROCESSENTRY32 ProcEntry;
	ProcEntry.dwSize = sizeof(PROCESSENTRY32);

	//Get the first process
	if (Process32First(hSnapshot, &ProcEntry)) {
		do
		{
			//If the found process name is equal to the on we're searching for
			if (!strcmp(ProcEntry.szExeFile, ProcessName))
			{
				CloseHandle(hSnapshot);
				//Return the processID of the found process
				//TODO: return a list of found processes instead
				return ProcEntry.th32ProcessID;
			}
		} while (Process32Next(hSnapshot, &ProcEntry)); //Get the next process
	}

	CloseHandle(hSnapshot);
	//Since a process hasn't been found, return 0
	return 0;
}
