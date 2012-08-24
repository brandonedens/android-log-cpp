/** @file device.h - interface to an android device. */

/** @file device.h @verbatim
 *========================================================================
 * Copyright (C) 2012 Brandon Edens - All Rights Reserved
 *========================================================================
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
#ifndef _DEVICE_H
#define _DEVICE_H


#include <stdio.h>
#if 0
#include <tbb/compat/thread>
#endif
#include <tbb/tbb_thread.h>
#include <map>
#include <string>


// A macro to disallow the copy constructor and operator= functions
// This should be used in the private: declarations for a class
#define DISALLOW_COPY_AND_ASSIGN(TypeName) \
    TypeName(const TypeName&);             \
    void operator=(const TypeName&)


/**
 * The index numbers for different portions of matched regular expression in
 * line of log.
 */
enum regex_match {
    TIME    = 1,
    TAGTYPE = 2,
    TAG     = 3,
    OWNER   = 4,
    MESSAGE = 5,
};

/**
 * A type that represents the various ANSI colors usable for displaying log tag
 * information. Please note that the ordering here is used to determine what
 * color will be used for the next unknown tag that is seen.
 */
enum tag_color {
    TAG_NULL           = 0,
    TAG_RED            = 31,
    TAG_GREEN          = 32,
    TAG_YELLOW         = 33,
    TAG_BLUE           = 34,
    TAG_MAGENTA        = 35,
    TAG_CYAN           = 36,
    TAG_BRIGHT_RED     = 91,
    TAG_BRIGHT_GREEN   = 92,
    TAG_BRIGHT_YELLOW  = 93,
    TAG_BRIGHT_BLUE    = 94,
    TAG_BRIGHT_MAGENTA = 95,
    TAG_BRIGHT_CYAN    = 96
};


class Device {
 public:
    explicit Device(const std::string serial);
    void init(void);
    void run(void);
    std::string serial_number(void) const {
        return serial_number_;
    }
    bool operator <(const Device& other) const {
        return serial_number_ < other.serial_number();
    }
    bool operator ==(const Device& other) const {
        return serial_number_ == other.serial_number();
    }

 private:
    FILE *file_;
    std::string serial_number_;

    DISALLOW_COPY_AND_ASSIGN(Device);
};


#endif /* _DEVICE_H */
