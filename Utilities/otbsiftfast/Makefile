all:
	@mkdir -p build
	@if [ $(prefix) ]; then \
		cd build && cmake -DCMAKE_INSTALL_PREFIX=$(prefix) -DCMAKE_BUILD_TYPE=Release ..; \
	else \
		cd build && cmake -DCMAKE_VERBOSE_MAKEFILE=OFF -DCMAKE_BUILD_TYPE=Release ..; \
	fi
	cd build && make

install:
	cd build && make install

uninstall:
	cd build && make uninstall

test: all
	cd build && make test

test-future: all
	cd build && make test-future

clean:
	-cd build && make clean
	rm -rf build
