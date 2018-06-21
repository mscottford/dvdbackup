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
#include <climits>
#include <clocale>
#include <cstdio>
#include <cstdlib>

/* C POSIX libraries */
#include <sys/stat.h>
#include <unistd.h>

/* libdvdread */
#include <dvdread/dvd_reader.h>

/* other libraries */
#include <getopt.h>

#include <algorithm>
#include <string>
#include <vector>


void read_title_name(dvd_reader_t* _dvd, char* dvd, char* title_name) {
    if (DVDGetTitleName(dvd,title_name) != 0) {
        fprintf(stderr, "You must provide a title name when you read your DVD-Video structure direct from the HD\n");
        DVDClose(_dvd);
        exit(1);
    }
}

// source: https://stackoverflow.com/a/868894/243215
class InputParser {
public:
    InputParser (int &argc, char **argv) {
        for (int i=1; i < argc; ++i)
            this->tokens.push_back(std::string(argv[i]));
    }

    /// @author iain
    const std::string& getCmdOption(const std::string &option) const {
        std::vector<std::string>::const_iterator itr;
        itr =  std::find(this->tokens.begin(), this->tokens.end(), option);
        if (itr != this->tokens.end() && ++itr != this->tokens.end()){
            return *itr;
        }
        static const std::string empty_string("");
        return empty_string;
    }

    /// @author iain
    bool cmdOptionExists(const std::string &option) const {
        return std::find(this->tokens.begin(), this->tokens.end(), option)
               != this->tokens.end();
    }

private:
    std::vector <std::string> tokens;
};


int main(int argc, char* argv[]) {

    int return_code = 0;

    InputParser input(argc, argv);
    if (argc != 2 || input.cmdOptionExists("-h")) {
        printf("Usage\n"
               "\n"
               "  dvdreadtitle <bsd-device-path>\n"
               "\n"
        );
        exit(return_code);
    }

    /* DVD Video device */
    char* dvd = argv[1];

    /* Title of the DVD */
    char title_name[33] = "";

    /* The DVD main structure */
    dvd_reader_t* _dvd = nullptr;

    _dvd = DVDOpen(dvd);
    if (!_dvd) {
        fprintf(stderr, "Cannot open specified device %s - check your DVD device\n", dvd);
        exit(-1);
    }

    read_title_name(_dvd, dvd, title_name);
    fprintf(stdout, "title: %s", title_name);

    DVDClose(_dvd);
    exit(return_code);
}
