GCC = g++-4.9
CPPFLAGS = -g -std=gnu++14

log-test:
	$(GCC) src/log_test.cpp -o bin/logtest $(CPPFLAGS)

qr-test:
	$(GCC) src/qr_test.cpp -o bin/qrtest $(CPPFLAGS)

#piag:
#	$(GCC) src/main.cpp -o bin/piag -lwiringPi -lwiringPiDev -lpthread $(CPPFLAGS)
	
#test:
#	$(GCC) src/test.cpp -o bin/test $(CPPFLAGS)

docs:
	rm -r doc/
	doxygen etc/doxygen.config
	git reset
	git add doc/.
	git commit -m "Updated documentation."
	git push

count:
	wc -l src/*  # works assuming there's no subdirectories
