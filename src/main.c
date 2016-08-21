/*
 * dvdbackup - tool to rip DVDs from the command line
 *
 * Copyright (C) 2002  Olaf Beck <olaf_sc@yahoo.com>
 * Copyright (C) 2008-2012  Benjamin Drung <benjamin.drung@gmail.com>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <config.h>
#include "dvdbackup.h"

/* internationalisation */
#include "gettext.h"
#define _(String) gettext(String)

/* C standard libraries */
#include <limits.h>
#include <locale.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* C POSIX libraries */
#include <sys/stat.h>
#include <unistd.h>

/* libdvdread */
#include <dvdread/dvd_reader.h>

/* other libraries */
#include <getopt.h>


void init_i18n() {
	setlocale(LC_ALL, "");
	bindtextdomain(PACKAGE, LOCALEDIR);
	textdomain(PACKAGE);
}


int main(void) {

	int do_mirror = 0;

	/* Because of copy protection you normally want to skip
	 * the defect sectors. To speed things up we skip multiblocks.
	 */
	read_error_strategy_t errorstrat = STRATEGY_SKIP_MULTIBLOCK;

	int return_code = 0;

	/* DVD Video device */
	char* dvd = "/dev/dvd";


	/* Title of the DVD */
	char title_name[33] = "";

	/* Targer dir */
	char* targetdir = ".";

	/* Temp filename,dirname */
	char targetname[PATH_MAX];
	struct stat fileinfo;

	/* The DVD main structure */
	dvd_reader_t* _dvd = NULL;

	init_i18n();

	/* TODO: do isdigit check */

	do_mirror = 1;
	progress = 1;
	dvd = "/dev/rdisk2";
	targetdir = "./test/post";

	aspect = 0;

	_dvd = DVDOpen(dvd);
	if (!_dvd) {
		fprintf(stderr,_("Cannot open specified device %s - check your DVD device\n"), dvd);
		exit(-1);
	}

	if (DVDGetTitleName(dvd,title_name) != 0) {
		fprintf(stderr,_("You must provide a title name when you read your DVD-Video structure direct from the HD\n"));
		DVDClose(_dvd);
		exit(1);
	}
	if (strstr(title_name, "DVD_VIDEO") != NULL) {
		fprintf(stderr,_("The DVD-Video title on the disk is DVD_VIDEO, which is too generic; please provide a title with the -n switch\n"));
		DVDClose(_dvd);
		exit(2);
	}

	sprintf(targetname,"%s",targetdir);

	if (stat(targetname, &fileinfo) == 0) {
		if (! S_ISDIR(fileinfo.st_mode)) {
			fprintf(stderr,_("The target directory is not valid; it may be an ordinary file.\n"));
		}
	} else {
		if (mkdir(targetname, 0777) != 0) {
			fprintf(stderr,_("Failed creating target directory %s\n"), targetname);
			perror("");
			DVDClose(_dvd);
			exit(-1);
		}
	}

	sprintf(targetname,"%s/%s",targetdir, title_name);

	if (stat(targetname, &fileinfo) == 0) {
		if (! S_ISDIR(fileinfo.st_mode)) {
			fprintf(stderr,_("The title directory is not valid; it may be an ordinary file.\n"));
		}
	} else {
		if (mkdir(targetname, 0777) != 0) {
			fprintf(stderr,_("Failed creating title directory\n"));
			perror("");
			DVDClose(_dvd);
			exit(-1);
		}
	}

	sprintf(targetname,"%s/%s/VIDEO_TS",targetdir, title_name);

	if (stat(targetname, &fileinfo) == 0) {
		if (! S_ISDIR(fileinfo.st_mode)) {
			fprintf(stderr,_("The VIDEO_TS directory is not valid; it may be an ordinary file.\n"));
		}
	} else {
		if (mkdir(targetname, 0777) != 0) {
			fprintf(stderr,_("Failed creating VIDEO_TS directory\n"));
			perror("");
			DVDClose(_dvd);
			exit(-1);
		}
	}


#ifdef DEBUG
	fprintf(stderr,"After dirs\n");
#endif


	if(do_mirror) {
		if ( DVDMirror(_dvd, targetdir, title_name, errorstrat) != 0 ) {
			fprintf(stderr, _("Mirror of DVD failed\n"));
			return_code = -1;
		} else {
			return_code = 0;
		}
	}
#ifdef DEBUG
	fprintf(stderr,"After Mirror\n");
#endif

	DVDClose(_dvd);
	exit(return_code);
}
