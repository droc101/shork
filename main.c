#include <signal.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include "args.h"
#include "platform/platform.h"

PlatformID platform = PLATFORM_CONSOLE;

int main(const int argc, char *argv[])
{
    if (has_arg(argc, argv, "-h") || has_arg(argc, argv, "--help"))
    {
        printf("Usage: %s [--flag=<value>] [--platform=<value>]\n", argv[0]);
        printf("Options:\n");
        printf("  --flag=<value>  Set the flag value (default: rainbow)\n");
        printf("  --platform=[console|sdl]  Set the platform to use (default: console)\n\n");
        printf("Flags should be placed in \"assets/flags\" named <value>.png\n");
        return 0;
    }

    char *path = argv[0];
    char *lastSlash = strrchr(path, '/');
    if (lastSlash != NULL)
    {
        *lastSlash = '\0';
    }
    else
    {
        path = ".";
    }
    if (chdir(path) != 0)
    {
        fprintf(stderr, "Error changing directory to %s\n", path);
        return 1;
    }

    const char *flag = get_arg(argc, argv, "--flag", "rainbow");

    const char *platform_arg = get_arg(argc, argv, "--platform", "console");
    if (strncmp(platform_arg, "console", strlen("console")) == 0)
    {
        platform = PLATFORM_CONSOLE;
    } else if (strncmp(platform_arg, "sdl", strlen("sdl")) == 0)
    {
        platform = PLATFORM_SDL;
    }

    if (!platforms[platform].init(flag)) return 1;

    platforms[platform].loop();

    platforms[platform].cleanup();
    fflush(stdout);
    return 0;
}
