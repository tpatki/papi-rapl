#include <stdio.h>
#include <stdlib.h>
#include "rapl_print.h"
#include "papi_test.h"

void rapl_print_info(int code, int num_events, double elapsed, long long *values){
	
	int i=0;
	int r; 
	/* MAX_RAPL_EVENTS is 64 but is not exported externally */
	char event_names[64][PAPI_MAX_STR_LEN];
    	char units[64][PAPI_MIN_STR_LEN];
	PAPI_event_info_t evinfo;
	int retval;

	for(i=0;i<num_events;i++) {
	        retval = PAPI_event_code_to_name( code, event_names[i] );
	        if ( retval != PAPI_OK ) {
        		   printf("Error translating %x\n",code);
           		   test_fail( __FILE__, __LINE__, "PAPI_event_code_to_name", retval );
       		 }

       		 retval = PAPI_get_event_info(code,&evinfo);
        	if (retval != PAPI_OK) {
          		test_fail( __FILE__, __LINE__,"Error getting event info\n",retval);
        	}

        	strncpy(units[i],evinfo.units,PAPI_MIN_STR_LEN);

	}

	printf("\n************** BEGIN RAPL INFO ***************");

        printf("Energy measurements:\n");

	for(i=0;i<num_events;i++) {
           if (strstr(units[i],"nJ")) {

              printf("%s\t%.6fJ\t(Average Power %.1fW)\n",
                    event_names[i],
                    (double)values[i]/1.0e9,
                    ((double)values[i]/1.0e9)/elapsed);
           }
        }

        printf("\n");
        printf("Fixed values:\n");

        for(i=0;i<num_events;i++) {
           if (!strstr(units[i],"nJ")) {

             union {
               long long ll;
               double fp;
             } result;

             result.ll=values[i];

              printf("%s\t%0.3f%s\n",
                    event_names[i],
                    result.fp,
                    units[i]);
           }
        }

	printf("\n************** END RAPL INFO ***************");
}

