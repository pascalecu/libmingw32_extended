#ifndef _WS2_LAZY_H
#define _WS2_LAZY_H

#include "ws_types.h"

#define WS_SYM(name, type) extern type p_##name;
#include "ws_symbols.def"
#undef WS_SYM

void ensure_ws2_loaded (void);

#endif // _WS2_LAZY_H
