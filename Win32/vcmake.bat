@echo off
call vcvars32
copy makefile.msc ..\Makefile
copy config.h ..
win32ver --def >> ..\config.h
chdir ..
nmake %1
