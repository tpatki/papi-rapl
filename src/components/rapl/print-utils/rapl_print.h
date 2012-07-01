#ifndef RAPL_PRINT_H
#define RAPL_PRINT_H

#include "papi_test.h"

/*Not exported externally so need to redefine it here*/
#define MAX_RAPL_EVENTS 64

void rapl_print_info(int *code, int num_events, double elapsed, long long *values, int uid);
void rapl_print_info_event_names(char event_names[MAX_RAPL_EVENTS][PAPI_MAX_STR_LEN], int num_events, double elapsed, long long *values, int uid);

#endif 
