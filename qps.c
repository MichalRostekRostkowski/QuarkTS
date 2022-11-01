/*!
 * @file qps.c
 * @author Michal Rostkowski
 * @note This file is part of the QuarkTS distribution.
 **/

#include <QuarkTS.h>
#include "qps.h"

static qPS_Signal_t *sigTable = NULL;
static size_t sigTableEntries = 0;

/*============================================================================*/
qBool_t qPS_Init( qPS_Signal_t * table,
                  const size_t n )
{
    qBool_t retValue = qFalse;
    qUINT32_t i;
    qBool_t init_list;

    qDebug_Message ("<qPS_Init");
    qDebug_Decimal (n);

    if (sigTable == NULL) {
        sigTable = table;
        sigTableEntries = n;

        for (i = 0; i < sigTableEntries; i++) {
            init_list = qList_Initialize ((qList_t *)&(sigTable[i].subscribers));
            qDebug_Bool (init_list);
        }

        retValue = qTrue;
    }

    qDebug_Message ("qPS_Init>");

    return retValue;
}

/*============================================================================*/
qBool_t qPS_Sub( qSM_Signal_t sig,
                 qPS_Subscriber_t *sub )
{
    qBool_t retValue = qFalse;
    qUINT32_t i;
    size_t length;
    qBool_t insert;

    qDebug_Message ("qPS_Sub >>>");
    qDebug_Hexadecimal (sig);
    qDebug_Hexadecimal (sub);

    if (sigTable != NULL) {
        for (i = 0; i < sigTableEntries; i++) {
            if (sigTable[i].signal == sig) {
                insert = qList_Insert ((qList_t *)&(sigTable[i].subscribers), sub, QLIST_ATBACK);
                qDebug_Bool (insert);
                length = qList_Length ((qList_t *)&(sigTable[i].subscribers));
                qDebug_Decimal (length);
                retValue = qTrue;
                break;
            }
        }
    }
    qDebug_Message ("qPS_Sub <<<");

    return retValue;
}
/*============================================================================*/

 qBool_t pubForEachFcn( qList_ForEachHandle_t h )
 {
    qPS_Subscriber_t *subscriber;
    qSM_t *sm;
    qSM_Signal_t *signal;

    switch (h->stage) {
        case qList_WalkThrough:
            subscriber = (qPS_Subscriber_t *)h->node;
            sm = subscriber->sm;
            signal = (qSM_Signal_t *)h->arg;

            qDebug_Message("Sending signal >>>");
            qDebug_Hexadecimal (sm);
            qDebug_Hexadecimal (*signal);
            qDebug_Message("Sending signal <<<");

            qStateMachine_SendSignal (sm, *signal,  qFalse);
            break;
        case qList_WalkInit:
        case qList_WalkEnd:
        default:
            break;
    }

    return qFalse;
 }

qBool_t qPS_Pub( qSM_Signal_t sig )
{
    qBool_t retValue = qFalse;
    qUINT32_t i;

    qDebug_Message ("qPS_Pub >>>");
    qDebug_Hexadecimal (sig);

    if (sigTable != NULL) {
        for (i = 0; i < sigTableEntries; i++) {
            qDebug_Decimal (i);
            if (sigTable[i].signal == sig) {
                qDebug_Hexadecimal (sigTable[i].signal);
                qList_ForEach((qList_t *)&(sigTable[i].subscribers), pubForEachFcn, (void*)&(sigTable[i].signal), QLIST_FORWARD, NULL);
                retValue = qTrue;
                break;
            }
        }
    }

    qDebug_Message ("qPS_Pub <<<");

    return retValue;
}

/*============================================================================*/