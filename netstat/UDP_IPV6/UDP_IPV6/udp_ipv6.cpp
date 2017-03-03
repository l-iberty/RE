#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <winsock2.h>
#include <ws2tcpip.h>
#include <iphlpapi.h>
#include <stdio.h>

#pragma comment(lib, "iphlpapi.lib")
#pragma comment(lib, "ws2_32.lib")

#define MALLOC(x) HeapAlloc(GetProcessHeap(), 0, (x))
#define FREE(x) HeapFree(GetProcessHeap(), 0, (x))

void printLocalAddr(UCHAR *ipv6Addr, int i);

int main() {
	PMIB_UDP6TABLE_OWNER_MODULE pUdp6Entry;
	DWORD dwSize = 0;
	DWORD dwRetVal = 0;

	pUdp6Entry = (MIB_UDP6TABLE_OWNER_MODULE *)MALLOC(sizeof(MIB_UDP6TABLE_OWNER_MODULE));
	if (pUdp6Entry == NULL) {
		printf("Error allocating memory\n");
		return 1;
	}

	dwSize = sizeof(MIB_UDP6TABLE_OWNER_MODULE);
	// Make an initial call to GetExtendedUdpTable to
	// get the necessary size into the dwSize variable
	if ((dwRetVal = GetExtendedUdpTable(pUdp6Entry, &dwSize, TRUE, AF_INET6, UDP_TABLE_OWNER_MODULE, 0)) ==
		ERROR_INSUFFICIENT_BUFFER) {
		FREE(pUdp6Entry);
		pUdp6Entry = (MIB_UDP6TABLE_OWNER_MODULE *)MALLOC(dwSize);
		if (pUdp6Entry == NULL) {
			printf("Error allocating memory\n");
			return 1;
		}
	}
	// Make a second call to GetExtendedUdpTable to get
	// the actual data we require
	if ((dwRetVal = GetExtendedUdpTable(pUdp6Entry, &dwSize, TRUE, AF_INET6, UDP_TABLE_OWNER_MODULE, 0)) ==
		NO_ERROR) {
		printf("\tNumber of entries: %d\n", (int)pUdp6Entry->dwNumEntries);

		for (int i = 0;i < (int)pUdp6Entry->dwNumEntries;i++) {
			printLocalAddr(pUdp6Entry->table[i].ucLocalAddr, i);
			printf("\tUDP[%d] Local Port: %d \n", i,
				ntohs((u_short)pUdp6Entry->table[i].dwLocalPort));

			printf("\tUDP[%d] Owning PID: %d\n", i, pUdp6Entry->table[i].dwOwningPid);
		}
	}
	else {
		printf("\tGetExtendedUdpTable failed with #%d\n", dwRetVal);
		FREE(pUdp6Entry);
		return 1;
	}

	if (pUdp6Entry != NULL) {
		FREE(pUdp6Entry);
		pUdp6Entry = NULL;
	}

	return 0;
}

void printLocalAddr(UCHAR *ipv6Addr, int i) {
	printf("\n\tUDP[%d] Local Address: ", i);
	for (int i = 0;i < 16;) {
		if(ipv6Addr[i] != 0) printf("%.1x", ipv6Addr[i]);
		if (!((++i) % 2) && (i < 16)) printf(":");
	}
	printf("\n");
}