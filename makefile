GCC = g++-4.9
CPPFLAGS = -g -std=gnu++14
CVFLAGS = $(shell pkg-config --cflags --libs opencv)

configure:
	apt-get install -y libopencv-dev libzbar-dev

cv-test:
	$(GCC) src/cv_test.cpp -o bin/cvtest $(CVFLAGS) $(CPPFLAGS)

log-test:
	$(GCC) src/log_test.cpp -o bin/logtest $(CPPFLAGS)

qr-test:
	$(GCC) src/qr_test.cpp -o bin/qrtest $(CPPFLAGS)

docs:
	rm -r doc/
	doxygen etc/doxygen.config
	git reset
	git add doc/.
	git commit -m "Updated documentation."
	git push

count:
	find src/ -name '*' | xargs wc -l  # works assuming there's no subdirectories
