##
# ##
# Project Title
#
# @file
# @version 0.1
build:
	mkdir -p build && cd build && rm -rf * && cmake .. && cmake --build .


run:
	LD_LIBRARY_PATH=/usr/local/lib/ ./build/osrm-cli /tmp/germany-mld/germany-latest.osrm latlon.csv

clean:
	rm -rf build

# end
