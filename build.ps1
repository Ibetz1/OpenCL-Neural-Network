if (Test-Path -Path build) {
    Remove-Item ./build -r -force
}
mkdir build
Copy-Item -Path "project/src/kernels/*" -Destination build
g++ project/src/**.cpp -Iproject/inc -Iproject/src -lOpenCL -o build/main.exe

Set-Location ./build
./main.exe
Set-Location ..