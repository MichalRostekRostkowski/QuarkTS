/*!
* @file qps.h
* @author Michal Rostkowski
* @version 1.0
* @note This file is part of the QuarkTS distribution.
* @brief Publish/Subscriber interface for QuarkTS
*/
#ifndef QPUBSUB_H
    #define QPUBSUB_H

    #include "qtypes.h"
    #include "qlists.h"
    #include "qfsm.h"

    #ifdef __cplusplus
    extern "C" {
    #endif

    /** @addtogroup  qPS Publish/Subscribe interface for QuarkTS FSMs
     * @brief API interface PUB/SUB
     *  @{
     */

    typedef struct _qPS_Subscriber_s {
        qNode_MinimalFields;                    /**< Linked-list pointers. */
        qSM_t * const sm;
    } qPS_Subscriber_t;

    typedef struct _qPS_Signal_s {
        qNode_MinimalFields;                    /**< Linked-list pointers. */
        qSM_Signal_t signal;
        qPS_Subscriber_t subscribers;
    } qPS_Signal_t;

    /**
    * @brief Initilize Publish/Subscribe module
    * @param[in]  signal signal to which subscriber wants to subscribe
    * @param[in]  subscriber subscribber that will be notified when signal occures
    * @return #qTrue on success, otherwise returns #qFalse.
    */
    qBool_t qPS_Init( qPS_Signal_t * const table,
                      const size_t n );

    /**
    * @brief Subscribe state machine to particular signal
    * @param[in]  sig signal to which subscriber wants to subscribe
    * @param[in]  sub subscribber that will be notified when signal occures
    * @return #qTrue on success, otherwise returns #qFalse.
    */
    qBool_t qPS_Sub( qSM_Signal_t sig,
                     qPS_Subscriber_t *sub );

    /**
    * @brief Publish signal to all subscribers
    * @param[in]  sig signal to be published
    * @return #qTrue on success, otherwise returns #qFalse.
    */
    qBool_t qPS_Pub( qSM_Signal_t sig );


    void qPS_Test(void);

    /** @}*/

    #ifdef __cplusplus
    }
    #endif

#endif

