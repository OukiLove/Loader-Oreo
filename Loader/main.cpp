#include <Windows.h>
#include <iostream>
#include "memory.hpp"
#include <string>
#include <format>
#include <filesystem>

#pragma comment(lib, "urlmon.lib")

using namespace std;
string namedll;
Injector inj;
DWORD pid;

void bypass()
{
	LPVOID ntOpenFile = GetProcAddress(LoadLibraryW(L"ntdll"), "NtOpenFile");
	if (ntOpenFile) {
		char originalBytes[5];
		memcpy(originalBytes, ntOpenFile, 5);
		WriteProcessMemory(inj.process, ntOpenFile, originalBytes, 5, NULL);
	}
	else
	{
		cout << "Unable to bypass.\n";
	}
}

void injectCheat()
{
	cout << "Wait..." << endl;
	bypass();
	string dwnld_URL = "https://github.com/OukiLove/oukilove.github.io/raw/main/assets/loader/Oreo.dll";
	string savepath = "C:\\Windows\\Oreo.dll";
	URLDownloadToFile(NULL, dwnld_URL.c_str(), savepath.c_str(), 0, NULL);
	inj.inject(pid, savepath.c_str());
	cout << "Inject successfully\n";
	system("pause");
}

int main()
{
	SetConsoleTitle("Loader By Ouki76");
	CreateDirectory("C:\\Oreo\\", NULL);

	inj.hwndproc = FindWindowA(0, "Counter-Strike: Global Offensive - Direct3D 9");
	GetWindowThreadProcessId(inj.hwndproc, &pid);
	inj.process = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pid);
	inj.clientDLL = inj.GetModule(pid, "client.dll");

	if (pid > 1)
	{
		injectCheat();
	}
	else
	{
		ShellExecuteA(NULL, "open", "steam://rungameid/730", NULL, NULL, SW_SHOWNORMAL);
		while (true)
		{
			inj.hwndproc = FindWindowA(0, "Counter-Strike: Global Offensive - Direct3D 9");
			GetWindowThreadProcessId(inj.hwndproc, &pid);
			inj.process = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pid);
			inj.clientDLL = inj.GetModule(pid, "client.dll");

			if (pid > 1)
				break;
		}
		Sleep(1500);
		injectCheat();
	}
}