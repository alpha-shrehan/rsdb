#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <assert.h>
#include <stdarg.h>
#include <stdbool.h>

#ifdef _WIN32
#include "windows.h"
#endif

#define RS_API

#include "mem.h"

enum
{
    RSERR_OK,
    RSERR_NOFILE,
    RSERR_WRITE_ERR,
    RSERR_FILE_INVALIDPERMS,
};