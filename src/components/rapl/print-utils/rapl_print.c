#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "rapl_print.h"

void rapl_print_info(int *code, int num_events, double elapsed, long long *values, int uid){
	
	int i=0;
	int r; 
	/* MAX_RAPL_EVENTS is 64 but is not exported externally */
	char event_names[MAX_RAPL_EVENTS][PAPI_MAX_STR_LEN];
    	char units[MAX_RAPL_EVENTS][PAPI_MIN_STR_LEN];
	PAPI_event_info_t evinfo;
	int retval;
	char hostname[1025];
	char filetag[2048];
	FILE *fp=NULL;
	
	gethostname(hostname, 1024);
	sprintf(filetag, "%s_rapl_%d.out", hostname, uid);
	fp = fopen(filetag, "w");
	if(fp==NULL){
		fprintf(stderr, "Error: Could not open file %s. Printing to screen.", filetag);
		
		for(i=0;i<num_events;i++) {
		        retval = PAPI_event_code_to_name(code[i], event_names[i] );
	       		 if ( retval != PAPI_OK ) {
        		   printf("Error translating %x\n",code[i]);
           		   test_fail( __FILE__, __LINE__, "PAPI_event_code_to_name", retval );
       		 	}

       		 	retval = PAPI_get_event_info(code[i],&evinfo);
        		if (retval != PAPI_OK) {
          			test_fail( __FILE__, __LINE__,"Error getting event info\n",retval);
        		}

        		strncpy(units[i],evinfo.units,PAPI_MIN_STR_LEN);

		}

		printf("\n*************** BEGIN RAPL INFO ***************");

        	printf("\nEnergy measurements:\n");

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
	   		if(strstr(units[i],"R")){
				printf("%s\t0x%x\n", event_names[i], values[i]);
	   		}
           	if (!strstr(units[i],"nJ") && !strstr(units[i],"R")) {

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

		printf("\n*************** END RAPL INFO ***************\n");
	} /*End of if(fp==NULL) */
	
	else { /*Write to a file */
	
	
		for(i=0;i<num_events;i++) {
		        retval = PAPI_event_code_to_name(code[i], event_names[i] );
	       		 if ( retval != PAPI_OK ) {
        		   printf("Error translating %x\n",code[i]);
           		   test_fail( __FILE__, __LINE__, "PAPI_event_code_to_name", retval );
       		 	}

       		 	retval = PAPI_get_event_info(code[i],&evinfo);
        		if (retval != PAPI_OK) {
          			test_fail( __FILE__, __LINE__,"Error getting event info\n",retval);
        		}

        		strncpy(units[i],evinfo.units,PAPI_MIN_STR_LEN);

		}

		fprintf(fp,"\n*************** BEGIN RAPL INFO ***************");

        	fprintf(fp,"\nEnergy measurements:\n");

		for(i=0;i<num_events;i++) {
           		if (strstr(units[i],"nJ")) {

              		fprintf(fp,"%s\t%.6fJ\t(Average Power %.1fW)\n",
                    		event_names[i],
                    		(double)values[i]/1.0e9,
                    		((double)values[i]/1.0e9)/elapsed);
           		}
        	}

        	fprintf(fp,"\n");
       		fprintf(fp,"Fixed values:\n");

        	for(i=0;i<num_events;i++) {
	   		if(strstr(units[i],"R")){
				fprintf(fp,"%s\t0x%x\n", event_names[i], values[i]);
	   		}
           	if (!strstr(units[i],"nJ") && !strstr(units[i],"R")) {

             	union {
               		long long ll;
               		double fp;
             	} result;

             	result.ll=values[i];

              	fprintf(fp,"%s\t%0.3f%s\n",
                	    event_names[i],
                    		result.fp,
                    		units[i]);

		}

	   }
	fclose(fp);
   }/*End of else*/
}


void rapl_print_info_event_names(char event_names[MAX_RAPL_EVENTS][PAPI_MAX_STR_LEN], int num_events, double elapsed, long long *values, int uid){
	
	int i=0;
	int r; 
	int code;
    	char units[MAX_RAPL_EVENTS][PAPI_MIN_STR_LEN];
	PAPI_event_info_t evinfo;
	int retval;

	for(i=0;i<num_events;i++) {
	        retval = PAPI_event_name_to_code(event_names[i],&code );
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

	printf("\n*************** BEGIN RAPL INFO ***************");

        printf("\nEnergy measurements:\n");

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
	   if(strstr(units[i],"R")){
		printf("%s\t0x%x\n", event_names[i], values[i]);
	   }
           if (!strstr(units[i],"nJ") && !strstr(units[i],"R")) {

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

	printf("\n*************** END RAPL INFO ***************\n");
}

