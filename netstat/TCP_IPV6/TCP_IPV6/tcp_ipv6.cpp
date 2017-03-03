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

int main()
{
	PMIB_TCP6TABLE2 pTcp6Table;
	ULONG ulSize = 0;
	DWORD dwRetVal = 0;

	int i;

	pTcp6Table = (MIB_TCP6TABLE2 *)MALLOC(sizeof(MIB_TCP6TABLE2));
	if (pTcp6Table == NULL) {
		printf("Error allocating memory\n");
		return 1;
	}

	ulSize = sizeof(MIB_TCPTABLE);
	// Make an initial call to GetTcp6Table2 to
	// get the necessary size into the ulSize variable
	if ((dwRetVal = GetTcp6Table2(pTcp6Table, &ulSize, TRUE)) ==
		ERROR_INSUFFICIENT_BUFFER) {
		FREE(pTcp6Table);
		pTcp6Table = (MIB_TCP6TABLE2 *)MALLOC(ulSize);
		if (pTcp6Table == NULL) {
			printf("Error allocating memory\n");
			return 1;
		}
	}
	// Make a second call to GetTcp6Table2 to get
	// the actual data we require
	if ((dwRetVal = GetTcp6Table2(pTcp6Table, &ulSize, TRUE)) == NO_ERROR) {
		printf("\tNumber of entries: %d\n", (int)pTcp6Table->dwNumEntries);
		for (i = 0; i < (int)pTcp6Table->dwNumEntries; i++) {
			printf("\n\tTCP[%d] State: %ld - ", i,
				pTcp6Table->table[i].State);
			switch (pTcp6Table->table[i].State) {
			case MIB_TCP_STATE_CLOSED:
				printf("CLOSED\n");
				break;
			case MIB_TCP_STATE_LISTEN:
				printf("LISTEN\n");
				break;
			case MIB_TCP_STATE_SYN_SENT:
				printf("SYN-SENT\n");
				break;
			case MIB_TCP_STATE_SYN_RCVD:
				printf("SYN-RECEIVED\n");
				break;
			case MIB_TCP_STATE_ESTAB:
				printf("ESTABLISHED\n");
				break;
			case MIB_TCP_STATE_FIN_WAIT1:
				printf("FIN-WAIT-1\n");
				break;
			case MIB_TCP_STATE_FIN_WAIT2:
				printf("FIN-WAIT-2 \n");
				break;
			case MIB_TCP_STATE_CLOSE_WAIT:
				printf("CLOSE-WAIT\n");
				break;
			case MIB_TCP_STATE_CLOSING:
				printf("CLOSING\n");
				break;
			case MIB_TCP_STATE_LAST_ACK:
				printf("LAST-ACK\n");
				break;
			case MIB_TCP_STATE_TIME_WAIT:
				printf("TIME-WAIT\n");
				break;
			case MIB_TCP_STATE_DELETE_TCB:
				printf("DELETE-TCB\n");
				break;
			default:
				printf("UNKNOWN State value\n");
				break;
			}
			printLocalAddr(pTcp6Table->table[i].LocalAddr.u.Byte, i);
			printf("\tTCP[%d] Local Port: %d \n", i,
				ntohs((u_short)pTcp6Table->table[i].dwLocalPort));

			printLocalAddr(pTcp6Table->table[i].RemoteAddr.u.Byte, i);
			printf("\tTCP[%d] Remote Port: %d\n", i,
				ntohs((u_short)pTcp6Table->table[i].dwRemotePort));

			printf("\tTCP[%d] Owning PID: %d\n", i, pTcp6Table->table[i].dwOwningPid);
		}
	}
	else {
		printf("\tGetTcpTable2 failed with %d\n", dwRetVal);
		FREE(pTcp6Table);
		return 1;
	}

	if (pTcp6Table != NULL) {
		FREE(pTcp6Table);
		pTcp6Table = NULL;
	}

	return 0;
}

void printLocalAddr(UCHAR *ipv6Addr, int i) {
	printf("\n\tUDP[%d] Local Address: ", i);
	for (int i = 0;i < 16;) {
		if (ipv6Addr[i] != 0) printf("%.1x", ipv6Addr[i]);
		if (!((++i) % 2) && (i < 16)) printf(":");
	}
	printf("\n");
}
