/* Copyright (C) 2000-2020 by Massimiliano Ghilardi
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 */
#ifndef _TWIN_STL_DEFS_H
#define _TWIN_STL_DEFS_H

#if defined(__cplusplus) && __cplusplus >= 201103L
#define STL_CXX11
#define STL_USING using
#else
#undef STL_CXX11
#define STL_USING
#endif

#endif /* _TWIN_STL_DEFS_H */
