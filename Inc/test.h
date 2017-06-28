#ifndef _TEST_H
#define _TEST_H

#include <stdbool.h>

#define SAMPLE_NUM 8192
#define DEBUG_MODE 0

#define dprintf(...) \
do \
{ \
if (DEBUG_MODE == 1) \
    printf(__VA_ARGS__); \
} while(0)


extern bool g_record_finished;
extern int  g_num_samples;

void dump_sample_times();


#endif