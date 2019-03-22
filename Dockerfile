#
#	Dockerfile for OceanFS build envirment
#	Author David R. F. Harris
#
FROM rikorose/gcc-cmake:latest
MAINTAINER davidrfharris@gmail.com
LABEL "org.oceanfs.www"="OceanFS LLC"
LABEL org.oceanfs.www="dev"
LABEL version="0.1"
LABEL description="Build environment."

RUN apt update
RUN apt upgrade -y
RUN apt install -y xxd
RUN apt install -y byobu
#RUN apt install –y byobu

RUN echo "xxd -c32 /dev/shm/simpleblocksd" >> ~/.bash_history
RUN echo "clear;cmake ..;make clean;make -j2 && ./simpleblocksd/simpleblocksd" >> ~/.bash_history

#COPY . /usr/src/OceanFS/
WORKDIR /usr/src/OceanFS/_build

CMD ["bash"]
