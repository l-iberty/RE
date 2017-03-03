/*******************************************************************
 * Fport.exe 中使用的 API AllocateAndGetExTcpTableFromStack
 * 已经过时, msdn文档中推荐用 GetTcpTable2 代替之.
 *******************************************************************/

#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <winsock2.h>
#include <ws2tcpip.h>
#include <iphlpapi.h>
#include <Windows.h>
#include <Psapi.h>
#include <stdio.h>
#include "CModule.h"
#include "CAccessToken.h"

#pragma comment(lib, "iphlpapi.lib")
#pragma comment(lib, "ws2_32.lib")

#define BUFFSIZE 128
#define MAX_MODULESNUM 128

int main(int argc, char **argv) {
	PMIB_TCPTABLE2 pTcpTable;
	ULONG ulSize = 0;
	DWORD dwRetVal = 0;

	char ProcessName[BUFFSIZE];
	char ProcessPath[BUFFSIZE];

	// Enable the privilage of the process
	if (argc != 2) {
		printf("Input <FportDemo 1> to enable privilege\n");
	}
	if (argc == 2 && !lstrcmpA(argv[1], "1")) {
		CAccessToken accessToken = CAccessToken();
		accessToken.EnablePrivilege(SE_DEBUG_NAME);
	}

	pTcpTable = (MIB_TCPTABLE2 *)malloc(sizeof(MIB_TCPTABLE2));
	if (pTcpTable == NULL) {
		printf("Error allocating memory\n");
		return 1;
	}

	ulSize = sizeof(MIB_TCPTABLE);
	// Make an initial call to GetTcpTable2 to
	// get the necessary size into the ulSize variable
	if ((dwRetVal = GetTcpTable2(pTcpTable, &ulSize, TRUE)) ==
		ERROR_INSUFFICIENT_BUFFER) {
		free(pTcpTable);
		pTcpTable = (MIB_TCPTABLE2 *)malloc(ulSize);
		if (pTcpTable == NULL) {
			printf("Error allocating memory\n");
			return 1;
		}
	}

	// Make a second call to GetTcpTable2 to get
	// the actual data we require
	if ((dwRetVal = GetTcpTable2(pTcpTable, &ulSize, TRUE)) == NO_ERROR) {
		printf("\tNumber of entries: %d\n", (int)pTcpTable->dwNumEntries);
		for (int i = 0; i < (int)pTcpTable->dwNumEntries; i++) {
			printf("\n\n\tTCP[%d] Local Port: %d \n", i,
				ntohs((u_short)pTcpTable->table[i].dwLocalPort));

			DWORD dwPID = pTcpTable->table[i].dwOwningPid;
			printf("\tTCP[%d] Owning PID: %d\n", i, dwPID);
			// ID 为0和4的两个进程特殊处理, OpenProcess无法访问之, 即使提升了
			// 进程权限.
			// 对 Fport.exe 的逆向分析显示, Fport 源码中包含 "System Idle Process"
			// 和 "System" 两个字符串, 似乎是对这两个进程做了特别对待.
			switch (dwPID) {
			case 0:
				printf("\tProcess Name: System Idle Process\n");
				continue;
			case 4:
				printf("\tProcess Name: System\n");
				continue;
			}
			
			CModule *processModule = new CModule();
			if (processModule->Init(dwPID)) {
				processModule->GetProcessName(ProcessName, BUFFSIZE);
				processModule->GetProcessFilePath(ProcessPath, BUFFSIZE);

				printf("\tProcess Name: %s\n", ProcessName);
				printf("\tPath: %s\n", ProcessPath);
			}
			delete processModule;
		}
	}
	else {
		printf("\tGetTcpTable2 failed with %d\n", dwRetVal);
		free(pTcpTable);
		return 1;
	}

	if (pTcpTable != NULL) {
		free(pTcpTable);
		pTcpTable = NULL;
	}

	return 0;
}
