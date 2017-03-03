#pragma once

#include <Windows.h>
#include <Psapi.h>
#include <stdio.h>

#define BUFFSIZE 128
#define MAX_MODULESNUM 128

class CModule {
private:
	char BaseName[BUFFSIZE];
	char FileName[BUFFSIZE];
	HANDLE hProcess = NULL;
	HMODULE hModules[MAX_MODULESNUM];
	DWORD cbNeeded = 0, cModules = 0;

public:
	CModule();
	~CModule();
	bool Init(DWORD dwPID);
	DWORD GetProcessName(LPSTR lpProcessName, DWORD BufSize);
	DWORD GetProcessFilePath(LPSTR lpProcessPath, DWORD BufSize);
};

CModule::CModule() {
	// do nothing
}

bool CModule::Init(DWORD dwPID) {
	hProcess = OpenProcess(
		PROCESS_VM_READ | PROCESS_QUERY_INFORMATION,
		FALSE,
		dwPID);
	if (hProcess == INVALID_HANDLE_VALUE || hProcess == NULL) {
		DWORD dwErr = GetLastError();
		printf("\tOpenProcess error: #%d ", dwErr);
		if (dwErr == 5) {
			printf("Access Denied\n");
		}
		return false;
	}

	if (!EnumProcessModules(hProcess, hModules, MAX_MODULESNUM*sizeof(HMODULE), &cbNeeded)) {
		printf("\tEnumProcessModules error: #%d\n", GetLastError());
		return false;
	}

	return true;
}

DWORD CModule::GetProcessName(LPSTR lpProcessName, DWORD BufSize) {
	GetModuleBaseNameA(hProcess, hModules[0], lpProcessName, BufSize);
	return GetLastError();
}

DWORD CModule::GetProcessFilePath(LPSTR lpProcessPath, DWORD BufSize) {
	GetModuleFileNameExA(hProcess, hModules[0], lpProcessPath, BufSize);
	return GetLastError();
}

CModule::~CModule() {
	if (hProcess != INVALID_HANDLE_VALUE) {
		CloseHandle(hProcess);
	}
	hProcess = NULL;
}