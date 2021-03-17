FROM ubuntu:18.04

RUN apt-get -y update && apt-get -y upgrade
RUN apt-get -y install valgrind clang make

RUN mkdir -p /root/build
VOLUME /root/build
