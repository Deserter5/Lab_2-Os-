#include "Manager.h"

LONG g_counter = 0;
int g_sharedData = 0;

CRITICAL_SECTION g_cs;
HANDLE g_hMutex = NULL;
HANDLE g_hSemaphore = NULL;
HANDLE g_hEvent = NULL;

void InitSyncPrimitives() {
    InitializeCriticalSection(&g_cs);
    g_hMutex = CreateMutex(NULL, FALSE, NULL);
    g_hSemaphore = CreateSemaphore(NULL, 1, 1, NULL);
    g_hEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
}

void CloseSyncPrimitives() {
    CloseHandle(g_hMutex);
    CloseHandle(g_hSemaphore);
    CloseHandle(g_hEvent);
    DeleteCriticalSection(&g_cs);
}

DWORD WINAPI ThreadProc1(LPVOID lpParam) {
    for (int i = 0; i < 5; ++i) {
        EnterCriticalSection(&g_cs);
        g_sharedData++;
        cout << "[Thread 1] Critical Section: g_sharedData = " << g_sharedData << endl;
        LeaveCriticalSection(&g_cs);
        Sleep(250);
    }
    cout << "[Thread 1] Trying to acquire semaphore..." << endl;
    WaitForSingleObject(g_hSemaphore, INFINITE); 
    cout << "[Thread 1] Semaphore acquired successfully!" << endl;

    Sleep(100);
    ReleaseSemaphore(g_hSemaphore, 1, NULL);
    cout << "[Thread 1] Semaphore released." << endl;

    cout << "[Thread 1] Trying to acquire Mutex..." << endl;
    WaitForSingleObject(g_hMutex, INFINITE);
    g_counter++;
    cout << "[Thread 1] Mutex acquired. Interlocked counter: " << g_counter << endl;
    ReleaseMutex(g_hMutex);

    SetEvent(g_hEvent);
    return 0;
}

DWORD WINAPI ThreadProc2(LPVOID lpParam) {
    EnterCriticalSection(&g_cs);
    g_sharedData += 10;
    cout << "[Thread 2] Critical Section: g_sharedData = " << g_sharedData << endl;
    LeaveCriticalSection(&g_cs);

    cout << "[Thread 2] Trying to acquire semaphore first..." << endl;
    WaitForSingleObject(g_hSemaphore, INFINITE);
    cout << "[Thread 2] Semaphore acquired! Holding it for 1 second..." << endl;

    Sleep(1000); 
    ReleaseSemaphore(g_hSemaphore, 1, NULL);
    cout << "[Thread 2] Semaphore released." << endl;

    cout << "[Thread 2] Trying to acquire Mutex first..." << endl;
    WaitForSingleObject(g_hMutex, INFINITE);
    cout << "[Thread 2] Mutex acquired! Holding it for 1 second..." << endl;

    Sleep(1000);
    ReleaseMutex(g_hMutex);

    WaitForSingleObject(g_hEvent, INFINITE);
    cout << "[Thread 2] Event signal received from Thread 1!" << endl;
    return 0;
}