/*!
 * @file qps.c
 * @author Michal Rostkowski
 * @note This file is part of the QuarkTS distribution.
 **/

#include <QuarkTS.h>
#include "qps.h"

#include <stdio.h>

static qSM_SignalType_t  psSignals[Q_PS_SIGNALS_MAX];
static qSM_t            *psSubs[Q_PS_SIGNALS_MAX][Q_PS_SUBS_PER_SIGNAL_MAX];

/*============================================================================*/
qBool_t qPS_Init()
{
    qBool_t retValue = qFalse;
    qUINT32_t signal, sub;

    qDebug_Message ("<qPS_Init");

    for (signal = 0; signal < Q_PS_SIGNALS_MAX; signal++) {
        psSignals[ signal ] = QSM_SIGNAL_TYPE_NONE;

        for (sub = 0; sub < Q_PS_SUBS_PER_SIGNAL_MAX; sub++) {
            psSubs[ signal ][ sub ] = NULL;
        }
    }

    qDebug_Message ("qPS_Init>");

    return retValue;
}

/*============================================================================*/
qBool_t qPS_Sub( qSM_SignalType_t signalType,
                 qSM_t *subscriber )
{
    qBool_t retValue = qFalse;
    qUINT32_t sigIdx, subIdx, freeSigIdx;

    if ((subscriber != NULL) && (signalType < QSM_SIGNAL_RANGE_MAX)) {
        retValue = qPS_IsSub (signalType, subscriber);
        if (qTrue != retValue) {
            freeSigIdx = Q_PS_SIGNALS_MAX;
            for (sigIdx = 0; (sigIdx < Q_PS_SIGNALS_MAX) && (qFalse == retValue); sigIdx++) {
                if (psSignals[sigIdx] == signalType) {
                    for (subIdx = 0; (subIdx < Q_PS_SUBS_PER_SIGNAL_MAX) && (qFalse == retValue); subIdx++) {
                        if (psSubs[ sigIdx ][ subIdx ] == NULL) {
                            psSubs[ sigIdx ][ subIdx ] = subscriber;
                            retValue = qTrue;
                        }
                    }
                } else if (psSignals[sigIdx] == QSM_SIGNAL_TYPE_NONE) {
                    if (freeSigIdx >= Q_PS_SIGNALS_MAX) {
                        freeSigIdx = sigIdx;
                    }
                }
            }

            if (( qFalse == retValue ) && ( Q_PS_SIGNALS_MAX > freeSigIdx )) {
                psSignals[ freeSigIdx ] = signalType;
                psSubs[ freeSigIdx ] [ 0 ] = subscriber;
                retValue = qTrue;
            }
        }
    }

    return retValue;
}

qBool_t qPS_UnSub( qSM_SignalType_t signalType,
                   qSM_t *subscriber )
{
    qBool_t retValue = qFalse;
    qUINT32_t sigIdx, subIdx;

    if ((subscriber != NULL) && (signalType < QSM_SIGNAL_RANGE_MAX)) {
        for (sigIdx = 0; (sigIdx < Q_PS_SIGNALS_MAX) && (qFalse == retValue); sigIdx++) {
            if (psSignals[sigIdx] == signalType) {
                for (subIdx = 0; (subIdx < Q_PS_SUBS_PER_SIGNAL_MAX) && (qFalse == retValue); subIdx++) {
                    if (psSubs[ sigIdx ][ subIdx ] == subscriber) {
                        psSubs[ sigIdx ][ subIdx ] = NULL;
                        retValue = qTrue;
                    }
                }
            }
        }
    }

    return retValue;
}

qBool_t qPS_IsSub( qSM_SignalType_t signalType,
                   qSM_t *subscriber )
{
    qBool_t retValue = qFalse;
    qUINT32_t sigIdx, subIdx;

    if ((subscriber != NULL) && (signalType < QSM_SIGNAL_RANGE_MAX)) {
        for (sigIdx = 0; (sigIdx < Q_PS_SIGNALS_MAX) && (qFalse == retValue); sigIdx++) {
            if (psSignals[sigIdx] == signalType) {
                for (subIdx = 0; (subIdx < Q_PS_SUBS_PER_SIGNAL_MAX) && (qFalse == retValue); subIdx++) {
                    if (psSubs[ sigIdx ][ subIdx ] == subscriber) {
                        retValue = qTrue;
                    }
                }
            }
        }
    }

    return retValue;
}
/*============================================================================*/

qBool_t qPS_Pub( qSM_SignalType_t signalType, void *data, size_t dataSize )
{
    qBool_t retValue = qFalse;
    qUINT32_t sigIdx, subIdx;

    if (signalType < QSM_SIGNAL_RANGE_MAX) {

        for (sigIdx = 0; sigIdx < Q_PS_SIGNALS_MAX; sigIdx++) {
            if (psSignals[ sigIdx ] == signalType) {
                for (subIdx = 0; subIdx < Q_PS_SUBS_PER_SIGNAL_MAX; subIdx++) {
                    if (psSubs[ sigIdx ][ subIdx ] != NULL) {
                        qStateMachine_SendSignal (psSubs[ sigIdx ][ subIdx ], signalType, data, dataSize, qFalse);
                    }
                }
                retValue = qTrue;
                break;
            }
        }
    }

    return retValue;
}

/*============================================================================*/