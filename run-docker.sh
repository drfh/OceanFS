#!/bin/bash
docker build --tag oceanfs:latest --tag oceanfs:v1 .
docker run --rm -it -v ${PWD}:/usr/src/OceanFS oceanfs:latest
