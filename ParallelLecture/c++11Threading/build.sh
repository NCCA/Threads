#!/bin/bash
clang++ -std=c++11 $1  -I../LoggerC++11 -L../LoggerC++11 -lNCCALogger -I/usr/local/include