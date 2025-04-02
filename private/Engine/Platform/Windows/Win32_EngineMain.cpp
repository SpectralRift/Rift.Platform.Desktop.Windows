#include <Windows.h>

// reference to EngineMain
extern int EngineMain(int argc, char *argv[]);

void EnginePreShutdown() {}

#ifdef USE_WINDOWED_SUBSYSTEM
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    return EngineMain(0, nullptr);
}
#else

int main(int argc, char *argv[]) {
    return EngineMain(argc, argv);
}

#endif