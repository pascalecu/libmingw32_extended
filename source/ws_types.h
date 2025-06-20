#ifndef _WS_TYPES_H
#define _WS_TYPES_H

#include <sys/cdefs.h>

__BEGIN_DECLS

#define WS_SYM(name, type, ret, ...) \
    typedef ret (*type)(__VA_ARGS__);
#include "ws_symbols.def"
#undef WS_SYM

__END_DECLS
#endif /* _WS_TYPES_H */
