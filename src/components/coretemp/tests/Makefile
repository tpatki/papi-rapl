include ../../Makefile_comp_tests

%.o:%.c
	$(CC) $(CFLAGS) $(INCLUDE) -c -o $@ $<

TESTS = coretemp_basic coretemp_pretty

coretemp_tests: $(TESTS)

coretemp_basic: coretemp_basic.o $(UTILOBJS) $(PAPILIB)
	$(CC) $(CFLAGS) $(INCLUDE) -o coretemp_basic coretemp_basic.o $(UTILOBJS) $(PAPILIB) $(LDFLAGS) 

coretemp_pretty: coretemp_pretty.o $(UTILOBJS) $(PAPILIB)
	$(CC) $(CFLAGS) $(INCLUDE) -o coretemp_pretty coretemp_pretty.o $(UTILOBJS) $(PAPILIB) $(LDFLAGS) 


clean:
	rm -f $(TESTS) *.o





