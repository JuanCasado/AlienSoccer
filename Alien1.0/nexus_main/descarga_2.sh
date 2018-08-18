#! /bin/bash\
avrdude -P /dev/tty.usbserial-A6007mH0 -p m1280 -c stk500 -e -U flash:w:applet/nexus_main.cpp.hex