// printf_(err/warn/abortion) prints errors in console, but in debug mode also in log file,
// printf_log_(msg/grey/bold) only in debug mode prints in console and log file
// printf_note prints only in console in both modes

# ifndef DEBUG_H
# define DEBUG_H

#include <stdio.h>
#include "font_styles.h"

extern FILE* log_ptr;

enum err_t
{
    ok,
    error
};

enum md_t
{
    off,
    on
};

#define printf_log_msg(mode, ...) do{           \
    if (mode == on)                             \
    {                                           \
        printf(__VA_ARGS__);                    \
        fprintf(log_ptr, __VA_ARGS__);          \
    }                                           \
}                                               \
while(0)

#define printf_log_grey(mode, text, ...) do{                                \
    if (mode == on)                                                         \
    {                                                                       \
        printf(MAKE_GREY(text), __VA_ARGS__);                               \
        fprintf(log_ptr, text, __VA_ARGS__);                                \
    }                                                                       \
}                                                                           \
while(0)

#define printf_log_bold(mode, text, ...) do{                                \
    if (mode == on)                                                         \
    {                                                                       \
        printf(MAKE_BOLD(text), __VA_ARGS__);                               \
        fprintf(log_ptr, text, __VA_ARGS__);                                \
    }                                                                       \
}                                                                           \
while(0)

#define printf_err(mode, ...) do{                                             \
    printf("\n");                                                             \
    printf( MAKE_BOLD_RED("ERROR: ") __VA_ARGS__);                            \
    if (mode == on)                                                           \
    {                                                                         \
        fprintf(log_ptr, "\n");                                               \
        fprintf(log_ptr, "\n" "ERROR: " __VA_ARGS__);                         \
    }                                                                         \
}                                                                             \
while(0)

#define printf_warn(mode, ...) do{                                          \
    printf("\n");                                                           \
    printf("\n" MAKE_BOLD("WARNING: ") __VA_ARGS__);                        \
    if (mode == on)                                                         \
    {                                                                       \
        fprintf(log_ptr, "\n");                                             \
        fprintf(log_ptr, "\n" "WARNING: " __VA_ARGS__);                     \
    }                                                                       \
}                                                                           \
while(0)

#define printf_note(mode, text, ...) do{           \
    printf(MAKE_GREY(text), __VA_ARGS__);          \
}                                                  \
while(0)

#define printf_abortion(mode, ...) do{                                      \
    printf(MAKE_BOLD_RED("aborting due to error: ") __VA_ARGS__);           \
    if (mode == on)                                                         \
        fprintf(log_ptr, "aborting due to error: " __VA_ARGS__);            \
}                                                                           \
while(0)

#endif
