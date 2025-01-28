if (Test-Path -Path build) {
    Remove-Item ./build -r -force
}
mkdir build
g++ project/src/**.cpp -Iproject/inc -Iproject/src -lOpenCL -o build/main.exe