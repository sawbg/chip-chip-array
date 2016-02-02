GCC = g++-4.9
CPPFLAGS = -g -std=gnu++14
CVFLAGS = -lraspicam -lraspicam_cv -lmmal -lmmal_core -lmmal_util -lzbar -lopencv_core -lopencv_highgui -lopencv_imgproc
DEBUG = -DDEBUG
LOG = -DLOG

export LIBRARY_PATH=/opt/vc/lib:/usr/lib/arm-linux-gnueabihf

block-test:
	$(GCC) src/cv_shape.cpp -o bin/cvshape $(CVFLAGS) $(CPPFLAGS)

configure:
	sudo apt-get install -y libopencv-dev libzbar-dev cmake doxygen
	git clone https://github.com/cedricve/raspicam
	cd raspicam; mkdir build; cd build; cmake ..; make; sudo make install; sudo ldconfig;
	sudo rm -r raspicam
	mkdir docs

cv-test:
	$(GCC) src/cv_test.cpp -o bin/cvtest $(CVFLAGS) $(CPPFLAGS) $(LOG)

log-test:
	$(GCC) src/log_test.cpp -o bin/logtest $(CPPFLAGS) $(LOG)

net-cv-hue-test:
	$(GCC) src/cv_hue.cpp -o bin/cvhue $(CPPFLAGS) $(CVFLAGS)

net-qr-test:
	$(GCC) src/net_qr_test.cpp -o bin/netqrtest $(CPPFLAGS) $(CVFLAGS)

qr-test:
	$(GCC) src/qr_test.cpp -o bin/qrtest $(CPPFLAGS) $(CVFLAGS) $(LOG)

docs:
	rm -r doc/
	doxygen etc/doxygen.config
	git reset
	git add doc/.
	git commit -m "Updated documentation."
	git push

count:
	find src/ -name '*' | xargs wc -l  # works assuming there's no subdirectories
