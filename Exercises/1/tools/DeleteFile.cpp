#include <Windows.h>
#include <stdio.h>

void main()
{
	HANDLE hFile;
	hFile = CreateFileA("C:\\135.txt",
		GENERIC_READ | GENERIC_WRITE,
		FILE_SHARE_READ | FILE_SHARE_WRITE,
		NULL,
		OPEN_ALWAYS,
		FILE_ATTRIBUTE_HIDDEN | FILE_ATTRIBUTE_SYSTEM,
		NULL);

	char Buffer[12] = { 0 };
	DWORD cbRead;
	ReadFile(hFile, Buffer, 1, &cbRead, NULL);
	printf("%s\n", Buffer);

	CloseHandle(hFile);
	BOOL bOk = DeleteFileA("C:\\135.txt");
	if (!bOk)
		printf("Error\n");
}