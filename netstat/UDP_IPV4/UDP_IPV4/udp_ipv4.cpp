#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <winsock2.h>
#include <ws2tcpip.h>
#include <iphlpapi.h>
#include <stdio.h>

#pragma comment(lib, "iphlpapi.lib")
#pragma comment(lib, "ws2_32.lib")

#define MALLOC(x) HeapAlloc(GetProcessHeap(), 0, (x))
#define FREE(x) HeapFree(GetProcessHeap(), 0, (x))

int main() {
	PMIB_UDPTABLE_OWNER_MODULE pUdpEntry;
	DWORD dwSize = 0;
	DWORD dwRetVal = 0;

	char szLocalAddr[128];

	struct in_addr IpAddr;

	pUdpEntry = (MIB_UDPTABLE_OWNER_MODULE *)MALLOC(sizeof(MIB_UDPTABLE_OWNER_MODULE));
	if (pUdpEntry == NULL) {
		printf("Error allocating memory\n");
		return 1;
	}

	dwSize = sizeof(MIB_UDPTABLE_OWNER_MODULE);
	// Make an initial call to GetExtendedUdpTable to
	// get the necessary size into the dwSize variable
	if ((dwRetVal = GetExtendedUdpTable(pUdpEntry, &dwSize, TRUE, AF_INET, UDP_TABLE_OWNER_MODULE, 0)) ==
		ERROR_INSUFFICIENT_BUFFER) {
		FREE(pUdpEntry);
		pUdpEntry = (MIB_UDPTABLE_OWNER_MODULE *)MALLOC(dwSize);
		if (pUdpEntry == NULL) {
			printf("Error allocating memory\n");
			return 1;
		}
	}
	// Make a second call to GetExtendedUdpTable to get
	// the actual data we require
	if ((dwRetVal = GetExtendedUdpTable(pUdpEntry, &dwSize, TRUE, AF_INET, UDP_TABLE_OWNER_MODULE, 0)) ==
		NO_ERROR) {
		printf("\tNumber of entries: %d\n", (int)pUdpEntry->dwNumEntries);

		for (int i = 0;i < (int)pUdpEntry->dwNumEntries;i++) {
			IpAddr.S_un.S_addr = (u_long)pUdpEntry->table[i].dwLocalAddr;
			strcpy_s(szLocalAddr, sizeof(szLocalAddr), inet_ntoa(IpAddr));
			printf("\n\tUDP[%d] Local Addr: %s\n", i, szLocalAddr);
			printf("\tUDP[%d] Local Port: %d \n", i,
				ntohs((u_short)pUdpEntry->table[i].dwLocalPort));

			printf("\tUDP[%d] Owning PID: %d\n", i, pUdpEntry->table[i].dwOwningPid);
		}
	}
	else {
		printf("\tGetExtendedUdpTable failed with #%d\n", dwRetVal);
		FREE(pUdpEntry);
		return 1;
	}

	if (pUdpEntry != NULL) {
		FREE(pUdpEntry);
		pUdpEntry = NULL;
	}
	
	return 0;
}