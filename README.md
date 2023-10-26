# OSRM CLI program

This is a (simple) CLI program to perform many routings using `libosrm`, the C++ library of the OpenSourceRoutingMachine (OSRM).

It can be used to compute shortest routes for many pairs where using the http-API has a too large overhead.

I tested this on Linux, on Mac it should also be possible to build it using the same instructions.

## Dependencies

The same dependencies are necessary as for building the OSRM toolchain, plus libosrm. So for instance running
``` sh
cmake .. -DCMAKE_BUILD_TYPE=Release -DBUILD_SHARED_LIBS=ON
cmake --build . && cmake --build . --target install
```
will build OSRM and libosrm and place binaries and libosrm in the respective system directories.

## Building

``` sh
cd src && make build
```

The compiled program is called osrm-cli.

## Running

In order to use the tool, it is necessary to prepare a map for routing first. See documentation of OSRM. Once, a map is prepared, create a CSV with all the lat-lon pairs for the routes and run

``` sh
osrm-cli /path/to/prepared/map.osrm latlon-pairs.csv
```

Make sure, that libosrm is in your PATH or make your current terminal session aware of its location using LD_LIBRARY_PATH.

The CSV file must have five columns, seperated by a semicolon \(`;`\) and no column names. The first column is an identifier, the second and third column are longitude and latitude of origin location and the fourth and fifth column are longitude and latitude of the destination location. The input must not have column names in its first line.

The utility sends its result (which is again a CSV file with `;` as seperator ) to `stdout`. The output also has five columns. The first one is the id, the second is distance in meters, the third duration in seconds, the fourth/fifth give the distance between origin/destination location and the closest point on the road graph.
