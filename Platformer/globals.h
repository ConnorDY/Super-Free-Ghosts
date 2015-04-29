#ifndef GLOBALS_H
#define GLOBALS_H

#define DEBUG_MODE true
#if DEBUG_MODE
#define DEBUG(x) x
#else
#define DEBUG(x)
#endif

#define VIEW_WIDTH 400
#define VIEW_HEIGHT 300

#define WINDOW_WIDTH VIEW_WIDTH * 2
#define WINDOW_HEIGHT VIEW_HEIGHT * 2

#ifdef __GNUC__
#define GCC(x) x
#else
#define GCC(x)
#endif

#endif
