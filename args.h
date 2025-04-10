//
// Created by droc101 on 4/9/25.
//

#ifndef ARGS_H
#define ARGS_H
#include <stdbool.h>

/**
 * Get the value of an argument from the command line arguments.
 * @param argc The number of command line arguments.
 * @param argv The command line arguments.
 * @param argument The argument to search for (e.g. "--arg").
 * @param default_value The default value to return if the argument is not found.
 * @return The value of the argument, or the default value if not found.
 * @note Surely this is entirely memory safe, surely.
 */
char *get_arg(int argc, char *argv[], const char *argument, char *default_value);

/**
 * Check if an argument is present in the command line arguments.
 * @param argc The number of command line arguments.
 * @param argv The command line arguments.
 * @param argument The argument to search for (e.g. "--arg").
 * @return Whether the argument is present or not.
 */
bool has_arg(int argc, char *argv[], const char *argument);

#endif //ARGS_H
