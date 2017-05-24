cd ..
git submodule update --init .
cd src/ext/bde
../bde-tools/bin/waf configure
../bde-tools/bin/waf build
