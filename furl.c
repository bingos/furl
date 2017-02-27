/*
 * furl
 * Copyright (C) 2017 BingosNET Produktions Ltd
 * Written by Kidney Bingos aka Chris Williams
 * <chris@bingosnet.co.uk>
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

#ifndef WIN32
#include <stdio.h>
#endif
#include <stdlib.h>
#include <string.h>
#include "getopt.h"
#include "furlutil.h"

int main(int argc, char *argv[])
{

	static struct option options[] =
	{
		{"body", no_argument,NULL,'b'},
		{"version", no_argument,NULL,'V'},
		{"file", required_argument,NULL,'f'},
		{"help",    no_argument,NULL,'h'},
		{"ie", no_argument,NULL,'i'},
		{"mozilla", no_argument,NULL, 'm'},
	};

	FILE *in = 0;
	char opt;
	char *optstring = "imbVhf:";
	char *progname;
	int file_count, fileind, length;
	int file_open = 0;
	char protocol[1024];
	char port[20];
	char path[1024];
	char hostname[1024];
	char fileurl[1024];
	char method[5] = "HEAD";
	char useragent[100] = "Furl/3.0";

	/* Let's see if it works */
	unsigned long ipAddr;
	SOCKET sock;

#ifdef WIN32
	Win32Init();
#endif

	progname = basename(argv[0]);
	strlwr(progname);

	while ((opt=getopt_long(argc,argv,optstring,options,NULL)) != EOF ) {
		switch (opt) {

			case 'i':
				strcpy(useragent, "Mozilla/4.0 (compatible; MSIE 6.0; Windows NT 5.1;)");
				break;
			case 'm':
				strcpy(useragent, "Mozilla/5.0 (Windows; U; Windows NT 5.1; en-US; rv:1.7b) Gecko/20040421");
				break;
			case 'b':
				strcpy(method, "GET");
				break;
			case 'V':
				PrintVersion(progname);
				return 0;

			case 'h':
				PrintUsage(progname);
				return 0;

			case 'f':
				if((in=fopen(optarg, "r")) == NULL) {
					fprintf(stderr,"Bad Magic. Can\'t open specified file %s\n", optarg);
#ifdef WIN32
					Win32Cleanup();
#endif
					exit(1);
				} else {
					file_open = 1;
				}
				break;

			default:
				fprintf(stderr,"Try '%s --help' for more information.\n",progname);
				return 1;
		}
	}

	file_count = argc - optind;

	fileind = optind++;

	if((file_count == 0  || file_count > 1) && !file_open) {

		PrintUsage(progname);
#ifdef WIN32
		Win32Cleanup();
#endif
		exit(1);

	} else {

		while(1) {
			if (file_open) {
				if(fgets(fileurl,1022,in) == NULL) {
					if (feof(in)) {
						break;
					} else {
						perror("Bad Magic ");
#ifdef WIN32
						Win32Cleanup();
#endif
						exit(1);
					}
				} else {
					length = strlen(fileurl);
					fileurl[length-1] = '\0';
					fprintf(stdout,"\nURL= %s\n\n", fileurl);
				}
			} else {
				length = -1;
				strcpy(fileurl, argv[fileind]);
			}
			if (length > 1 || length == -1) {
				if (!parseurl(fileurl,protocol,hostname,port,path,method,useragent)) {

					sock = CreateSocket(hostname,port);
					if (sock == INVALID_SOCKET) {
						fprintf(stderr,"Bad Magic. Couldn\'t get a socket connection\n");
					} else {
						if (ProcessSocket(sock, path) == SOCKET_ERROR) {
							fprintf(stderr,"Bad Magic. Encountered an error connecting to socket\n");
#ifdef WIN32
							Win32Cleanup();
#endif
							exit(1);
						}
					}

				} else {
					fprintf(stderr,"Protocol not supported\n");
					fprintf(stderr,"Try '%s --help' for more information.\n",progname);
				}
			}
			if (!file_open) {
				break;
			}
		}
	}
#ifdef WIN32
	Win32Cleanup();
#endif
	exit(0);
}
