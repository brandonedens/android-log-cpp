/** @file device.cc - implementation of software represents android device. */

/** @file device.cc @verbatim
 *==============================================================================
 * Copyright (C) 2012 Brandon Edens - All Rights Reserved
 *==============================================================================
 *
 * android-log is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * android-log is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with android-log. If not, see <http://www.gnu.org/licenses/>.
 *
 * Project: android-log
 * Date: 2012-08-22
 * Author: Brandon Edens <brandonedens@gmail.com>
 *
 * @endverbatim
 */


/*******************************************************************************
 * includes
 */
#include "device.h"

#include <boost/regex.hpp>
#include <tbb/mutex.h>


/*******************************************************************************
 * defines
 */

/** Maxmium number of characters in a read line. */
#define LINE_NCHARS (1024)


/*******************************************************************************
 * prototypes
 */

static enum tag_color next_tag_color(enum tag_color current);


/******************************************************************************
 * constructors / destructors
 */

Device::Device(const std::string serial)
    : file_(NULL),
      serial_number_(serial) {
}


/******************************************************************************
 * public methods
 */

void Device::init(void) {
}

void Device::run(void) {
    static std::map<std::string, enum tag_color> tags;
    static tbb::mutex tags_mutex;
    static enum tag_color last_color;
    static tbb::mutex last_color_mutex;
    static tbb::mutex stdout_mutex;

    tbb::mutex::scoped_lock tags_lock;
    tags_lock.acquire(tags_mutex);
    if (tags.size() == 0) {
        // on first usage of tags we populate it with default values.
        tags[std::string("dalvikvm")] = TAG_BLUE;
        tags[std::string("Process")] = TAG_BLUE;
        tags[std::string("ActivityManager")] = TAG_CYAN;
        tags[std::string("ActivityThread")] = TAG_CYAN;
        // and set the last color to red.
        last_color = TAG_RED;
    }
    tags_lock.release();

    char cmd[256];
    snprintf(cmd, sizeof(cmd), "adb -s %s logcat -v time",
             serial_number_.c_str());
    file_ = popen(cmd, "r");

    boost::regex line_regex;

    line_regex.assign("^([0-9]{2}-[0-9]{2} "
                      "[0-9]{2}:[0-9]{2}:[0-9]{2}\\.[0-9]{3}) "
                      "([A-Z])/([^\\(]+)\\(([^\\)]+)\\): (.*)$",
                      boost::regex::egrep);

    char line[LINE_NCHARS];
    while (fgets(line, sizeof(line), file_) != NULL) {
        boost::match_results<const char *> results;
        tbb::mutex::scoped_lock stdout_lock;
        stdout_lock.acquire(stdout_mutex);

        if (boost::regex_match(line, results, line_regex)) {
            // print device
            printf("\e[32m%-16.16s\e[0m", serial_number_.c_str());

            // print time
            printf(" \e[34m%s\e[0m", results[TIME].str().c_str());

            // print owner
            printf(" \e[30;100m%s\e[0m", results[OWNER].str().c_str());

            // print tag
            std::string tag = results[TAG];
            enum tag_color color;
            tags_lock.acquire(tags_mutex);
            if (tags.find(tag) == tags.end()) {
                tbb::mutex::scoped_lock last_color_lock;
                last_color_lock.acquire(last_color_mutex);
                // this tag does not exist in the map; add it.
                color = next_tag_color(last_color);
                last_color = color;
                last_color_lock.release();
                tags[tag] = color;
            } else {
                // gather the existing color
                color = tags[tag];
            }
            tags_lock.release();
            printf(" \e[%dm%-20.20s\e[0m", color, tag.c_str());

            // print tagtype
            std::string tag_type = results[TAGTYPE].str();
            putchar(' ');
            switch (*(tag_type.c_str())) {
            case 'D':
                printf("\e[30;44m D \e[0m");
                break;
            case 'E':
                printf("\e[30;41m E \e[0m");
                break;
            case 'F':
                printf("\e[5;30;41m F \e[0m");
                break;
            case 'I':
                printf("\e[30;42m I \e[0m");
                break;
            case 'V':
                printf("\e[37m V ");
                break;
            case 'W':
                printf("\e[30;43m W \e[0m");
                break;
            }

            // print message
            printf(" \e[1;30m%s\e[0m", results[MESSAGE].str().c_str());
        } else {
            // line did not match our regular expression. just print it
            // normally.
            printf("\e[1;30m%s\e[0m", line);
        }
        stdout_lock.release();
    }
}


/******************************************************************************/

/**
 * Return the next tag color to use after the current one.
 */
static enum tag_color
next_tag_color(enum tag_color current) {
    if (current == TAG_CYAN) {
        return TAG_BRIGHT_RED;
    } else if (current == TAG_BRIGHT_CYAN) {
        return TAG_RED;
    } else if (current >= TAG_RED && current < TAG_BRIGHT_CYAN) {
        return (enum tag_color)(current + 1);
    } else {
        return TAG_RED;
    }
}
