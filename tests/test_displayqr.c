/**
 * @file
 * @author  cd611@cam.ac.uk
 * @version 1.0
 *
 * @section LICENSE
 *
 * Copyright Pico project, 2016
 *
 * @section DESCRIPTION
 *
 */

#include <check.h>
#include "pico/buffer.h"
#include "pico/displayqr.h"

// Defines

// Structure definitions

// Function prototypes

// Function definitions
char* input[3] = {
	"a",
	"TEST",
	"í"
};

char * expected_color_utf8[3] = {
	"\x1b[30;47;27m                         \x1b[0m\n"
	"\x1b[30;47;27m  █▀▀▀▀▀█ ▀ █▄▀ █▀▀▀▀▀█  \x1b[0m\n"
	"\x1b[30;47;27m  █ ███ █ ▄██ █ █ ███ █  \x1b[0m\n"
	"\x1b[30;47;27m  █ ▀▀▀ █ █ ▄▄▀ █ ▀▀▀ █  \x1b[0m\n"
	"\x1b[30;47;27m  ▀▀▀▀▀▀▀ █ █▄█ ▀▀▀▀▀▀▀  \x1b[0m\n"
	"\x1b[30;47;27m  █▄ ▄▀ ▀█▀▀██▄▀▀█▀█ ▄█  \x1b[0m\n"
	"\x1b[30;47;27m  █▄▀ ▄ ▀▄▄█▄▀ ▄█▀█▀█▄   \x1b[0m\n"
	"\x1b[30;47;27m  ▀▀  ▀▀▀▀█▄▀ ███   ██▀  \x1b[0m\n"
	"\x1b[30;47;27m  █▀▀▀▀▀█ ▀█▄▄█▀ ▄ ▄ █   \x1b[0m\n"
	"\x1b[30;47;27m  █ ███ █ ▀▄▀█▄ ▀█▀█▀██  \x1b[0m\n"
	"\x1b[30;47;27m  █ ▀▀▀ █  █ ▀ ▄█▀█▀█    \x1b[0m\n"
	"\x1b[30;47;27m  ▀▀▀▀▀▀▀ ▀▀▀ ▀▀▀   ▀ ▀  \x1b[0m\n"
	"\x1b[30;47;27m                         \x1b[0m\n",
	"\x1b[30;47;27m                         \x1b[0m\n"
	"\x1b[30;47;27m  █▀▀▀▀▀█ ▄██▀▀ █▀▀▀▀▀█  \x1b[0m\n"
	"\x1b[30;47;27m  █ ███ █  ▄▀█  █ ███ █  \x1b[0m\n"
	"\x1b[30;47;27m  █ ▀▀▀ █ ▀ ██▀ █ ▀▀▀ █  \x1b[0m\n"
	"\x1b[30;47;27m  ▀▀▀▀▀▀▀ ▀ █ ▀ ▀▀▀▀▀▀▀  \x1b[0m\n"
	"\x1b[30;47;27m  ▀▄█▄█▄▀▄▄  ▄▀▄ ▄▀  █   \x1b[0m\n"
	"\x1b[30;47;27m  █▄█▄ ▄▀▀▀▀██▄█▀█▄▀▀█▀  \x1b[0m\n"
	"\x1b[30;47;27m  ▀▀ ▀  ▀▀█▄ ▀ ▀█▀ █▀▀▀  \x1b[0m\n"
	"\x1b[30;47;27m  █▀▀▀▀▀█  █▀ ▀ ▄ ▀▀ █▀  \x1b[0m\n"
	"\x1b[30;47;27m  █ ███ █ ▀ ▀▄▀▄▀▄▀▀▀█▀  \x1b[0m\n"
	"\x1b[30;47;27m  █ ▀▀▀ █ ▀▄▄█▄█▀█▄▄▀▄▀  \x1b[0m\n"
	"\x1b[30;47;27m  ▀▀▀▀▀▀▀ ▀▀▀▀ ▀▀▀  ▀▀▀  \x1b[0m\n"
	"\x1b[30;47;27m                         \x1b[0m\n",
	"\x1b[30;47;27m                         \x1b[0m\n"
	"\x1b[30;47;27m  █▀▀▀▀▀█  █▀▄▀ █▀▀▀▀▀█  \x1b[0m\n"
	"\x1b[30;47;27m  █ ███ █ ██▀▀▀ █ ███ █  \x1b[0m\n"
	"\x1b[30;47;27m  █ ▀▀▀ █ █  ▀█ █ ▀▀▀ █  \x1b[0m\n"
	"\x1b[30;47;27m  ▀▀▀▀▀▀▀ █▄█▄█ ▀▀▀▀▀▀▀  \x1b[0m\n"
	"\x1b[30;47;27m  █ ██▀▀▀▄ ▀▄ █ ▀█▀█▀ ▄  \x1b[0m\n"
	"\x1b[30;47;27m  ▄ ▀▄ ▀▀ ▀▄▄▀ ▀ ▄█▀█▄█  \x1b[0m\n"
	"\x1b[30;47;27m   ▀  ▀▀▀▀▄▀▀█▄█▄ ▀▄▀    \x1b[0m\n"
	"\x1b[30;47;27m  █▀▀▀▀▀█ ▄██▄█▄█▀ ▄ ▀█  \x1b[0m\n"
	"\x1b[30;47;27m  █ ███ █ █▄▄ █  █  █    \x1b[0m\n"
	"\x1b[30;47;27m  █ ▀▀▀ █ ▀▀▄▀ ▀ ▄█▀█    \x1b[0m\n"
	"\x1b[30;47;27m  ▀▀▀▀▀▀▀ ▀  ▀ ▀  ▀▀▀▀   \x1b[0m\n"
	"\x1b[30;47;27m                         \x1b[0m\n",
};

char * expected_colorless_utf8[3] = {
	"█▀▀▀▀▀█ ▀ █▄▀ █▀▀▀▀▀█\n"
	"█ ███ █ ▄██ █ █ ███ █\n"
	"█ ▀▀▀ █ █ ▄▄▀ █ ▀▀▀ █\n"
	"▀▀▀▀▀▀▀ █ █▄█ ▀▀▀▀▀▀▀\n"
	"█▄ ▄▀ ▀█▀▀██▄▀▀█▀█ ▄█\n"
	"█▄▀ ▄ ▀▄▄█▄▀ ▄█▀█▀█▄ \n"
	"▀▀  ▀▀▀▀█▄▀ ███   ██▀\n"
	"█▀▀▀▀▀█ ▀█▄▄█▀ ▄ ▄ █ \n"
	"█ ███ █ ▀▄▀█▄ ▀█▀█▀██\n"
	"█ ▀▀▀ █  █ ▀ ▄█▀█▀█  \n"
	"▀▀▀▀▀▀▀ ▀▀▀ ▀▀▀   ▀ ▀\n",
	"█▀▀▀▀▀█ ▄██▀▀ █▀▀▀▀▀█\n"
	"█ ███ █  ▄▀█  █ ███ █\n"
	"█ ▀▀▀ █ ▀ ██▀ █ ▀▀▀ █\n"
	"▀▀▀▀▀▀▀ ▀ █ ▀ ▀▀▀▀▀▀▀\n"
	"▀▄█▄█▄▀▄▄  ▄▀▄ ▄▀  █ \n"
	"█▄█▄ ▄▀▀▀▀██▄█▀█▄▀▀█▀\n"
	"▀▀ ▀  ▀▀█▄ ▀ ▀█▀ █▀▀▀\n"
	"█▀▀▀▀▀█  █▀ ▀ ▄ ▀▀ █▀\n"
	"█ ███ █ ▀ ▀▄▀▄▀▄▀▀▀█▀\n"
	"█ ▀▀▀ █ ▀▄▄█▄█▀█▄▄▀▄▀\n"
	"▀▀▀▀▀▀▀ ▀▀▀▀ ▀▀▀  ▀▀▀\n",
	"█▀▀▀▀▀█  █▀▄▀ █▀▀▀▀▀█\n"
	"█ ███ █ ██▀▀▀ █ ███ █\n"
	"█ ▀▀▀ █ █  ▀█ █ ▀▀▀ █\n"
	"▀▀▀▀▀▀▀ █▄█▄█ ▀▀▀▀▀▀▀\n"
	"█ ██▀▀▀▄ ▀▄ █ ▀█▀█▀ ▄\n"
	"▄ ▀▄ ▀▀ ▀▄▄▀ ▀ ▄█▀█▄█\n"
	" ▀  ▀▀▀▀▄▀▀█▄█▄ ▀▄▀  \n"
	"█▀▀▀▀▀█ ▄██▄█▄█▀ ▄ ▀█\n"
	"█ ███ █ █▄▄ █  █  █  \n"
	"█ ▀▀▀ █ ▀▀▄▀ ▀ ▄█▀█  \n"
	"▀▀▀▀▀▀▀ ▀  ▀ ▀  ▀▀▀▀ \n"
};

char * expected_ansi[3] = {
	"\x1b[30;47;27m                                                  \x1b[0m\n"
	"\x1b[30;47;27m                                                  \x1b[0m\n"
	"\x1b[30;47;27m    \x1b[7m              \x1b[27m  \x1b[7m  \x1b[27m  \x1b[7m  \x1b[27m  \x1b[7m  \x1b[27m  \x1b[7m              \x1b[27m    \x1b[0m\n"
	"\x1b[30;47;27m    \x1b[7m  \x1b[27m          \x1b[7m  \x1b[27m      \x1b[7m    \x1b[27m    \x1b[7m  \x1b[27m          \x1b[7m  \x1b[27m    \x1b[0m\n"
	"\x1b[30;47;27m    \x1b[7m  \x1b[27m  \x1b[7m      \x1b[27m  \x1b[7m  \x1b[27m    \x1b[7m    \x1b[27m  \x1b[7m  \x1b[27m  \x1b[7m  \x1b[27m  \x1b[7m      \x1b[27m  \x1b[7m  \x1b[27m    \x1b[0m\n"
	"\x1b[30;47;27m    \x1b[7m  \x1b[27m  \x1b[7m      \x1b[27m  \x1b[7m  \x1b[27m  \x1b[7m      \x1b[27m  \x1b[7m  \x1b[27m  \x1b[7m  \x1b[27m  \x1b[7m      \x1b[27m  \x1b[7m  \x1b[27m    \x1b[0m\n"
	"\x1b[30;47;27m    \x1b[7m  \x1b[27m  \x1b[7m      \x1b[27m  \x1b[7m  \x1b[27m  \x1b[7m  \x1b[27m      \x1b[7m  \x1b[27m  \x1b[7m  \x1b[27m  \x1b[7m      \x1b[27m  \x1b[7m  \x1b[27m    \x1b[0m\n"
	"\x1b[30;47;27m    \x1b[7m  \x1b[27m          \x1b[7m  \x1b[27m  \x1b[7m  \x1b[27m  \x1b[7m    \x1b[27m    \x1b[7m  \x1b[27m          \x1b[7m  \x1b[27m    \x1b[0m\n"
	"\x1b[30;47;27m    \x1b[7m              \x1b[27m  \x1b[7m  \x1b[27m  \x1b[7m  \x1b[27m  \x1b[7m  \x1b[27m  \x1b[7m              \x1b[27m    \x1b[0m\n"
	"\x1b[30;47;27m                    \x1b[7m  \x1b[27m  \x1b[7m      \x1b[27m                    \x1b[0m\n"
	"\x1b[30;47;27m    \x1b[7m  \x1b[27m      \x1b[7m  \x1b[27m  \x1b[7m            \x1b[27m  \x1b[7m          \x1b[27m    \x1b[7m  \x1b[27m    \x1b[0m\n"
	"\x1b[30;47;27m    \x1b[7m    \x1b[27m  \x1b[7m  \x1b[27m      \x1b[7m  \x1b[27m    \x1b[7m      \x1b[27m    \x1b[7m  \x1b[27m  \x1b[7m  \x1b[27m  \x1b[7m    \x1b[27m    \x1b[0m\n"
	"\x1b[30;47;27m    \x1b[7m  \x1b[27m  \x1b[7m  \x1b[27m      \x1b[7m  \x1b[27m    \x1b[7m  \x1b[27m  \x1b[7m  \x1b[27m    \x1b[7m          \x1b[27m        \x1b[0m\n"
	"\x1b[30;47;27m    \x1b[7m    \x1b[27m    \x1b[7m  \x1b[27m    \x1b[7m        \x1b[27m    \x1b[7m    \x1b[27m  \x1b[7m  \x1b[27m  \x1b[7m    \x1b[27m      \x1b[0m\n"
	"\x1b[30;47;27m    \x1b[7m    \x1b[27m    \x1b[7m          \x1b[27m  \x1b[7m  \x1b[27m  \x1b[7m      \x1b[27m      \x1b[7m      \x1b[27m    \x1b[0m\n"
	"\x1b[30;47;27m                    \x1b[7m    \x1b[27m    \x1b[7m      \x1b[27m      \x1b[7m    \x1b[27m      \x1b[0m\n"
	"\x1b[30;47;27m    \x1b[7m              \x1b[27m  \x1b[7m    \x1b[27m    \x1b[7m    \x1b[27m          \x1b[7m  \x1b[27m      \x1b[0m\n"
	"\x1b[30;47;27m    \x1b[7m  \x1b[27m          \x1b[7m  \x1b[27m    \x1b[7m        \x1b[27m    \x1b[7m  \x1b[27m  \x1b[7m  \x1b[27m  \x1b[7m  \x1b[27m      \x1b[0m\n"
	"\x1b[30;47;27m    \x1b[7m  \x1b[27m  \x1b[7m      \x1b[27m  \x1b[7m  \x1b[27m  \x1b[7m  \x1b[27m  \x1b[7m    \x1b[27m    \x1b[7m              \x1b[27m    \x1b[0m\n"
	"\x1b[30;47;27m    \x1b[7m  \x1b[27m  \x1b[7m      \x1b[27m  \x1b[7m  \x1b[27m    \x1b[7m  \x1b[27m  \x1b[7m    \x1b[27m    \x1b[7m  \x1b[27m  \x1b[7m  \x1b[27m  \x1b[7m    \x1b[27m    \x1b[0m\n"
	"\x1b[30;47;27m    \x1b[7m  \x1b[27m  \x1b[7m      \x1b[27m  \x1b[7m  \x1b[27m    \x1b[7m  \x1b[27m  \x1b[7m  \x1b[27m    \x1b[7m          \x1b[27m        \x1b[0m\n"
	"\x1b[30;47;27m    \x1b[7m  \x1b[27m          \x1b[7m  \x1b[27m    \x1b[7m  \x1b[27m      \x1b[7m    \x1b[27m  \x1b[7m  \x1b[27m  \x1b[7m  \x1b[27m        \x1b[0m\n"
	"\x1b[30;47;27m    \x1b[7m              \x1b[27m  \x1b[7m      \x1b[27m  \x1b[7m      \x1b[27m      \x1b[7m  \x1b[27m  \x1b[7m  \x1b[27m    \x1b[0m\n"
	"\x1b[30;47;27m                                                  \x1b[0m\n"
	"\x1b[30;47;27m                                                  \x1b[0m\n",
	"\x1b[30;47;27m                                                  \x1b[0m\n"
	"\x1b[30;47;27m                                                  \x1b[0m\n"
	"\x1b[30;47;27m    \x1b[7m              \x1b[27m    \x1b[7m        \x1b[27m  \x1b[7m              \x1b[27m    \x1b[0m\n"
	"\x1b[30;47;27m    \x1b[7m  \x1b[27m          \x1b[7m  \x1b[27m  \x1b[7m      \x1b[27m      \x1b[7m  \x1b[27m          \x1b[7m  \x1b[27m    \x1b[0m\n"
	"\x1b[30;47;27m    \x1b[7m  \x1b[27m  \x1b[7m      \x1b[27m  \x1b[7m  \x1b[27m      \x1b[7m    \x1b[27m    \x1b[7m  \x1b[27m  \x1b[7m      \x1b[27m  \x1b[7m  \x1b[27m    \x1b[0m\n"
	"\x1b[30;47;27m    \x1b[7m  \x1b[27m  \x1b[7m      \x1b[27m  \x1b[7m  \x1b[27m    \x1b[7m  \x1b[27m  \x1b[7m  \x1b[27m    \x1b[7m  \x1b[27m  \x1b[7m      \x1b[27m  \x1b[7m  \x1b[27m    \x1b[0m\n"
	"\x1b[30;47;27m    \x1b[7m  \x1b[27m  \x1b[7m      \x1b[27m  \x1b[7m  \x1b[27m  \x1b[7m  \x1b[27m  \x1b[7m      \x1b[27m  \x1b[7m  \x1b[27m  \x1b[7m      \x1b[27m  \x1b[7m  \x1b[27m    \x1b[0m\n"
	"\x1b[30;47;27m    \x1b[7m  \x1b[27m          \x1b[7m  \x1b[27m      \x1b[7m    \x1b[27m    \x1b[7m  \x1b[27m          \x1b[7m  \x1b[27m    \x1b[0m\n"
	"\x1b[30;47;27m    \x1b[7m              \x1b[27m  \x1b[7m  \x1b[27m  \x1b[7m  \x1b[27m  \x1b[7m  \x1b[27m  \x1b[7m              \x1b[27m    \x1b[0m\n"
	"\x1b[30;47;27m                        \x1b[7m  \x1b[27m                        \x1b[0m\n"
	"\x1b[30;47;27m    \x1b[7m  \x1b[27m  \x1b[7m  \x1b[27m  \x1b[7m  \x1b[27m  \x1b[7m  \x1b[27m          \x1b[7m  \x1b[27m      \x1b[7m  \x1b[27m    \x1b[7m  \x1b[27m      \x1b[0m\n"
	"\x1b[30;47;27m      \x1b[7m          \x1b[27m  \x1b[7m    \x1b[27m    \x1b[7m  \x1b[27m  \x1b[7m  \x1b[27m  \x1b[7m  \x1b[27m      \x1b[7m  \x1b[27m      \x1b[0m\n"
	"\x1b[30;47;27m    \x1b[7m  \x1b[27m  \x1b[7m  \x1b[27m      \x1b[7m            \x1b[27m  \x1b[7m      \x1b[27m  \x1b[7m        \x1b[27m    \x1b[0m\n"
	"\x1b[30;47;27m    \x1b[7m        \x1b[27m  \x1b[7m  \x1b[27m        \x1b[7m        \x1b[27m  \x1b[7m    \x1b[27m    \x1b[7m  \x1b[27m      \x1b[0m\n"
	"\x1b[30;47;27m    \x1b[7m    \x1b[27m  \x1b[7m  \x1b[27m    \x1b[7m      \x1b[27m    \x1b[7m  \x1b[27m  \x1b[7m      \x1b[27m  \x1b[7m        \x1b[27m    \x1b[0m\n"
	"\x1b[30;47;27m                    \x1b[7m    \x1b[27m        \x1b[7m  \x1b[27m    \x1b[7m  \x1b[27m          \x1b[0m\n"
	"\x1b[30;47;27m    \x1b[7m              \x1b[27m    \x1b[7m    \x1b[27m  \x1b[7m  \x1b[27m      \x1b[7m    \x1b[27m  \x1b[7m    \x1b[27m    \x1b[0m\n"
	"\x1b[30;47;27m    \x1b[7m  \x1b[27m          \x1b[7m  \x1b[27m    \x1b[7m  \x1b[27m        \x1b[7m  \x1b[27m        \x1b[7m  \x1b[27m      \x1b[0m\n"
	"\x1b[30;47;27m    \x1b[7m  \x1b[27m  \x1b[7m      \x1b[27m  \x1b[7m  \x1b[27m  \x1b[7m  \x1b[27m  \x1b[7m  \x1b[27m  \x1b[7m  \x1b[27m  \x1b[7m  \x1b[27m  \x1b[7m          \x1b[27m    \x1b[0m\n"
	"\x1b[30;47;27m    \x1b[7m  \x1b[27m  \x1b[7m      \x1b[27m  \x1b[7m  \x1b[27m        \x1b[7m  \x1b[27m  \x1b[7m  \x1b[27m  \x1b[7m  \x1b[27m      \x1b[7m  \x1b[27m      \x1b[0m\n"
	"\x1b[30;47;27m    \x1b[7m  \x1b[27m  \x1b[7m      \x1b[27m  \x1b[7m  \x1b[27m  \x1b[7m  \x1b[27m    \x1b[7m  \x1b[27m  \x1b[7m      \x1b[27m    \x1b[7m  \x1b[27m  \x1b[7m  \x1b[27m    \x1b[0m\n"
	"\x1b[30;47;27m    \x1b[7m  \x1b[27m          \x1b[7m  \x1b[27m    \x1b[7m          \x1b[27m  \x1b[7m      \x1b[27m  \x1b[7m  \x1b[27m      \x1b[0m\n"
	"\x1b[30;47;27m    \x1b[7m              \x1b[27m  \x1b[7m        \x1b[27m  \x1b[7m      \x1b[27m    \x1b[7m      \x1b[27m    \x1b[0m\n"
	"\x1b[30;47;27m                                                  \x1b[0m\n"
	"\x1b[30;47;27m                                                  \x1b[0m\n",
	"\x1b[30;47;27m                                                  \x1b[0m\n"
	"\x1b[30;47;27m                                                  \x1b[0m\n"
	"\x1b[30;47;27m    \x1b[7m              \x1b[27m    \x1b[7m    \x1b[27m  \x1b[7m  \x1b[27m  \x1b[7m              \x1b[27m    \x1b[0m\n"
	"\x1b[30;47;27m    \x1b[7m  \x1b[27m          \x1b[7m  \x1b[27m    \x1b[7m  \x1b[27m  \x1b[7m  \x1b[27m    \x1b[7m  \x1b[27m          \x1b[7m  \x1b[27m    \x1b[0m\n"
	"\x1b[30;47;27m    \x1b[7m  \x1b[27m  \x1b[7m      \x1b[27m  \x1b[7m  \x1b[27m  \x1b[7m          \x1b[27m  \x1b[7m  \x1b[27m  \x1b[7m      \x1b[27m  \x1b[7m  \x1b[27m    \x1b[0m\n"
	"\x1b[30;47;27m    \x1b[7m  \x1b[27m  \x1b[7m      \x1b[27m  \x1b[7m  \x1b[27m  \x1b[7m    \x1b[27m        \x1b[7m  \x1b[27m  \x1b[7m      \x1b[27m  \x1b[7m  \x1b[27m    \x1b[0m\n"
	"\x1b[30;47;27m    \x1b[7m  \x1b[27m  \x1b[7m      \x1b[27m  \x1b[7m  \x1b[27m  \x1b[7m  \x1b[27m    \x1b[7m    \x1b[27m  \x1b[7m  \x1b[27m  \x1b[7m      \x1b[27m  \x1b[7m  \x1b[27m    \x1b[0m\n"
	"\x1b[30;47;27m    \x1b[7m  \x1b[27m          \x1b[7m  \x1b[27m  \x1b[7m  \x1b[27m      \x1b[7m  \x1b[27m  \x1b[7m  \x1b[27m          \x1b[7m  \x1b[27m    \x1b[0m\n"
	"\x1b[30;47;27m    \x1b[7m              \x1b[27m  \x1b[7m  \x1b[27m  \x1b[7m  \x1b[27m  \x1b[7m  \x1b[27m  \x1b[7m              \x1b[27m    \x1b[0m\n"
	"\x1b[30;47;27m                    \x1b[7m          \x1b[27m                    \x1b[0m\n"
	"\x1b[30;47;27m    \x1b[7m  \x1b[27m  \x1b[7m          \x1b[27m    \x1b[7m  \x1b[27m    \x1b[7m  \x1b[27m  \x1b[7m          \x1b[27m        \x1b[0m\n"
	"\x1b[30;47;27m    \x1b[7m  \x1b[27m  \x1b[7m    \x1b[27m      \x1b[7m  \x1b[27m    \x1b[7m  \x1b[27m  \x1b[7m  \x1b[27m    \x1b[7m  \x1b[27m  \x1b[7m  \x1b[27m    \x1b[7m  \x1b[27m    \x1b[0m\n"
	"\x1b[30;47;27m        \x1b[7m  \x1b[27m    \x1b[7m    \x1b[27m  \x1b[7m  \x1b[27m    \x1b[7m  \x1b[27m  \x1b[7m  \x1b[27m    \x1b[7m      \x1b[27m  \x1b[7m  \x1b[27m    \x1b[0m\n"
	"\x1b[30;47;27m    \x1b[7m  \x1b[27m    \x1b[7m  \x1b[27m          \x1b[7m    \x1b[27m        \x1b[7m    \x1b[27m  \x1b[7m      \x1b[27m    \x1b[0m\n"
	"\x1b[30;47;27m      \x1b[7m  \x1b[27m    \x1b[7m        \x1b[27m  \x1b[7m      \x1b[27m  \x1b[7m  \x1b[27m    \x1b[7m  \x1b[27m  \x1b[7m  \x1b[27m        \x1b[0m\n"
	"\x1b[30;47;27m                    \x1b[7m  \x1b[27m    \x1b[7m        \x1b[27m    \x1b[7m  \x1b[27m          \x1b[0m\n"
	"\x1b[30;47;27m    \x1b[7m              \x1b[27m    \x1b[7m    \x1b[27m  \x1b[7m  \x1b[27m  \x1b[7m    \x1b[27m      \x1b[7m    \x1b[27m    \x1b[0m\n"
	"\x1b[30;47;27m    \x1b[7m  \x1b[27m          \x1b[7m  \x1b[27m  \x1b[7m              \x1b[27m    \x1b[7m  \x1b[27m    \x1b[7m  \x1b[27m    \x1b[0m\n"
	"\x1b[30;47;27m    \x1b[7m  \x1b[27m  \x1b[7m      \x1b[27m  \x1b[7m  \x1b[27m  \x1b[7m  \x1b[27m      \x1b[7m  \x1b[27m    \x1b[7m  \x1b[27m    \x1b[7m  \x1b[27m        \x1b[0m\n"
	"\x1b[30;47;27m    \x1b[7m  \x1b[27m  \x1b[7m      \x1b[27m  \x1b[7m  \x1b[27m  \x1b[7m      \x1b[27m  \x1b[7m  \x1b[27m    \x1b[7m  \x1b[27m    \x1b[7m  \x1b[27m        \x1b[0m\n"
	"\x1b[30;47;27m    \x1b[7m  \x1b[27m  \x1b[7m      \x1b[27m  \x1b[7m  \x1b[27m  \x1b[7m    \x1b[27m  \x1b[7m  \x1b[27m  \x1b[7m  \x1b[27m    \x1b[7m      \x1b[27m        \x1b[0m\n"
	"\x1b[30;47;27m    \x1b[7m  \x1b[27m          \x1b[7m  \x1b[27m      \x1b[7m  \x1b[27m        \x1b[7m    \x1b[27m  \x1b[7m  \x1b[27m        \x1b[0m\n"
	"\x1b[30;47;27m    \x1b[7m              \x1b[27m  \x1b[7m  \x1b[27m    \x1b[7m  \x1b[27m  \x1b[7m  \x1b[27m    \x1b[7m        \x1b[27m      \x1b[0m\n"
	"\x1b[30;47;27m                                                  \x1b[0m\n"
	"\x1b[30;47;27m                                                  \x1b[0m\n"
};

char * expected_dos[3] = {
	"\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB\n"
	"\xDB\xDB \xDC\xDC\xDC\xDC\xDC \xDB\xDC\xDB \xDF\xDC\xDB \xDC\xDC\xDC\xDC\xDC \xDB\xDB\n"
	"\xDB\xDB \xDB   \xDB \xDB\xDF  \xDB \xDB \xDB   \xDB \xDB\xDB\n"
	"\xDB\xDB \xDB\xDC\xDC\xDC\xDB \xDB \xDB\xDF\xDF\xDC\xDB \xDB\xDC\xDC\xDC\xDB \xDB\xDB\n"
	"\xDB\xDB\xDC\xDC\xDC\xDC\xDC\xDC\xDC\xDB \xDB \xDF \xDB\xDC\xDC\xDC\xDC\xDC\xDC\xDC\xDB\xDB\n"
	"\xDB\xDB \xDF\xDB\xDF\xDC\xDB\xDC \xDC\xDC  \xDF\xDC\xDC \xDC \xDB\xDF \xDB\xDB\n"
	"\xDB\xDB \xDF\xDC\xDB\xDF\xDB\xDC\xDF\xDF \xDF\xDC\xDB\xDF \xDC \xDC \xDF\xDB\xDB\xDB\n"
	"\xDB\xDB\xDC\xDC\xDB\xDB\xDC\xDC\xDC\xDC \xDF\xDC\xDB   \xDB\xDB\xDB  \xDC\xDB\xDB\n"
	"\xDB\xDB \xDC\xDC\xDC\xDC\xDC \xDB\xDC \xDF\xDF \xDC\xDB\xDF\xDB\xDF\xDB \xDB\xDB\xDB\n"
	"\xDB\xDB \xDB   \xDB \xDB\xDC\xDF\xDC \xDF\xDB\xDC \xDC \xDC  \xDB\xDB\n"
	"\xDB\xDB \xDB\xDC\xDC\xDC\xDB \xDB\xDB \xDB\xDC\xDB\xDF \xDC \xDC \xDB\xDB\xDB\xDB\n"
	"\xDB\xDB\xDC\xDC\xDC\xDC\xDC\xDC\xDC\xDB\xDC\xDC\xDC\xDB\xDC\xDC\xDC\xDB\xDB\xDB\xDC\xDB\xDC\xDB\xDB\n"
	"\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB\n",
	"\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB\n"
	"\xDB\xDB \xDC\xDC\xDC\xDC\xDC \xDB\xDF  \xDC\xDC\xDB \xDC\xDC\xDC\xDC\xDC \xDB\xDB\n"
	"\xDB\xDB \xDB   \xDB \xDB\xDB\xDF\xDC \xDB\xDB \xDB   \xDB \xDB\xDB\n"
	"\xDB\xDB \xDB\xDC\xDC\xDC\xDB \xDB\xDC\xDB  \xDC\xDB \xDB\xDC\xDC\xDC\xDB \xDB\xDB\n"
	"\xDB\xDB\xDC\xDC\xDC\xDC\xDC\xDC\xDC\xDB\xDC\xDB \xDB\xDC\xDB\xDC\xDC\xDC\xDC\xDC\xDC\xDC\xDB\xDB\n"
	"\xDB\xDB\xDC\xDF \xDF \xDF\xDC\xDF\xDF\xDB\xDB\xDF\xDC\xDF\xDB\xDF\xDC\xDB\xDB \xDB\xDB\xDB\n"
	"\xDB\xDB \xDF \xDF\xDB\xDF\xDC\xDC\xDC\xDC  \xDF \xDC \xDF\xDC\xDC \xDC\xDB\xDB\n"
	"\xDB\xDB\xDC\xDC\xDB\xDC\xDB\xDB\xDC\xDC \xDF\xDB\xDC\xDB\xDC \xDC\xDB \xDC\xDC\xDC\xDB\xDB\n"
	"\xDB\xDB \xDC\xDC\xDC\xDC\xDC \xDB\xDB \xDC\xDB\xDC\xDB\xDF\xDB\xDC\xDC\xDB \xDC\xDB\xDB\n"
	"\xDB\xDB \xDB   \xDB \xDB\xDC\xDB\xDC\xDF\xDC\xDF\xDC\xDF\xDC\xDC\xDC \xDC\xDB\xDB\n"
	"\xDB\xDB \xDB\xDC\xDC\xDC\xDB \xDB\xDC\xDF\xDF \xDF \xDC \xDF\xDF\xDC\xDF\xDC\xDB\xDB\n"
	"\xDB\xDB\xDC\xDC\xDC\xDC\xDC\xDC\xDC\xDB\xDC\xDC\xDC\xDC\xDB\xDC\xDC\xDC\xDB\xDB\xDC\xDC\xDC\xDB\xDB\n"
	"\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB\n",
	"\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB\n"
	"\xDB\xDB \xDC\xDC\xDC\xDC\xDC \xDB\xDB \xDC\xDF\xDC\xDB \xDC\xDC\xDC\xDC\xDC \xDB\xDB\n"
	"\xDB\xDB \xDB   \xDB \xDB  \xDC\xDC\xDC\xDB \xDB   \xDB \xDB\xDB\n"
	"\xDB\xDB \xDB\xDC\xDC\xDC\xDB \xDB \xDB\xDB\xDC \xDB \xDB\xDC\xDC\xDC\xDB \xDB\xDB\n"
	"\xDB\xDB\xDC\xDC\xDC\xDC\xDC\xDC\xDC\xDB \xDF \xDF \xDB\xDC\xDC\xDC\xDC\xDC\xDC\xDC\xDB\xDB\n"
	"\xDB\xDB \xDB  \xDC\xDC\xDC\xDF\xDB\xDC\xDF\xDB \xDB\xDC \xDC \xDC\xDB\xDF\xDB\xDB\n"
	"\xDB\xDB\xDF\xDB\xDC\xDF\xDB\xDC\xDC\xDB\xDC\xDF\xDF\xDC\xDB\xDC\xDB\xDF \xDC \xDF \xDB\xDB\n"
	"\xDB\xDB\xDB\xDC\xDB\xDB\xDC\xDC\xDC\xDC\xDF\xDC\xDC \xDF \xDF\xDB\xDC\xDF\xDC\xDB\xDB\xDB\xDB\n"
	"\xDB\xDB \xDC\xDC\xDC\xDC\xDC \xDB\xDF  \xDF \xDF \xDC\xDB\xDF\xDB\xDC \xDB\xDB\n"
	"\xDB\xDB \xDB   \xDB \xDB \xDF\xDF\xDB \xDB\xDB \xDB\xDB \xDB\xDB\xDB\xDB\n"
	"\xDB\xDB \xDB\xDC\xDC\xDC\xDB \xDB\xDC\xDC\xDF\xDC\xDB\xDC\xDB\xDF \xDC \xDB\xDB\xDB\xDB\n"
	"\xDB\xDB\xDC\xDC\xDC\xDC\xDC\xDC\xDC\xDB\xDC\xDB\xDB\xDC\xDB\xDC\xDB\xDB\xDC\xDC\xDC\xDC\xDB\xDB\xDB\n"
	"\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB\n"
};

char expected_bitmap[3][100] = {
	{0b11111111,0b11111111,0b11111111,0b11111111,
	0b11111111,0b11111111,0b11111111,0b11111111,
	0b11000000,0b01010101,0b00000001,0b10000000,
	0b11011111,0b01110011,0b01111101,0b10000000,
	0b11010001,0b01100101,0b01000101,0b10000000,
	0b11010001,0b01000101,0b01000101,0b10000000,
	0b11010001,0b01011101,0b01000101,0b10000000,
	0b11011111,0b01010011,0b01111101,0b10000000,
	0b11000000,0b01010101,0b00000001,0b10000000,
	0b11111111,0b11010001,0b11111111,0b10000000,
	0b11011101,0b00000010,0b00001101,0b10000000,
	0b11001011,0b10110001,0b10101001,0b10000000,
	0b11010111,0b01101011,0b00000111,0b10000000,
	0b11001101,0b10000110,0b01010011,0b10000000,
	0b11001100,0b00010100,0b01110001,0b10000000,
	0b11111111,0b11001100,0b01110011,0b10000000,
	0b11000000,0b01001100,0b11111011,0b10000000,
	0b11011111,0b01100001,0b10101011,0b10000000,
	0b11010001,0b01010011,0b00000001,0b10000000,
	0b11010001,0b01101001,0b10101001,0b10000000,
	0b11010001,0b01101011,0b00000111,0b10000000,
	0b11011111,0b01101110,0b01010111,0b10000000,
	0b11000000,0b01000100,0b01110101,0b10000000,
	0b11111111,0b11111111,0b11111111,0b11111111,
	0b11111111,0b11111111,0b11111111,0b11111111},
	{0b11111111,0b11111111,0b11111111,0b11111111,
	0b11111111,0b11111111,0b11111111,0b11111111,
	0b11000000,0b01100001,0b00000001,0b10000000,
	0b11011111,0b01000111,0b01111101,0b10000000,
	0b11010001,0b01110011,0b01000101,0b10000000,
	0b11010001,0b01101011,0b01000101,0b10000000,
	0b11010001,0b01010001,0b01000101,0b10000000,
	0b11011111,0b01110011,0b01111101,0b10000000,
	0b11000000,0b01010101,0b00000001,0b10000000,
	0b11111111,0b11110111,0b11111111,0b10000000,
	0b11010101,0b01111101,0b11011011,0b10000000,
	0b11100000,0b10011010,0b10111011,0b10000000,
	0b11010111,0b00000010,0b00100001,0b10000000,
	0b11000010,0b11110000,0b10011011,0b10000000,
	0b11001011,0b00011010,0b00100001,0b10000000,
	0b11111111,0b11001111,0b01101111,0b10000000,
	0b11000000,0b01100101,0b11001001,0b10000000,
	0b11011111,0b01101111,0b01111011,0b10000000,
	0b11010001,0b01010101,0b01000001,0b10000000,
	0b11010001,0b01111010,0b10111011,0b10000000,
	0b11010001,0b01011010,0b00110101,0b10000000,
	0b11011111,0b01100000,0b10001011,0b10000000,
	0b11000000,0b01000010,0b00110001,0b10000000,
	0b11111111,0b11111111,0b11111111,0b11111111,
	0b11111111,0b11111111,0b11111111,0b11111111},
	{0b11111111,0b11111111,0b11111111,0b11111111,
	0b11111111,0b11111111,0b11111111,0b11111111,
	0b11000000,0b01100101,0b00000001,0b10000000,
	0b11011111,0b01101011,0b01111101,0b10000000,
	0b11010001,0b01000001,0b01000101,0b10000000,
	0b11010001,0b01001111,0b01000101,0b10000000,
	0b11010001,0b01011001,0b01000101,0b10000000,
	0b11011111,0b01011101,0b01111101,0b10000000,
	0b11000000,0b01010101,0b00000001,0b10000000,
	0b11111111,0b11000001,0b11111111,0b10000000,
	0b11010000,0b01101101,0b00000111,0b10000000,
	0b11010011,0b10110101,0b10101101,0b10000000,
	0b11110110,0b01011010,0b11000101,0b10000000,
	0b11011011,0b11100111,0b10010001,0b10000000,
	0b11101100,0b00100010,0b11010111,0b10000000,
	0b11111111,0b11011000,0b01101111,0b10000000,
	0b11000000,0b01100101,0b00111001,0b10000000,
	0b11011111,0b01000000,0b01101101,0b10000000,
	0b11010001,0b01011101,0b10110111,0b10000000,
	0b11010001,0b01000101,0b10110111,0b10000000,
	0b11010001,0b01001010,0b11000111,0b10000000,
	0b11011111,0b01110111,0b10010111,0b10000000,
	0b11000000,0b01011010,0b11000011,0b10000000,
	0b11111111,0b11111111,0b11111111,0b11111111,
	0b11111111,0b11111111,0b11111111,0b11111111}
};

char expected_bitmap2x2[3][400] = {
	{
	0b11111111,0b11111111,0b11111111,0b11111111,0b11111111,0b11111111,0b11111111,0b00000000,
	0b11111111,0b11111111,0b11111111,0b11111111,0b11111111,0b11111111,0b11111111,0b00000000,
	0b11111111,0b11111111,0b11111111,0b11111111,0b11111111,0b11111111,0b11111111,0b00000000,
	0b11111111,0b11111111,0b11111111,0b11111111,0b11111111,0b11111111,0b11111111,0b00000000,
	0b11110000,0b00000000,0b00110011,0b00110011,0b00000000,0b00000011,0b11000000,0b00000000,
	0b11110000,0b00000000,0b00110011,0b00110011,0b00000000,0b00000011,0b11000000,0b00000000,
	0b11110011,0b11111111,0b00111111,0b00001111,0b00111111,0b11110011,0b11000000,0b00000000,
	0b11110011,0b11111111,0b00111111,0b00001111,0b00111111,0b11110011,0b11000000,0b00000000,
	0b11110011,0b00000011,0b00111100,0b00110011,0b00110000,0b00110011,0b11000000,0b00000000,
	0b11110011,0b00000011,0b00111100,0b00110011,0b00110000,0b00110011,0b11000000,0b00000000,
	0b11110011,0b00000011,0b00110000,0b00110011,0b00110000,0b00110011,0b11000000,0b00000000,
	0b11110011,0b00000011,0b00110000,0b00110011,0b00110000,0b00110011,0b11000000,0b00000000,
	0b11110011,0b00000011,0b00110011,0b11110011,0b00110000,0b00110011,0b11000000,0b00000000,
	0b11110011,0b00000011,0b00110011,0b11110011,0b00110000,0b00110011,0b11000000,0b00000000,
	0b11110011,0b11111111,0b00110011,0b00001111,0b00111111,0b11110011,0b11000000,0b00000000,
	0b11110011,0b11111111,0b00110011,0b00001111,0b00111111,0b11110011,0b11000000,0b00000000,
	0b11110000,0b00000000,0b00110011,0b00110011,0b00000000,0b00000011,0b11000000,0b00000000,
	0b11110000,0b00000000,0b00110011,0b00110011,0b00000000,0b00000011,0b11000000,0b00000000,
	0b11111111,0b11111111,0b11110011,0b00000011,0b11111111,0b11111111,0b11000000,0b00000000,
	0b11111111,0b11111111,0b11110011,0b00000011,0b11111111,0b11111111,0b11000000,0b00000000,
	0b11110011,0b11110011,0b00000000,0b00001100,0b00000000,0b11110011,0b11000000,0b00000000,
	0b11110011,0b11110011,0b00000000,0b00001100,0b00000000,0b11110011,0b11000000,0b00000000,
	0b11110000,0b11001111,0b11001111,0b00000011,0b11001100,0b11000011,0b11000000,0b00000000,
	0b11110000,0b11001111,0b11001111,0b00000011,0b11001100,0b11000011,0b11000000,0b00000000,
	0b11110011,0b00111111,0b00111100,0b11001111,0b00000000,0b00111111,0b11000000,0b00000000,
	0b11110011,0b00111111,0b00111100,0b11001111,0b00000000,0b00111111,0b11000000,0b00000000,
	0b11110000,0b11110011,0b11000000,0b00111100,0b00110011,0b00001111,0b11000000,0b00000000,
	0b11110000,0b11110011,0b11000000,0b00111100,0b00110011,0b00001111,0b11000000,0b00000000,
	0b11110000,0b11110000,0b00000011,0b00110000,0b00111111,0b00000011,0b11000000,0b00000000,
	0b11110000,0b11110000,0b00000011,0b00110000,0b00111111,0b00000011,0b11000000,0b00000000,
	0b11111111,0b11111111,0b11110000,0b11110000,0b00111111,0b00001111,0b11000000,0b00000000,
	0b11111111,0b11111111,0b11110000,0b11110000,0b00111111,0b00001111,0b11000000,0b00000000,
	0b11110000,0b00000000,0b00110000,0b11110000,0b11111111,0b11001111,0b11000000,0b00000000,
	0b11110000,0b00000000,0b00110000,0b11110000,0b11111111,0b11001111,0b11000000,0b00000000,
	0b11110011,0b11111111,0b00111100,0b00000011,0b11001100,0b11001111,0b11000000,0b00000000,
	0b11110011,0b11111111,0b00111100,0b00000011,0b11001100,0b11001111,0b11000000,0b00000000,
	0b11110011,0b00000011,0b00110011,0b00001111,0b00000000,0b00000011,0b11000000,0b00000000,
	0b11110011,0b00000011,0b00110011,0b00001111,0b00000000,0b00000011,0b11000000,0b00000000,
	0b11110011,0b00000011,0b00111100,0b11000011,0b11001100,0b11000011,0b11000000,0b00000000,
	0b11110011,0b00000011,0b00111100,0b11000011,0b11001100,0b11000011,0b11000000,0b00000000,
	0b11110011,0b00000011,0b00111100,0b11001111,0b00000000,0b00111111,0b11000000,0b00000000,
	0b11110011,0b00000011,0b00111100,0b11001111,0b00000000,0b00111111,0b11000000,0b00000000,
	0b11110011,0b11111111,0b00111100,0b11111100,0b00110011,0b00111111,0b11000000,0b00000000,
	0b11110011,0b11111111,0b00111100,0b11111100,0b00110011,0b00111111,0b11000000,0b00000000,
	0b11110000,0b00000000,0b00110000,0b00110000,0b00111111,0b00110011,0b11000000,0b00000000,
	0b11110000,0b00000000,0b00110000,0b00110000,0b00111111,0b00110011,0b11000000,0b00000000,
	0b11111111,0b11111111,0b11111111,0b11111111,0b11111111,0b11111111,0b11111111,0b00000000,
	0b11111111,0b11111111,0b11111111,0b11111111,0b11111111,0b11111111,0b11111111,0b00000000,
	0b11111111,0b11111111,0b11111111,0b11111111,0b11111111,0b11111111,0b11111111,0b00000000,
	0b11111111,0b11111111,0b11111111,0b11111111,0b11111111,0b11111111,0b11111111,0b00000000},
	
	{
	0b11111111,0b11111111,0b11111111,0b11111111,0b11111111,0b11111111,0b11111111,0b00000000,
	0b11111111,0b11111111,0b11111111,0b11111111,0b11111111,0b11111111,0b11111111,0b00000000,
	0b11111111,0b11111111,0b11111111,0b11111111,0b11111111,0b11111111,0b11111111,0b00000000,
	0b11111111,0b11111111,0b11111111,0b11111111,0b11111111,0b11111111,0b11111111,0b00000000,
	0b11110000,0b00000000,0b00111100,0b00000011,0b00000000,0b00000011,0b11000000,0b00000000,
	0b11110000,0b00000000,0b00111100,0b00000011,0b00000000,0b00000011,0b11000000,0b00000000,
	0b11110011,0b11111111,0b00110000,0b00111111,0b00111111,0b11110011,0b11000000,0b00000000,
	0b11110011,0b11111111,0b00110000,0b00111111,0b00111111,0b11110011,0b11000000,0b00000000,
	0b11110011,0b00000011,0b00111111,0b00001111,0b00110000,0b00110011,0b11000000,0b00000000,
	0b11110011,0b00000011,0b00111111,0b00001111,0b00110000,0b00110011,0b11000000,0b00000000,
	0b11110011,0b00000011,0b00111100,0b11001111,0b00110000,0b00110011,0b11000000,0b00000000,
	0b11110011,0b00000011,0b00111100,0b11001111,0b00110000,0b00110011,0b11000000,0b00000000,
	0b11110011,0b00000011,0b00110011,0b00000011,0b00110000,0b00110011,0b11000000,0b00000000,
	0b11110011,0b00000011,0b00110011,0b00000011,0b00110000,0b00110011,0b11000000,0b00000000,
	0b11110011,0b11111111,0b00111111,0b00001111,0b00111111,0b11110011,0b11000000,0b00000000,
	0b11110011,0b11111111,0b00111111,0b00001111,0b00111111,0b11110011,0b11000000,0b00000000,
	0b11110000,0b00000000,0b00110011,0b00110011,0b00000000,0b00000011,0b11000000,0b00000000,
	0b11110000,0b00000000,0b00110011,0b00110011,0b00000000,0b00000011,0b11000000,0b00000000,
	0b11111111,0b11111111,0b11111111,0b00111111,0b11111111,0b11111111,0b11000000,0b00000000,
	0b11111111,0b11111111,0b11111111,0b00111111,0b11111111,0b11111111,0b11000000,0b00000000,
	0b11110011,0b00110011,0b00111111,0b11110011,0b11110011,0b11001111,0b11000000,0b00000000,
	0b11110011,0b00110011,0b00111111,0b11110011,0b11110011,0b11001111,0b11000000,0b00000000,
	0b11111100,0b00000000,0b11000011,0b11001100,0b11001111,0b11001111,0b11000000,0b00000000,
	0b11111100,0b00000000,0b11000011,0b11001100,0b11001111,0b11001111,0b11000000,0b00000000,
	0b11110011,0b00111111,0b00000000,0b00001100,0b00001100,0b00000011,0b11000000,0b00000000,
	0b11110011,0b00111111,0b00000000,0b00001100,0b00001100,0b00000011,0b11000000,0b00000000,
	0b11110000,0b00001100,0b11111111,0b00000000,0b11000011,0b11001111,0b11000000,0b00000000,
	0b11110000,0b00001100,0b11111111,0b00000000,0b11000011,0b11001111,0b11000000,0b00000000,
	0b11110000,0b11001111,0b00000011,0b11001100,0b00001100,0b00000011,0b11000000,0b00000000,
	0b11110000,0b11001111,0b00000011,0b11001100,0b00001100,0b00000011,0b11000000,0b00000000,
	0b11111111,0b11111111,0b11110000,0b11111111,0b00111100,0b11111111,0b11000000,0b00000000,
	0b11111111,0b11111111,0b11110000,0b11111111,0b00111100,0b11111111,0b11000000,0b00000000,
	0b11110000,0b00000000,0b00111100,0b00110011,0b11110000,0b11000011,0b11000000,0b00000000,
	0b11110000,0b00000000,0b00111100,0b00110011,0b11110000,0b11000011,0b11000000,0b00000000,
	0b11110011,0b11111111,0b00111100,0b11111111,0b00111111,0b11001111,0b11000000,0b00000000,
	0b11110011,0b11111111,0b00111100,0b11111111,0b00111111,0b11001111,0b11000000,0b00000000,
	0b11110011,0b00000011,0b00110011,0b00110011,0b00110000,0b00000011,0b11000000,0b00000000,
	0b11110011,0b00000011,0b00110011,0b00110011,0b00110000,0b00000011,0b11000000,0b00000000,
	0b11110011,0b00000011,0b00111111,0b11001100,0b11001111,0b11001111,0b11000000,0b00000000,
	0b11110011,0b00000011,0b00111111,0b11001100,0b11001111,0b11001111,0b11000000,0b00000000,
	0b11110011,0b00000011,0b00110011,0b11001100,0b00001111,0b00110011,0b11000000,0b00000000,
	0b11110011,0b00000011,0b00110011,0b11001100,0b00001111,0b00110011,0b11000000,0b00000000,
	0b11110011,0b11111111,0b00111100,0b00000000,0b11000000,0b11001111,0b11000000,0b00000000,
	0b11110011,0b11111111,0b00111100,0b00000000,0b11000000,0b11001111,0b11000000,0b00000000,
	0b11110000,0b00000000,0b00110000,0b00001100,0b00001111,0b00000011,0b11000000,0b00000000,
	0b11110000,0b00000000,0b00110000,0b00001100,0b00001111,0b00000011,0b11000000,0b00000000,
	0b11111111,0b11111111,0b11111111,0b11111111,0b11111111,0b11111111,0b11111111,0b00000000,
	0b11111111,0b11111111,0b11111111,0b11111111,0b11111111,0b11111111,0b11111111,0b00000000,
	0b11111111,0b11111111,0b11111111,0b11111111,0b11111111,0b11111111,0b11111111,0b00000000,
	0b11111111,0b11111111,0b11111111,0b11111111,0b11111111,0b11111111,0b11111111,0b00000000},

	{
	0b11111111,0b11111111,0b11111111,0b11111111,0b11111111,0b11111111,0b11111111,0b00000000,
	0b11111111,0b11111111,0b11111111,0b11111111,0b11111111,0b11111111,0b11111111,0b00000000,
	0b11111111,0b11111111,0b11111111,0b11111111,0b11111111,0b11111111,0b11111111,0b00000000,
	0b11111111,0b11111111,0b11111111,0b11111111,0b11111111,0b11111111,0b11111111,0b00000000,
	0b11110000,0b00000000,0b00111100,0b00110011,0b00000000,0b00000011,0b11000000,0b00000000,
	0b11110000,0b00000000,0b00111100,0b00110011,0b00000000,0b00000011,0b11000000,0b00000000,
	0b11110011,0b11111111,0b00111100,0b11001111,0b00111111,0b11110011,0b11000000,0b00000000,
	0b11110011,0b11111111,0b00111100,0b11001111,0b00111111,0b11110011,0b11000000,0b00000000,
	0b11110011,0b00000011,0b00110000,0b00000011,0b00110000,0b00110011,0b11000000,0b00000000,
	0b11110011,0b00000011,0b00110000,0b00000011,0b00110000,0b00110011,0b11000000,0b00000000,
	0b11110011,0b00000011,0b00110000,0b11111111,0b00110000,0b00110011,0b11000000,0b00000000,
	0b11110011,0b00000011,0b00110000,0b11111111,0b00110000,0b00110011,0b11000000,0b00000000,
	0b11110011,0b00000011,0b00110011,0b11000011,0b00110000,0b00110011,0b11000000,0b00000000,
	0b11110011,0b00000011,0b00110011,0b11000011,0b00110000,0b00110011,0b11000000,0b00000000,
	0b11110011,0b11111111,0b00110011,0b11110011,0b00111111,0b11110011,0b11000000,0b00000000,
	0b11110011,0b11111111,0b00110011,0b11110011,0b00111111,0b11110011,0b11000000,0b00000000,
	0b11110000,0b00000000,0b00110011,0b00110011,0b00000000,0b00000011,0b11000000,0b00000000,
	0b11110000,0b00000000,0b00110011,0b00110011,0b00000000,0b00000011,0b11000000,0b00000000,
	0b11111111,0b11111111,0b11110000,0b00000011,0b11111111,0b11111111,0b11000000,0b00000000,
	0b11111111,0b11111111,0b11110000,0b00000011,0b11111111,0b11111111,0b11000000,0b00000000,
	0b11110011,0b00000000,0b00111100,0b11110011,0b00000000,0b00111111,0b11000000,0b00000000,
	0b11110011,0b00000000,0b00111100,0b11110011,0b00000000,0b00111111,0b11000000,0b00000000,
	0b11110011,0b00001111,0b11001111,0b00110011,0b11001100,0b11110011,0b11000000,0b00000000,
	0b11110011,0b00001111,0b11001111,0b00110011,0b11001100,0b11110011,0b11000000,0b00000000,
	0b11111111,0b00111100,0b00110011,0b11001100,0b11110000,0b00110011,0b11000000,0b00000000,
	0b11111111,0b00111100,0b00110011,0b11001100,0b11110000,0b00110011,0b11000000,0b00000000,
	0b11110011,0b11001111,0b11111100,0b00111111,0b11000011,0b00000011,0b11000000,0b00000000,
	0b11110011,0b11001111,0b11111100,0b00111111,0b11000011,0b00000011,0b11000000,0b00000000,
	0b11111100,0b11110000,0b00001100,0b00001100,0b11110011,0b00111111,0b11000000,0b00000000,
	0b11111100,0b11110000,0b00001100,0b00001100,0b11110011,0b00111111,0b11000000,0b00000000,
	0b11111111,0b11111111,0b11110011,0b11000000,0b00111100,0b11111111,0b11000000,0b00000000,
	0b11111111,0b11111111,0b11110011,0b11000000,0b00111100,0b11111111,0b11000000,0b00000000,
	0b11110000,0b00000000,0b00111100,0b00110011,0b00001111,0b11000011,0b11000000,0b00000000,
	0b11110000,0b00000000,0b00111100,0b00110011,0b00001111,0b11000011,0b11000000,0b00000000,
	0b11110011,0b11111111,0b00110000,0b00000000,0b00111100,0b11110011,0b11000000,0b00000000,
	0b11110011,0b11111111,0b00110000,0b00000000,0b00111100,0b11110011,0b11000000,0b00000000,
	0b11110011,0b00000011,0b00110011,0b11110011,0b11001111,0b00111111,0b11000000,0b00000000,
	0b11110011,0b00000011,0b00110011,0b11110011,0b11001111,0b00111111,0b11000000,0b00000000,
	0b11110011,0b00000011,0b00110000,0b00110011,0b11001111,0b00111111,0b11000000,0b00000000,
	0b11110011,0b00000011,0b00110000,0b00110011,0b11001111,0b00111111,0b11000000,0b00000000,
	0b11110011,0b00000011,0b00110000,0b11001100,0b11110000,0b00111111,0b11000000,0b00000000,
	0b11110011,0b00000011,0b00110000,0b11001100,0b11110000,0b00111111,0b11000000,0b00000000,
	0b11110011,0b11111111,0b00111111,0b00111111,0b11000011,0b00111111,0b11000000,0b00000000,
	0b11110011,0b11111111,0b00111111,0b00111111,0b11000011,0b00111111,0b11000000,0b00000000,
	0b11110000,0b00000000,0b00110011,0b11001100,0b11110000,0b00001111,0b11000000,0b00000000,
	0b11110000,0b00000000,0b00110011,0b11001100,0b11110000,0b00001111,0b11000000,0b00000000,
	0b11111111,0b11111111,0b11111111,0b11111111,0b11111111,0b11111111,0b11111111,0b00000000,
	0b11111111,0b11111111,0b11111111,0b11111111,0b11111111,0b11111111,0b11111111,0b00000000,
	0b11111111,0b11111111,0b11111111,0b11111111,0b11111111,0b11111111,0b11111111,0b00000000,
	0b11111111,0b11111111,0b11111111,0b11111111,0b11111111,0b11111111,0b11111111,0b00000000}
};


void generate_and_compare(DisplayQR * displayqr, char * input, char * expectedout) {
	displayqr_generate(displayqr, input);
	Buffer * output = displayqr_get_output(displayqr);
	buffer_append(output, "", 1); // Just to make sure it is a null terminated string
	ck_assert_str_eq(buffer_get_buffer(output), expectedout);
}

START_TEST (test_displayqr) {
	DisplayQR * displayqr;
	int i;
	for (i = 0; i < 3; i++) {
		displayqr = displayqr_new();
		generate_and_compare(displayqr, input[i], expected_color_utf8[i]);
		displayqr_delete(displayqr);
	}
}
END_TEST

START_TEST (test_displayqr_colorless_utf8) {
	DisplayQR * displayqr;
	int i;
	for (i = 0; i < 3; i++) {
		displayqr = displayqr_new_params(QRMODE_COLORLESS_UTF8);
		generate_and_compare(displayqr, input[i], expected_colorless_utf8[i]);
		displayqr_delete(displayqr);
	}
}
END_TEST

START_TEST (test_displayqr_ansi) {
	DisplayQR * displayqr;
	int i;
	for (i = 0; i < 3; i++) {
		displayqr = displayqr_new_params(QRMODE_ANSI);
		generate_and_compare(displayqr, input[i], expected_ansi[i]);
		displayqr_delete(displayqr);
	}
}
END_TEST

START_TEST (test_displayqr_dos) {
	DisplayQR * displayqr;
	int i;
	for (i = 0; i < 3; i++) {
		displayqr = displayqr_new_params(QRMODE_DOS);
		generate_and_compare(displayqr, input[i], expected_dos[i]);
		displayqr_delete(displayqr);
	}
}
END_TEST

START_TEST (test_displayqr_bitmap) {
	DisplayQR * displayqr;
	int i;
	for (i = 0; i < 3; i++) {
		displayqr = displayqr_new_params(QRMODE_MONOCHROME_BITMAP);
		displayqr_generate(displayqr, input[i]);
		Buffer * output = displayqr_get_output(displayqr);
		ck_assert(memcmp(buffer_get_buffer(output), expected_bitmap[i], sizeof(expected_bitmap)/3) == 0);
		displayqr_delete(displayqr);
	}
}
END_TEST

START_TEST (test_displayqr_bitmap22) {
	DisplayQR * displayqr;
	int i;
	int pixels = 2;
	for (i = 0; i < 3; i++) {
		displayqr = displayqr_new_params(QRMODE_MONOCHROME_BITMAP);
		displayqr_set_mode_params(displayqr, &pixels);
		displayqr_generate(displayqr, input[i]);
		Buffer * output = displayqr_get_output(displayqr);
		char * outputptr = buffer_get_buffer(output);
		ck_assert(memcmp(outputptr, expected_bitmap2x2[i], sizeof(expected_bitmap2x2)/3) == 0);
		displayqr_delete(displayqr);
	}
}
END_TEST

int main (void) {
	int number_failed;
	Suite * s;
	SRunner *sr;
	TCase * tc;

	s = suite_create("Libpico");

	// Base64 test case
	tc = tcase_create("DisplayQR");
	tcase_add_test(tc, test_displayqr);
	tcase_add_test(tc, test_displayqr_colorless_utf8);
	tcase_add_test(tc, test_displayqr_ansi);
	tcase_add_test(tc, test_displayqr_dos);
	tcase_add_test(tc, test_displayqr_bitmap);
	tcase_add_test(tc, test_displayqr_bitmap22);
	suite_add_tcase(s, tc);
	sr = srunner_create(s);

	srunner_run_all(sr, CK_NORMAL);
	number_failed = srunner_ntests_failed(sr);
	srunner_free(sr);

	return (number_failed == 0) ? 0 : -1;
}

