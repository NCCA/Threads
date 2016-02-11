#!/bin/bash
clang++ $1 -mmacosx-version-min=10.6 -I../Logger -L../Logger -lNCCALogger -lpthread -lboost_iostreams