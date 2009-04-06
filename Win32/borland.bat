@echo off
copy Makefile.borland ..\Makefile
copy config.h ..
win32ver --def >> ..\config.h
chdir ..
dmake %1
