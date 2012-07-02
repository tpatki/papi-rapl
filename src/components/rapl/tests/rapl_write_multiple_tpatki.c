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
#define NUM_EVENTS 4
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
    int r,i, j;
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




     /* Add the MSR_PKG_POWER_LIMIT: PACKAGE0 event */
	code[0] = PAPI_NATIVE_MASK; 

	retval = PAPI_event_name_to_code("PACKAGE_POWER_LIMIT:PACKAGE0", &code[0]);
        if ( retval != PAPI_OK ) {
           test_fail( __FILE__, __LINE__, "PACKAGE_POWER_LIMIT:PACKAGE0 not found\n",retval );
        }


     /* Add the PACKAGE_ENERGY:  PACKAGE0 event */

	code[1] = PAPI_NATIVE_MASK; 
        retval = PAPI_event_name_to_code("PACKAGE_ENERGY:PACKAGE0", &code[1]);
        if ( retval != PAPI_OK ) {
           test_fail( __FILE__, __LINE__, "PACKAGE_ENERGY:PACKAGE0 not found\n",retval );
        }


     /* Add the PACKAGE_POWER_LIMIT: PACKAGE1 event */
	code[2] = PAPI_NATIVE_MASK; 

        retval = PAPI_event_name_to_code("PACKAGE_POWER_LIMIT:PACKAGE1", &code[2]);
        if ( retval != PAPI_OK ) {
           test_fail( __FILE__, __LINE__, "PACKAGE_POWER_LIMIT:PACKAGE1 not found\n",retval );
        }

 /* Add the PACKAGE_ENERGY:  PACKAGE1 event */
       code[3] = PAPI_NATIVE_MASK;

        retval = PAPI_event_name_to_code("PACKAGE_ENERGY:PACKAGE1", &code[3]);
        if ( retval != PAPI_OK ) {
           test_fail( __FILE__, __LINE__, "PACKAGE_ENERGY:PACKAGE1 not found\n",retval );
        }

/*Add all events*/

	retval = PAPI_add_events(EventSet, code, num_events);

	if(retval != PAPI_OK) {
		test_fail(__FILE__, __LINE__, "PAPI_add_events()\n", retval);
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
	for(j=0;j<num_events;j++){
		printf("\nValues [%d] = %ld (%x) ", (int)j, values[j], values[j]);
	}	


	printf("\n Clamping power to 55W and then running the test"); 

	values[0] = (long long) (0x38198); 
	values[2] = (long long) (0x38198);

	
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
	for(j=0;j<num_events;j++){
		printf("Values [%d] = %ld (%x)\n", (int)j, values[j], values[j]);
	}	

     /* Stop Counting */
     after_time=PAPI_get_real_nsec();
     retval = PAPI_stop( EventSet, values);
     if (retval != PAPI_OK) {
	test_fail(__FILE__, __LINE__, "PAPI_stop()",retval);
     }

     elapsed_time=((double)(after_time-before_time))/1.0e9;

	/*Print Info*/
	rapl_print_info(code,num_events, elapsed_time, values,0);

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

