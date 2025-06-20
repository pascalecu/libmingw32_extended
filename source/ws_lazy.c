#include <windows.h>
#include <dlfcn.h>
#include "ws_types.h"

static INIT_ONCE once = INIT_ONCE_STATIC_INIT;
static HANDLE ws2_handle = NULL;

#define WS_SYM(name, type) type p_##name = NULL;
#include "ws_symbols.def"
#undef WS_SYM

typedef struct
{
  const char *name;
  PVOID *fn_ptr;
} SymbolEntry;

static const SymbolEntry ws2_symbols[] = {
#define WS_SYM(name, type) { #name, (PVOID *) &p_##name },
#include "ws_symbols.def"
#undef WS_SYM
};

#define WS2_SYMBOL_COUNT (sizeof (ws2_symbols) / sizeof (ws2_symbols[0]))

static BOOL CALLBACK
init_ws2 (PINIT_ONCE InitOnce, PVOID param, PVOID * ctx)
{
  PVOID handle = dlopen ("ws2_32.dll", RTLD_LAZY);
  if (!handle)
    return FALSE;

  for (size_t i = 0; i < WS2_SYMBOL_COUNT; ++i)
    {
      PVOID sym = dlsym (handle, ws2_symbols[i].name);
      if (!sym)
	{
	  dlclose (handle);
	  return FALSE;
	}
      *ws2_symbols[i].fn_ptr = sym;
    }

  ws2_handle = handle;
  return TRUE;
}

static void
ws2_unload (void)
{
  if (ws2_handle)
    {
#define WS_SYM(name, type) p_##name = NULL;
#include "ws_symbols.def"
#undef WS_SYM

      dlclose (ws2_handle);
      ws2_handle = NULL;
    }
}

__attribute__((destructor))
     static void ws2_cleanup_on_exit (void)
{
  ws2_unload ();
}

void
ensure_ws2_loaded (void)
{
  InitOnceExecuteOnce (&once, init_ws2, NULL, NULL);
}
