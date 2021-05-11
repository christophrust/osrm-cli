# OSRM CLI program

This is a (simple) CLI program to perform many routings using the `libosrm`, the c++ library of the OpenSourceRoutingMachine (OSRM).

It can be used to compute shortest routes for many pairs where using the http-API has a too large overhead.

I tested this on Linux, on Mac it should also be possible to build it using the same instructions.

## Dependencies

The same dependencies are necessary as for building the OSRM toolchain and libosrm. 

## Building

``` sh
cd src && make build
```

The compiled program is called osrm-example.

## Running

In order to use the tool, it is necessary to prepare a map for routing first. See documentation of OSRM. Once, a map is prepared, create a CSV with all the lat-lon pairs for the routes and run

``` sh
osrm-example /path/to/prepared/map.osrm latlon-pairs.csv
```

Make sure, that libosrm is in your PATH or make your current terminal session aware of its location using LD_LIBRARY_PATH.
