gcc -I"P:/BuildOrb/Libs/LibUSB-Win32/include/" -O -g -Wall -c opendevice.c
gcc -I"P:/BuildOrb/Libs/LibUSB-Win32/include/" -O -g -Wall -shared -c led.c
gcc -shared opendevice.o led.o -L"P:/BuildOrb/Libs/LibUSB-Win32/lib/gcc" -lusb -o led.dll