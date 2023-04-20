#ifndef LOG_H
#define LOG_H

#include <stdio.h>

#define LOG(lvl, ...) { fprintf(stderr, "%s: ", lvl); \
                    fprintf(stderr, __VA_ARGS__); \
                    fprintf(stderr, "\n"); }
#define FATAL(...) { LOG("FATAL", __VA_ARGS__); \
                    exit(EXIT_FAILURE); }
#define ERROR( ...) LOG("ERROR", __VA_ARGS__);
#define WARNING( ...) LOG("WARNING", __VA_ARGS__);
#define INFO(...) LOG("INFO", __VA_ARGS__);
#define DEBUG(...) LOG("DEBUG", __VA_ARGS__);

#endif

