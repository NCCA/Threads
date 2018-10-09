#!/bin/bash
g++ -std=c++11 $1  -I../LoggerC++11 -L../LoggerC++11 -lNCCALogger -lpthread