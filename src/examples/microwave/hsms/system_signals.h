#ifndef _SYSTEM_SIGNALS_H_
#define _SYSTEM_SIGNALS_H_

#include <QuarkTS.h>

#define SYS_SIG_OPEN	      ((qSM_Signal_t)(1))
#define SYS_SIG_CLOSE	      ((qSM_Signal_t)(2))
#define SYS_SIG_TOAST       ((qSM_Signal_t)(3))
#define SYS_SIG_BAKE        ((qSM_Signal_t)(4))
#define SYS_SIG_OFFTIMEOUT  ( QSM_SIGNAL_TIMEOUT (1))

#endif // _SYSTEM_SIGNALS_H_