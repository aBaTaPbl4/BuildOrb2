cd avrdude 
avrdude -vv -p ATtiny45 -c avrispmkII -P usb -U lfuse:w:0xe1:m -U hfuse:w:0xdd:m