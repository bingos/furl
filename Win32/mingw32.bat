@echo off
copy Makefile.mingw32 ..\Makefile
copy config.h ..
win32ver --def >> ..\config.h
chdir ..
dmake %1
