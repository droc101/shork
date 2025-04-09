//
// Created by droc101 on 4/9/25.
//

#include "args.h"
#include <string.h>

char *get_arg(const int argc, char *argv[], const char *argument, char *default_value)
{
    for (int i = 0; i < argc; i++)
    {
        if (strncmp(argv[i], argument, strlen(argument)) == 0)
        {
            char *value = strchr(argv[i], '=');
            if (value != NULL)
            {
                return value + 1;
            }
        }
    }
    return default_value;
}
