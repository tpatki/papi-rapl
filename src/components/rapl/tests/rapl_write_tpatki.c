/****************************/
/* THIS IS OPEN SOURCE CODE */
/****************************/

/** 
 * @author  Tapasya Patki
 * 
 * test case for RAPL component 
 * 
 * @brief
 * Sequential test: Tests the write function in rapl component 
*/

#include <stdio.h>
#include <stdlib.h>
#include "papi_test.h"

#define MAX_RAPL_EVENTS 64
#define NUM_EVENTS 1 
void run_test(int quiet) {

     if (!quiet) {
	printf("\nSleeping 2 second...\n");
     }

     /* Sleep */
     sleep(2);
}


int main (int argc, char **argv)
{

    int retval,cid,rapl_cid=-1,numcmp;
    int EventSet = PAPI_NULL;
    long long *values;
    int num_events=NUM_EVENTS;
    int code[NUM_EVENTS];
    char event_names[MAX_RAPL_EVENTS][PAPI_MAX_STR_LEN];
    char units[MAX_RAPL_EVENTS][PAPI_MIN_STR_LEN];
    int r,i;
    const PAPI_component_info_t *cmpinfo = NULL;
    PAPI_event_info_t evinfo;
    long long before_time,after_time;
    double elapsed_time;


        /* Set TESTS_QUIET variable */
     tests_quiet( argc, argv );      

	/* PAPI Initialization */
     retval = PAPI_library_init( PAPI_VER_CURRENT );
     if ( retval != PAPI_VER_CURRENT ) {
	test_fail(__FILE__, __LINE__,"PAPI_library_init failed\n",retval);
     }

     if (!TESTS_QUIET) {
	printf("Trying all RAPL events\n");
     }

     numcmp = PAPI_num_components();

     for(cid=0; cid<numcmp; cid++) {

	if ( (cmpinfo = PAPI_get_component_info(cid)) == NULL) {
	   test_fail(__FILE__, __LINE__,"PAPI_get_component_info failed\n", 0);
	}

	if (strstr(cmpinfo->name,"linux-rapl")) {
	   rapl_cid=cid;
	   if (!TESTS_QUIET) printf("Found rapl component at cid %d\n",
				    rapl_cid);
           if (cmpinfo->num_native_events==0) {
              test_skip(__FILE__,__LINE__,"No rapl events found",0);
           }
	   break;
	}
     }

     /* Component not found */
     if (cid==numcmp) {
       test_skip(__FILE__,__LINE__,"No rapl component found\n",0);
     }



     /* Create EventSet */
     retval = PAPI_create_eventset( &EventSet );
     if (retval != PAPI_OK) {
	test_fail(__FILE__, __LINE__, "PAPI_create_eventset()",retval);
     }


	code[0] = PAPI_NATIVE_MASK; 

     /* Add the MSR_PKG_POWER_LIMIT event */

	retval = PAPI_event_name_to_code("PACKAGE_POWER_LIMIT:PACKAGE0", &code[0]);
        if ( retval != PAPI_OK ) {
           test_fail( __FILE__, __LINE__, "PACKAGE_POWER_LIMIT:PACKAGE0 not found\n",retval );
        }

	retval = PAPI_add_event( EventSet, code[0]);
        if ( retval != PAPI_OK ) {
           test_fail( __FILE__, __LINE__, "PAPI_add_events failed\n", retval );
	}
     

     values=calloc(num_events,sizeof(long long));
     if (values==NULL) {
	test_fail(__FILE__, __LINE__, "No memory",retval);
     }

     if (!TESTS_QUIET) {
	printf("\nStarting measurements...\n\n");
     }



     /* Start Counting */
     before_time=PAPI_get_real_nsec();
     retval = PAPI_start( EventSet);
     if (retval != PAPI_OK) {
	test_fail(__FILE__, __LINE__, "PAPI_start()",retval);
     }

	/*Read the original values*/
	retval = PAPI_read(EventSet, values);
       if ( retval != PAPI_OK ) {
           test_fail( __FILE__, __LINE__, "PAPI_read failed\n",retval );
        }
 
	printf("\nBEFORE PAPI_write(): Values are:");
		printf("\nValues [%d] = %ld (%x) ", (int)0, values[0], values[0]);


	printf("\n Clamping power to 55W and then running the test"); 

	values[0] = (long long) (0x38198); 

	
	retval = PAPI_write ( EventSet, values );
        if ( retval != PAPI_OK ) {
           test_fail( __FILE__, __LINE__, "PAPI_write failed\n",retval );
        }


     /* Run test */
     run_test(TESTS_QUIET);

	/*Read the values again */

     retval = PAPI_read(EventSet, values); 
     if ( retval != PAPI_OK ) {
           test_fail( __FILE__, __LINE__, "PAPI_read failed\n",retval );
        }


	printf("AFTER PAPI_write(): Values are:\n");
		printf("Values [%d] = %ld (%x)\n", (int)0, values[0], values[0]);

     /* Stop Counting */
     after_time=PAPI_get_real_nsec();
     retval = PAPI_stop( EventSet, values);
     if (retval != PAPI_OK) {
	test_fail(__FILE__, __LINE__, "PAPI_stop()",retval);
     }

     elapsed_time=((double)(after_time-before_time))/1.0e9;


    /*try calling the write function */
	rapl_print_info(code,num_events,elapsed_time,values,0);


     /* Done, clean up */

     retval = PAPI_cleanup_eventset( EventSet );
     if (retval != PAPI_OK) {
	test_fail(__FILE__, __LINE__, "PAPI_cleanup_eventset()",retval);
     }

     retval = PAPI_destroy_eventset( &EventSet );
     if (retval != PAPI_OK) {
	test_fail(__FILE__, __LINE__, "PAPI_destroy_eventset()",retval);
     }
        
     test_pass( __FILE__, NULL, 0 );
		
     return 0;
}

