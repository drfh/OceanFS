### Docker build environment setup
docker build --no-cache -t storage_dev .

## Docker environment cleanup
docker system prune -a --volumes --filter 'label=storage_dev'

## Active developmet
docker run --rm -it -v ${PWD}:/usr/src/storage -w /usr/src/storage storage_dev bash


## Make all targets
docker run --rm -v /usr/src/storage -w /usr/src/storage storage_dev make all

## Make just the library
docker run --rm -v /usr/src/storage -w /usr/src/storage storage_dev make lib

## Make clean
docker run --rm -v /usr/src/storage -w /usr/src/storage storage_dev make clean
