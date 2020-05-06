
Sqrat is a C++ binding utility for the Squirrel language.
See the docs directory for usage info.

Sqrat only contains C++ headers so for installation you just 
need to copy the files in the include directory to 
some common header path, such as /usr/local/include.

However, an autotool based build system is included which 
allows you to install, on POSIX systems, with the familiar

autotool/configure --prefix=<prefix path>
make && make install

You can then run

make check

to run the unit tests.

Alternatively you can follow the steps in 
running_tests.txt

to directly run unit tests.
