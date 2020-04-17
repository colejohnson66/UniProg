/* ============================================================================
 * File:   main.cpp
 * Author: Cole Johnson
 * ============================================================================
 * Copyright (c) 2020 Cole Johnson
 * 
 * This file is part of upro.
 * 
 * upro is free software: you can redistribute it and/or modify it under the
 *   terms of the GNU General Public License as published by the Free Software
 *   Foundation, either version 3 of the License, or (at your option) any later
 *   version.
 * 
 * upro is distributed in the hope that it will be useful, but WITHOUT ANY
 *   WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 *   FOR A PARTICULAR PURPOSE. See the GNU General Public License for more
 *   details.
 * 
 * You should have received a copy of the GNU General Public License along with
 *   upro. If not, see <http://www.gnu.org/licenses/>.
 * ============================================================================
 */
#include "upro.hpp"
//#include "getopt.h"
#include <cursesw.h>

#include <iostream>
#include <signal.h>
#include <string>

enum class chip_option
{
    none,
    read,
    write,
    verify,
    blank_check,
    erase
};

auto resizeHandler(int sig) -> void;

auto interactive() -> void;
void print_license();
void print_version_info();
void print_help();
void list_devices();
void list_devices(std::string search);
void list_device_info(std::string device);
void check_hardware();
void update_firmware(std::string filepath);

int main(int argc, char **argv)
{
    interactive();
    //print_license();

    /*
    bool should_quit = false;
    chip_option chip_option_value = chip_option::none;
    std::string chip_file{""};
    std::string file_format{""};
    std::string device{""};
    bool verbose = false;
    int opt;
    while ((opt = getopt(argc, argv, "lL:d:r:w:m:f:bep:vVtu:h")) != -1)
    {
        switch (opt)
        {
        case 'l':
            list_devices();
            should_quit = true;
            break;

        case 'L':
            list_devices(std::string{optarg});
            should_quit = true;
            break;

        case 'd':
            list_device_info(std::string{optarg});
            should_quit = true;
            break;

        case 'r':
            if (chip_option_value != chip_option::none)
            {
                std::cerr << "Multiple chip operations selected." << std::endl;
                should_quit = true;
                break;
            }
            chip_option_value = chip_option::read;
            chip_file = optarg;
            break;

        case 'w':
            if (chip_option_value != chip_option::none)
            {
                std::cerr << "Multiple chip operations selected." << std::endl;
                should_quit = true;
                break;
            }
            chip_option_value = chip_option::write;
            chip_file = optarg;
            break;

        case 'm':
            if (chip_option_value != chip_option::none)
            {
                std::cerr << "Multiple chip operations selected." << std::endl;
                should_quit = true;
                break;
            }
            chip_option_value = chip_option::verify;
            chip_file = optarg;
            break;

        case 'f':
            file_format = optarg;
            should_quit = true;
            break;

        case 'b':
            if (chip_option_value != chip_option::none)
            {
                std::cerr << "Multiple chip operations selected." << std::endl;
                should_quit = true;
                break;
            }
            chip_option_value = chip_option::blank_check;
            break;

        case 'p':
            device = optarg;
            break;

        case 'v':
            verbose = true;
            break;

        case 'V':
            print_version_info();
            should_quit = true;
            break;

        case 't':
            check_hardware();
            should_quit = true;
            break;

        case 'u':
            update_firmware(optarg);
            should_quit = true;
            break;

        case 'h':
            print_help();
            should_quit = true;
            break;
        }

        if (should_quit)
        {
            std::cout << std::endl;
            return 0;
        }
    }*/

    std::cout << std::endl;
    return 0;
}

auto interactive() -> void
{
    signal(SIGWINCH, resizeHandler);
    initscr();
    cbreak();
    noecho();
    keypad(stdscr, true);

    border(0, 0, 0, 0, 0, 0, 0, 0);

    int ch;
    nodelay(stdscr, true);
    while (true)
    {
        if ((ch = getch()) == ERR)
            continue;
        if (ch == KEY_DOWN)
            break;

        addch(ch);
        refresh();
    }
    endwin();
}

auto resizeHandler(int sig) -> void
{
    int new_h, new_w;
    getmaxyx(stdscr, new_h, new_w);

    addstr()
}

void print_license()
{
    std::cout << "upro version " UPRO_VERSION_STR "\n";
    std::cout << "Copyright (c) 2020 Cole Johnson\n";
    std::cout << "Licensed under the GNU GPL v3 or later\n";
    std::cout << std::endl;
}

void print_version_info()
{
    std::cout << "upro version " UPRO_VERSION_STR "\n";
    std::cout << std::endl;
}

void print_help()
{
    std::cout << "Usage: upro [options]\n";
    std::cout << "options:\n";
    std::cout << "  -l          List all supported devices\n";
    std::cout << "  -L <device> Search for devices similar to <device>\n";
    std::cout << "  -d <device> Show device information for <device>\n";
    std::cout << "  -r <file>   Read chip contents into <file>\n";
    std::cout << "  -w <file>   Write chip contents from <file>\n";
    std::cout << "  -m <file>   Verify chip contents from <file>\n";
    std::cout << "  -f <format> Specify file format\n";
    std::cout << "          Possible values: srec, ihex, raw\n";
    std::cout << "  -b          Blank check\n";
    std::cout << "  -e          Erase device\n";
    std::cout << "  -p <device> Specify device\n";
    std::cout << "  -v          Verbose output\n";
    std::cout << "  -V          Prints version information\n";
    std::cout << "  -t          Check UniProg hardware\n";
    std::cout << "  -u <file>   Update firmware\n";
    std::cout << "  -h          Display help\n";
}

void list_devices()
{
    list_devices("");
}
void list_devices(std::string search)
{
}

void list_device_info(std::string device)
{
}

void check_hardware()
{
}

void update_firmware(std::string filepath)
{
}