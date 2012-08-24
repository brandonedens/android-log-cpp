/** @file main.cc - entry point of the software. */

/** @file main.cc @verbatim
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
 * Date: 2012-08-20
 * Author: Brandon Edens <brandonedens@gmail.com>
 *
 * @endverbatim
 */

#include <stdio.h>
#include <boost/regex.hpp>
#include <boost/shared_ptr.hpp>
#include <tbb/tbb_thread.h>
#include <map>
#include <string>

#include "device.h"


using ::boost::match_results;
using ::boost::regex;
using ::boost::regex_match;
using ::boost::shared_ptr;


/** Maxmium number of characters in a read line. */
#define LINE_NCHARS (1024)


/** Wrapper function for launching the device's run method. */
void process_log(shared_ptr<Device> device) {
    device->run();
}


int main(int argc, char *argv[]) {
    // first gather listing of available devices
    regex device_regex;
    device_regex.assign("^([0-9A-Fa-f]+)[ \t]+device.*$", regex::egrep);

    char cmd[256];
    snprintf(cmd, sizeof(cmd), "adb devices");
    FILE *file = popen(cmd, "r");

    std::set<std::string> device_set;
    char line[LINE_NCHARS];
    while (fgets(line, sizeof(line), file) != NULL) {
        match_results<const char *> matches;
        if (regex_match(line, matches, device_regex)) {
            device_set.insert(matches[1].str());
        }
    }

    if (device_set.size() == 0) {
        printf("No devices present for logging.\n");
        exit(0);
    }

    // now create instances of Device for each device serial number
    std::set<shared_ptr<Device>> devices;
    for (std::set<std::string>::iterator it = device_set.begin();
         it != device_set.end();
         it++) {
        devices.insert(shared_ptr<Device>(new Device(*it)));
    }

    // spin up collection of threads that execute the adb logcat for the device.
    std::set<shared_ptr<tbb::tbb_thread>> threads;
    for (std::set<shared_ptr<Device>>::iterator it = devices.begin();
         it != devices.end();
         it++) {
        shared_ptr<Device> device = *it;
        printf("device: %s\n", device->serial_number().c_str());
        shared_ptr<tbb::tbb_thread>
            thread(new tbb::tbb_thread(process_log, device));
        threads.insert(thread);
    }

    // attempt to join threads
    for (std::set<shared_ptr<tbb::tbb_thread>>::iterator it = threads.begin();
         it != threads.end();
         it++) {
        shared_ptr<tbb::tbb_thread> thread = *it;
        (*thread).join();
    }

    // TODO(brandon) sleep and attempt to spawn new threads of execution for
    // devices as they are inserted.

    return 0;
}
