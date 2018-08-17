#! /bin/bash\
avrdude -P /dev/tty.usbserial-A900991S -p m1280 -c stk500v2 -e -U flash:w:applet/nexus_main.cpp.hex