// Enjoy the leak!


#include <thread>
#include "stdafx.h"
#include "game.h"
#include "driver.h"
#include "ss.h"
#include <wininet.h>
#include <string>
#include "render/auth.hpp"
#include "console.h"
#include "proct.hpp"
#include <Windows.h>
#include <cstdlib>
#include <ctime>
#include <string>
#include <windows.h>
#include <iostream>

#include <filesystem>
#include <fstream>

using namespace KeyAuth;

// cool beans

std::string name = ""; // application name. right above the blurred text aka the secret on the licenses tab among other tabs
std::string ownerid = ""; // ownerid, found in account settings. click your profile picture on top right of dashboard and then account settings.
std::string secret = ""; // app secret, the blurred text on licenses tab and other tabs
std::string version = "1.1"; // leave alone unless you've changed version on website
std::string url = ("https://keyauth.win/api/1.2/");
std::string sslPin = ("ssl pin key (optional)");

api KeyAuthApp(name, ownerid, secret, version, url, sslPin);

void SetConsoleSize()
{
	HANDLE hOut;
	SMALL_RECT DisplayArea = { 0, 0, 0, 0 };
	int x = 80;
	int y = 20;
	hOut = GetStdHandle(STD_OUTPUT_HANDLE);
	DisplayArea.Right = x;
	DisplayArea.Bottom = y;

	SetConsoleWindowInfo(hOut, TRUE, &DisplayArea);
}

void HideConsole()
{
	::ShowWindow(::GetConsoleWindow(), SW_HIDE);
}

void ShowConsole()
{
	::ShowWindow(::GetConsoleWindow(), SW_SHOW);
}

auto get_process_wnd(uint32_t pid) -> HWND
{
	std::pair<HWND, uint32_t> params = { 0, pid };
	BOOL bResult = EnumWindows([](HWND hwnd, LPARAM lParam) -> BOOL {
		auto pParams = (std::pair<HWND, uint32_t>*)(lParam);
		uint32_t processId = 0;

		if (GetWindowThreadProcessId(hwnd, reinterpret_cast<LPDWORD>(&processId)) && processId == pParams->second) {
			SetLastError((uint32_t)-1);
			pParams->first = hwnd;
			return FALSE;
		}

		return TRUE;

		}, (LPARAM)&params);

	if (!bResult && GetLastError() == -1 && params.first)
		return params.first;

	return NULL;
}

void rndmTitle()
{
	constexpr int length = 10;
	const auto characters = TEXT("abcdefghi9182345jklmnopqrstuv211935960473wxyzABCDEFGHIJKLMNOPQRSTUVWXYZ1234567890");
	TCHAR title[length + 1]{};

	for (int j = 0; j != length; j++)
	{
		title[j] += characters[rand() % 90 + 1];
	}

	SetConsoleTitle(title);
}

#include "Mapper/Kdmapper/kdmapper.hpp"

inline HANDLE iqvw64e_device_handle;

inline LONG WINAPI SimplestCrashHandler(EXCEPTION_POINTERS* ExceptionInfo)
{
	if (ExceptionInfo && ExceptionInfo->ExceptionRecord)
		Log(L"[!!] Crash at addr 0x" << ExceptionInfo->ExceptionRecord->ExceptionAddress << L" 0x" << std::hex << ExceptionInfo->ExceptionRecord->ExceptionCode << std::endl);
	else
		Log(L"[!!] Crash" << std::endl);

	if (iqvw64e_device_handle)
		intel_driver::Unload(iqvw64e_device_handle);

}
std::wstring s2ws(const std::string& str) {
	int size_needed = MultiByteToWideChar(CP_UTF8, 0, &str[0], (int)str.size(), NULL, 0);
	std::wstring wstrTo(size_needed, 0);
	MultiByteToWideChar(CP_UTF8, 0, &str[0], (int)str.size(), &wstrTo[0], size_needed);
	return wstrTo;
}

int retreiveValProcessId()
{
	BYTE target_name[] = { 'V','A','L','O','R','A','N','T','-','W','i','n','6','4','-','S','h','i','p','p','i','n','g','.','e','x','e', 0 };
	std::wstring process_name = s2ws(std::string((char*)target_name));
	HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	PROCESSENTRY32W entry;
	entry.dwSize = sizeof(entry);

	if (!Process32First(snapshot, &entry)) {
		return 0;
	}

	while (Process32Next(snapshot, &entry)) {
		if (std::wstring(entry.szExeFile) == process_name) {
			return entry.th32ProcessID;
		}
	}

	return 0;
}

inline bool callbackExample(ULONG64* param1, ULONG64* param2, ULONG64 allocationPtr, ULONG64 allocationSize, ULONG64 mdlptr) {
	UNREFERENCED_PARAMETER(param1);
	UNREFERENCED_PARAMETER(param2);
	UNREFERENCED_PARAMETER(allocationPtr);
	UNREFERENCED_PARAMETER(allocationSize);
	UNREFERENCED_PARAMETER(mdlptr);
	Log("[+] Callback example called" << std::endl);
	return true;
}

int load_driver()
{
	iqvw64e_device_handle = intel_driver::Load();

	NTSTATUS exitCode = 0;
	if (!kdmapper::MapDriver(iqvw64e_device_handle, 0, 0, false, true, 0, 0, 0, &exitCode)) {
		{
			Log(L"[-] Failed to map" << std::endl);
			intel_driver::Unload(iqvw64e_device_handle);
			Sleep(500);
			exit(0);
		}

		if (!intel_driver::Unload(iqvw64e_device_handle)) {
			Log(L"[-] Warning failed to fully unload vulnerable driver " << std::endl);
		}
		Log(L"[+] Success" << std::endl);
	}
	intel_driver::Unload(iqvw64e_device_handle);

	Sleep(500);
}

int process_id;
HWND window;
int cheat()
{
	{
		if (GlobalFindAtomA(XorStr("DriverAlreadyLoaded").c_str()) == 0)
		{
			system(XorStr("cls").c_str());
			load_driver();
			GlobalAddAtomA(XorStr("DriverAlreadyLoaded").c_str());
		}

	if (mem::find_driver())
	{
		system(XorStr("cls").c_str());
		Abdul::SetPenisRep(GREEN);
		printf(XorStr("SUCCESS").c_str());
		Abdul::SetPenisRep(WHITE);
		printf(XorStr(": Driver is initialized!").c_str());
		printf(XorStr("\n\n").c_str());
	}
	else
	{
		system(XorStr("cls").c_str());

		Abdul::SetPenisRep(RED);
		printf(XorStr("ERROR").c_str());
		Abdul::SetPenisRep(WHITE);
		printf(XorStr(": Driver not initialized!").c_str());
		printf(XorStr("\n\n").c_str());
		Sleep(-1);
	}
	mem::find_process(L"VALORANT-Win64-Shipping.exe");
	virtualaddy = mem::get_guarded_region();

	HWND Entryhwnd = NULL;
	while (Entryhwnd == NULL)
	{
		process_id = retreiveValProcessId();;
		Entryhwnd = get_process_wnd(process_id);
		Sleep(1);
	}	
	game::start_cheat();
	}
	Sleep(-1);
	return NULL;
}

int option;
std::string key;

int main()
{
	SetConsoleSize();
	rndmTitle();
	cheat();
}