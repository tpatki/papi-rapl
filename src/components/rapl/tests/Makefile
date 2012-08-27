include ../../Makefile_comp_tests

TESTS = rapl_basic rapl_busy rapl_overflow rapl_write_tpatki rapl_write_multiple_tpatki rapl_mpi_basic rapl_mpi_basic_with_clamp

MPICC = mpicc

rapl_tests: $(TESTS)

rapl_print.o:	../print-utils/rapl_print.c  
	$(CC) $(CFLAGS) $(INCLUDE) -I../print-utils -c ../print-utils/rapl_print.c 

mpi-rapl-read-only.o:  
	./mpi/wrap.py -f -g -o ./mpi/mpi-rapl-read-only.c ./mpi/mpi-rapl-read-only.w
	$(MPICC) $(CFLAGS) $(INCLUDE) -I ./mpi -DPIC -fPIC -DARCH_SANDY_BRIDGE -DARCH_062D -c ./mpi/mpi-rapl-read-only.c

mpi-rapl-clamp-power.o:  
	./mpi/wrap.py -f -g -o ./mpi/mpi-rapl-clamp-power.c ./mpi/mpi-rapl-clamp-power.w
	$(MPICC) $(CFLAGS) $(INCLUDE) -I ./mpi -DPIC -fPIC -DARCH_SANDY_BRIDGE -DARCH_062D -c ./mpi/mpi-rapl-clamp-power.c

rapl_overflow.o:	rapl_overflow.c
	$(CC) $(CFLAGS) $(INCLUDE) -c rapl_overflow.c

rapl_overflow:	rapl_overflow.o $(UTILOBJS) $(PAPILIB)
	$(CC) $(CFLAGS) $(INCLUDE) -o rapl_overflow rapl_overflow.o $(UTILOBJS) $(PAPILIB) $(LDFLAGS) 

rapl_basic.o:	rapl_basic.c
	$(CC) $(CFLAGS) $(INCLUDE) -c rapl_basic.c -o rapl_basic.o -DBASIC_TEST=1

rapl_busy.o:	rapl_basic.c
	$(CC) $(CFLAGS) $(INCLUDE) -c rapl_basic.c -o rapl_busy.o -DBUSY_TEST=1

rapl_basic: rapl_basic.o rapl_print.o $(UTILOBJS) $(PAPILIB)
	$(CC) $(CFLAGS) $(INCLUDE) -o rapl_basic rapl_basic.o rapl_print.o $(UTILOBJS) $(PAPILIB) $(LDFLAGS) 

rapl_busy: rapl_busy.o rapl_print.o $(UTILOBJS) $(PAPILIB)
	$(CC) $(CFLAGS) $(INCLUDE) -o rapl_busy rapl_busy.o rapl_print.o $(UTILOBJS) $(PAPILIB) $(LDFLAGS) 

rapl_write_tpatki.o: rapl_write_tpatki.c  
	$(CC) $(CFLAGS) $(INCLUDE) -I ../print-utils -c rapl_write_tpatki.c 

rapl_write_tpatki: rapl_write_tpatki.o rapl_print.o $(UTILOBJS) $(PAPILIB)
	$(CC) $(CFLAGS) $(INCLUDE) -I ../print-utils -o rapl_write_tpatki rapl_write_tpatki.o rapl_print.o  $(UTILOBJS) $(PAPILIB) $(LDFLAGS) 


rapl_write_multiple_tpatki.o: rapl_write_multiple_tpatki.c
	$(CC) $(CFLAGS) $(INCLUDE) -c rapl_write_multiple_tpatki.c 

rapl_write_multiple_tpatki: rapl_write_multiple_tpatki.o rapl_print.o  $(UTILOBJS) $(PAPILIB)
	$(CC) $(CFLAGS) $(INCLUDE) -o rapl_write_multiple_tpatki rapl_write_multiple_tpatki.o rapl_print.o $(UTILOBJS) $(PAPILIB) $(LDFLAGS) 

rapl_mpi_basic.o: rapl_mpi_basic.c
	$(MPICC) $(CFLAGS) $(INCLUDE) -c rapl_mpi_basic.c

rapl_mpi_basic: rapl_mpi_basic.o rapl_print.o rapl_print.o mpi-rapl-read-only.o  $(UTILOBJS) $(PAPILIB)
	$(MPICC) $(CFLAGS) $(INCLUDE) -o rapl_mpi_basic rapl_mpi_basic.o rapl_print.o mpi-rapl-read-only.o $(UTILOBJS) $(PAPILIB) $(LDFLAGS) 

rapl_mpi_basic_with_clamp: rapl_mpi_basic.o rapl_print.o mpi-rapl-clamp-power.o  $(UTILOBJS) $(PAPILIB)
	$(MPICC) $(CFLAGS) $(INCLUDE) -o rapl_mpi_basic_with_clamp rapl_mpi_basic.o rapl_print.o mpi-rapl-clamp-power.o $(UTILOBJS) $(PAPILIB) $(LDFLAGS) 

clean:
	rm -f $(TESTS) *.o mpi/*.o mpi/*.c *~





