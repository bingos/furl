/*
 * furl
 * Copyright (C) 1999 BingosNET Produktions Ltd
 * Written by Kidney Bingos aka Chris Williams
 * <chris@bingosnet.co.uk>
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

#include <ctype.h>
#include <errno.h>
#include <sys/types.h>
#include <stdlib.h>
#include "furlutil.h"
#include "version.h"
#include "tailor.h"

/* ========================================================================
 * Return the base name of a file (remove any directory prefix and
 * any version suffix). For systems with file names that are not
 * case sensitive, force the base name to lower case.
 */
char *basename(fname)
    char *fname;
{
    char *p;

    if ((p = strrchr(fname, PATH_SEP))  != NULL) fname = p+1;
#ifdef PATH_SEP2
    if ((p = strrchr(fname, PATH_SEP2)) != NULL) fname = p+1;
#endif
#ifdef PATH_SEP3
    if ((p = strrchr(fname, PATH_SEP3)) != NULL) fname = p+1;
#endif
#ifdef SUFFIX_SEP
    if ((p = strrchr(fname, SUFFIX_SEP)) != NULL) *p = '\0';
#endif
    if (casemap('A') == 'a') strlwr(fname);
    return fname;
}

/* ========================================================================
 * Put string s in lower case, return s.
 */
char *strlwr(s)
    char *s;
{
    char *t;
    for (t = s; *t; t++) *t = tolow(*t);
    return s;
}

void PrintUsage(const char *prog_name)
{
	printf("Usage: %s [OPTIONS] [URL] [filename]\n",prog_name);
	printf("Where URL is in the format:\n\n");
	printf("      [http://]www.foobar.com[:80][/example.html]\n\n");
	printf("\n");
	printf("-f, --file     specify an input file containing URLs\n");
	printf("-b, --body     output the contents of HTTP request and the header\n");
	printf("-V, --version  output version information and exit\n");
	printf("-h, --help     display this help message and exit\n");
	printf("-i, --ie       pretend to be Internet Explorer\n");
	printf("-m, --mozilla  pretend to be Mozilla\n");
	printf("\nBug reports to furl-bugs@bingosnet.co.uk\n");
}

void PrintVersion(const char *prog_name)
{
	printf("%s\n\n", prog_name);
	printf("version:                    %s\n",VERSION);
	printf("compile date:               %s %s\n",__DATE__,__TIME__);
#if defined __GNUC__
	printf("compiler:                   %s %s\n",COMPILER,__VERSION__);
#else
	printf("compiler:                   %s\n",COMPILER);
#endif
	printf("environment:                %s\n",BUILD_ENV);
	printf("author:                     Kidney Bingos (chris@bingosnet.co.uk)\n");
}

int parseurl (char *url, char protocol[], char hostname[], char port[], char path[], char method[], char useragent[])
{
	char *m, *s;

	if (!strncmp(url,"http://",7)) {
		s = url + 7;
		strncpy(protocol,"http://",8);
	} else if (!strncmp(url,"ftp://",6)) {
		return -1;
	} else if (!strncmp(url,"gopher://",9)) {
		return -1;
	} else if (!strncmp(url,"raw://",6)) {
		return -1;
	} else if (!strncmp(url,"mailto://",9)) {
		return -1;
	} else if (!strncmp(url,"telnet://",9)) {
		return -1;
	} else {
		s = url;
		strncpy(protocol,"http://",8);
	}

	for (m=hostname;s[0]!=':'&&s[0]!='/'&&s[0];) {
		*m++ = *s++;
	}
	m[0] = '\0';
	m = port;
	if (s[0] == ':') {
		s++;
		do {
			*m++ = *s++;
		} while (isdigit(s[0]));
		*m = 0;
	} else {
		strcpy(m,"80");
	}
	sprintf(path,"%s %s HTTP/1.1\r\nConnection: close\r\nHost: %s\r\nAccept: */*\r\nUser-Agent: %s\r\n\r\n", method, (*s) ? s : "/", hostname, useragent);
	return 0;
}


int GetAddress (unsigned long *ipAddr, char hostname[])
{
	struct hostent *ent;
	unsigned long a,b,c,d;

	if (sscanf(hostname,"%ld.%ld.%ld.%ld", &a, &b, &c, &d) == 4) {

		*ipAddr = (a << 24) | (b << 16) | (c << 8) | d;
	} else {
		ent = gethostbyname(hostname);
		if (!ent) {
			return -1;
		}
		*ipAddr = ntohl(**((unsigned long**)ent->h_addr_list));
	}
	return 0;
}

SOCKET CreateSocket (unsigned long *ipAddr, char port[])
{

	struct sockaddr_in sa;
	SOCKET sock;

	sock = socket (AF_INET,SOCK_STREAM,IPPROTO_TCP);
	if (sock == INVALID_SOCKET) {
		return INVALID_SOCKET;
	}

	sa.sin_family = AF_INET;
	sa.sin_addr.s_addr = htonl(*ipAddr);
	sa.sin_port = htons(atoi(port));

	if (connect(sock,(struct sockaddr*)&sa,sizeof(sa)) == SOCKET_ERROR) {
#ifdef WIN32
		closesocket(sock);
#else
		close(sock);
#endif
		return INVALID_SOCKET;
	}

	return sock;

}

SOCKET ProcessSocket (SOCKET sock, char path[])
{

	char buffer[64*1024];
	SOCKET n;

	if (send(sock,path,strlen(path),0) == SOCKET_ERROR) {
		perror("Ooops:");
		return SOCKET_ERROR;
	} else {

		while ((n = recv(sock,buffer,sizeof(buffer),0)) > 0 ) {
			buffer[n] = 0;
			if (fputs(buffer,stdout) == EOF ) {
				fprintf(stderr,"Bad Magic. Couldn\'t write to STDOUT\n");
#ifdef WIN32
				closesocket(sock);
#else
				close(sock);
#endif
				return SOCKET_ERROR;
			}
		}
		if (n < 0) {
			return SOCKET_ERROR;
		} else {
			return 0;
		}
	}
}

#ifdef WIN32

void Win32Init (void)
{
	WSADATA wsaData;

	if (WSAStartup(MAKEWORD(2,0), &wsaData)) {
		fprintf(stderr,"Error initialising winsock.dll\n");
		exit(1);
	}
}

void Win32Cleanup (void)
{
	WSACleanup();
}

#endif
