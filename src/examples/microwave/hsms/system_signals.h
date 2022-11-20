#ifndef _SYSTEM_SIGNALS_H_
#define _SYSTEM_SIGNALS_H_

#include <QuarkTS.h>

#define SYS_SIG_TYPE_OPEN	       ((qSM_SignalType_t)(1))
#define SYS_SIG_TYPE_CLOSE       ((qSM_SignalType_t)(2))
#define SYS_SIG_TYPE_TOAST       ((qSM_SignalType_t)(3))
#define SYS_SIG_TYPE_BAKE        ((qSM_SignalType_t)(4))
#define SYS_SIG_TYPE_OFFTIMEOUT  ( QSM_SIGNAL_TYPE_TIMEOUT (1))

#endif // _SYSTEM_SIGNALS_H_