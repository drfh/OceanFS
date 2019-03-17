### Docker build environment setup
.

## Docker environment cleanup
.

## Active developmet
docker run --rm -it -v ${PWD}:/usr/src/OceanFS -w "/usr/src/OceanFS/\_build/" rikorose/gcc-cmake:latest bash

\_build# clear; cmake ..; make clean; make -j;


## URI (WIP) (RFC)

cluster://{admin:(MD5:password)}@(SHA265:cluster UUID)/{IP hint list}/Pool:(SHA265:pool UUID))/(SHA265:Meta Block Hash)

cluster://{admin:d996166114cb81197bac8481784bd3fa}@e3b0c44298fc1c149afbf4c8996fb92427ae41e4649b934ca495991b7852b855/{10.10.0.1/24,cluster.oceanfs.com:9090}/Pool:f0dde0b4a1b085722434a06756a880e1a30605a59bc9c594f3f00c3723d82f45/3b43717c562bce9a7e4fb710187bc955432132aedf97ae62f116d83e89cf4d3c/

# Connection using saved/cached password and auto peer discovery
cluster://{admin}@e3b0c44298fc1c149afbf4c8996fb92427ae41e4649b934ca495991b7852b855/{auto}/Pool:f0dde0b4a1b085722434a06756a880e1a30605a59bc9c594f3f00c3723d82f45/3b43717c562bce9a7e4fb710187bc955432132aedf97ae62f116d83e89cf4d3c/
