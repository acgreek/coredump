#ifndef COREDUMP_H
#define COREDUMP_H

#ifdef __cplusplus
extern "C" {
#endif

void setup_core_dump_handler(const char * path);
void fini_core_dump_handler(void);

#ifdef __cplusplus
}
class CoreDumpHandler final {
	public:
		CoreDumpHandler(const char * path) {
			setup_core_dump_handler(path);
		}
		~CoreDumpHandler() {
			fini_core_dump_handler();
		}
};
#endif

#endif  // COREDUMP_H
