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

#include "dvdbackup.h"
#include "config.h"

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


void read_title_name(dvd_reader_t* _dvd, char* dvd, char* title_name, char* targetdir) {
	/* Temp filename,dirname */
	char targetname[PATH_MAX];
	struct stat fileinfo;

	if (DVDGetTitleName(dvd,title_name) != 0) {
		fprintf(stderr, "You must provide a title name when you read your DVD-Video structure direct from the HD\n");
		DVDClose(_dvd);
		exit(1);
	}
	if (strstr(title_name, "DVD_VIDEO") != NULL) {
		fprintf(stderr, "The DVD-Video title on the disk is DVD_VIDEO, which is too generic; please provide a title with the -n switch\n");
		DVDClose(_dvd);
		exit(2);
	}

	sprintf(targetname,"%s",targetdir);

	if (stat(targetname, &fileinfo) == 0) {
		if (! S_ISDIR(fileinfo.st_mode)) {
			fprintf(stderr, "The target directory is not valid; it may be an ordinary file.\n");
		}
	} else {
		if (mkdir(targetname, 0777) != 0) {
			fprintf(stderr, "Failed creating target directory %s\n", targetname);
			perror("");
			DVDClose(_dvd);
			exit(-1);
		}
	}

	sprintf(targetname,"%s/%s",targetdir, title_name);

	if (stat(targetname, &fileinfo) == 0) {
		if (! S_ISDIR(fileinfo.st_mode)) {
			fprintf(stderr, "The title directory is not valid; it may be an ordinary file.\n");
		}
	} else {
		if (mkdir(targetname, 0777) != 0) {
			fprintf(stderr, "Failed creating title directory\n");
			perror("");
			DVDClose(_dvd);
			exit(-1);
		}
	}

	sprintf(targetname,"%s/%s/VIDEO_TS",targetdir, title_name);

	if (stat(targetname, &fileinfo) == 0) {
		if (! S_ISDIR(fileinfo.st_mode)) {
			fprintf(stderr, "The VIDEO_TS directory is not valid; it may be an ordinary file.\n");
		}
	} else {
		if (mkdir(targetname, 0777) != 0) {
			fprintf(stderr, "Failed creating VIDEO_TS directory\n");
			perror("");
			DVDClose(_dvd);
			exit(-1);
		}
	}
}


int main(void) {

	int return_code = 0;

	/* DVD Video device */
	char* dvd = "/dev/dvd";

	/* Title of the DVD */
	char title_name[33] = "";

	/* Targer dir */
	char* targetdir = ".";

	/* The DVD main structure */
	dvd_reader_t* _dvd = NULL;

	/* TODO: do isdigit check */

	progress = 1;
	dvd = "/dev/rdisk3";
	targetdir = "./test/post";

	aspect = 0;

	_dvd = DVDOpen(dvd);
	if (!_dvd) {
		fprintf(stderr, "Cannot open specified device %s - check your DVD device\n", dvd);
		exit(-1);
	}

	read_title_name(_dvd, dvd, title_name, targetdir);

	if ( DVDMirror(_dvd, targetdir, title_name, STRATEGY_SKIP_MULTIBLOCK) != 0 ) {
		fprintf(stderr,  "Mirror of DVD failed\n");
		return_code = -1;
	} else {
		return_code = 0;
	}

	DVDClose(_dvd);
	exit(return_code);
}
