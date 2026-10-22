#ifndef SYNC_MANAGER_H
#define SYNC_MANAGER_H

#include <windows.h>
#include <iostream>

using namespace std;

extern LONG g_counter;
extern int g_sharedData;

extern CRITICAL_SECTION g_cs;
extern HANDLE g_hMutex;
extern HANDLE g_hSemaphore;
extern HANDLE g_hEvent;

void InitSyncPrimitives();
void CloseSyncPrimitives();

DWORD WINAPI ThreadProc1(LPVOID lpParam);
DWORD WINAPI ThreadProc2(LPVOID lpParam);

#endif
