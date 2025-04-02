// reference to EngineMain
extern int EngineMain(int argc, char *argv[]);

void EnginePreShutdown() {}

int main(int argc, char *argv[]) {
    return EngineMain(argc, argv);
}