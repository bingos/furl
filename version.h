/*
 * chop
 * ( and it's nemesis unchop )
 * Copyright (C) 2017 BingosNET Produktions Ltd
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

#if defined __MINGW32__
#define COMPILER "GNU C Compiler mingw32"
#elif defined _MSC_VER
#define COMPILER "Microsoft Visual C++"
#elif defined __BORLANDC__
#define COMPILER "Borland C Compiler"
#elif defined __CYGWIN32__
#define COMPILER "Cygnus GNU C Compiler Win32"
#elif defined __GNUC__
#define COMPILER "GNU C Compiler"
#elif defined __HIGHC__
#define COMPILER "NCR High Performance C Compiler"
#else
#define COMPILER "Unknown"
#endif

#if defined CONF_MACHTYPE
#define BUILD_ENV CONF_MACHTYPE
#else
#define BUILD_ENV "Not defined"
#endif
