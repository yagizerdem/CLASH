rm -rf build
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build
rm -rf ./dist
mkdir ./dist
cp ./build/src/CLASH ./dist/CLASH