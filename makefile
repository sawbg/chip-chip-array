GCC = g++-4.9
CPPFLAGS = -g -std=gnu++14
CVFLAGS = $(shell pkg-config --cflags --libs opencv)

cv-test:
	$(GCC) src/cv_test.cpp -o bin/cvtest $(CVFLAGS) $(CPPFLAGS)

log-test:
	$(GCC) src/log_test.cpp -o bin/logtest $(CPPFLAGS)

qr-test:
	$(GCC) src/qr_test.cpp -o bin/qrtest $(CPPFLAGS)

log-viewer:
	$(GCC) src/log_viewer.cpp -o bin/lv $(CPPFLAGS)

docs:
	rm -r doc/
	doxygen etc/doxygen.config
	git reset
	git add doc/.
	git commit -m "Updated documentation."
	git push

count:
	find src/ -name '*' | xargs wc -l  # works assuming there's no subdirectories
