#!/bin/bash

echo -n "[~] Building winedetector.exe"

time x86_64-w64-mingw32-g++ main.cpp logger.cpp detector.cpp -o winedetector.exe -static -limagehlp -lntdll -O2 -pipe -flto -Wall #-march=native

echo "[~] Stripping winedetector.exe"

strip winedetector.exe