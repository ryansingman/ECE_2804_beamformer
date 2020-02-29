build:
	g++ -o test_main test_main.cpp beamformer.cpp

clean:
	rm test_main

run:
	./test_main

.PHONY: build clean run
