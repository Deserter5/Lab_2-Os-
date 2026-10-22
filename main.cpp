#include "Manager.h"

int main() {
    InitSyncPrimitives();

    if (!g_hMutex || !g_hSemaphore || !g_hEvent) {
        CloseSyncPrimitives();
        return GetLastError();
    }

    DWORD id1, id2;
    HANDLE hThreads[2];

    hThreads[0] = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)ThreadProc1, NULL, CREATE_SUSPENDED, &id1);
    hThreads[1] = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)ThreadProc2, NULL, CREATE_SUSPENDED, &id2);

    if (!hThreads[0] || !hThreads[1]) {
        CloseSyncPrimitives();
        return GetLastError();
    }

    SetThreadPriority(hThreads[0], THREAD_PRIORITY_NORMAL);
    SetThreadPriority(hThreads[1], THREAD_PRIORITY_ABOVE_NORMAL);

    ResumeThread(hThreads[0]);
    ResumeThread(hThreads[1]);

    WaitForMultipleObjects(2, hThreads, TRUE, INFINITE);

    CloseHandle(hThreads[0]);
    CloseHandle(hThreads[1]);

    CloseSyncPrimitives();

    cout << "Main thread finished successfully." << endl;
    cin.get();
    return 0;
}