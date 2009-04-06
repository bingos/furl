/*
 * furl 
 * Copyright (C) 1999 BingosNET Produktions Ltd
 * Written by Kidney Bingos aka Chris Williams
 * <chris@kidney-bingos.demon.co.uk> 
 * With some components borrowed from the Free Software Foundation
 * 
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
*/

#include <config.h>

#ifdef WIN32
#include <windows.h>
#define ERR_NO WSAGetLastError
#else
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <unistd.h>
#define SOCKET int
#define INVALID_SOCKET -1
#define SOCKET_ERROR -1
#define ERR_NO errno
#endif

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define tolow(c)  (isupper(c) ? (c)-'A'+'a' : (c))    /* force to lower case */

extern char *basename(char *fname);
extern char *strlwr(char *s);
extern void PrintUsage(const char *prog_name);
extern void PrintVersion(const char *prog_name);
extern int parseurl(char *url, char protocol[], char hostname[], char port[], char path[], char method[], char useragent[]);
extern int GetAddress(unsigned long *ipAddr, char hostname[]);
extern SOCKET CreateSocket(unsigned long *ipAddr, char port[]);
extern SOCKET ProcessSocket(SOCKET sock, char path[]);

#ifdef WIN32
extern void Win32Init(void);
extern void Win32Cleanup(void);
#endif
