#pragma once
#include <windows.h>
#include <stdio.h>

class CAccessToken {
public:
	CAccessToken() {};
	~CAccessToken() {};
	BOOL  EnablePrivilege(LPWSTR lpPrivilageName);
};

BOOL  CAccessToken::EnablePrivilege(LPWSTR lpPrivilageName) {
	HANDLE hToken;
	TOKEN_PRIVILEGES tp;
	BOOL fOk = FALSE;

	if (OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES, &hToken)) {
		tp.PrivilegeCount = 1;

		if (!LookupPrivilegeValue(NULL, lpPrivilageName, &tp.Privileges[0].Luid)) {
			printf("Can't lookup privilege value.\n");
		}
			
		tp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;
		if (!AdjustTokenPrivileges(hToken, FALSE, &tp, sizeof(TOKEN_PRIVILEGES), NULL, NULL)) {
			printf("Can't adjust privilege value.\n");
		}
			
		fOk = (GetLastError() == ERROR_SUCCESS);
		CloseHandle(hToken);
	}
	return fOk;
}