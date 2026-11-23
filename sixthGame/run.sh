#Remove the build folder
rm -rf build
#Contruct build folder
cmake -S . -B build

cmake --build build
