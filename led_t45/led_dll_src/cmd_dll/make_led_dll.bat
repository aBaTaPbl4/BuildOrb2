gcc -I"C:/LibUSB/include/" -O -g -Wall -c opendevice.c
gcc -I"C:/LibUSB/include/" -O -g -Wall -shared -c led.c
gcc -shared opendevice.o led.o -L"C:/LibUSB/lib/gcc" -lusb -o led.dll