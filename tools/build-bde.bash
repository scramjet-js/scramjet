#!/usr/bin/env bash
cd ..
git submodule update --init .
cd src/ext/bde
../bde-tools/bin/waf configure --build-type release
../bde-tools/bin/waf build
