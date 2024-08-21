#pragma once

#include <cerrno>
#include <cstdio>
#include <iostream>

#ifdef __OPTIMIZE__
#define DEBUG 0
#else
#define DEBUG 1
#endif

#define RESET "\033[;0m"
#define BOLD "\033[0;1m"
#define BOLD_RED "\033[1;31m"

#define error(msg)                                                             \
    do {                                                                       \
        std::cerr << BOLD << "cale: " << BOLD_RED << "error" << RESET << ": "  \
                  << msg << std::endl;                                         \
    } while (0)

#define perr(msg)                                                              \
    do {                                                                       \
        std::cerr << BOLD << "cale: " << BOLD_RED << "error" << RESET << ": "; \
        std::perror(msg);                                                      \
    } while (0)

#if DEBUG
#define debug(msg)                                                             \
    do {                                                                       \
        std::cout << BOLD << "Debug [" << __FILE__ << ":" << __LINE__          \
                  << "]: " << RESET << msg << std::endl;                       \
    } while (0)
#else
#define debug(msg)                                                             \
    do {                                                                       \
    } while (0)
#endif

#define STRINGIFY(x) #x
