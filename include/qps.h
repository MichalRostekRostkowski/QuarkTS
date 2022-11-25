/*!
* @file qps.h
* @author Michal Rostkowski
* @version 1.0
* @note This file is part of the QuarkTS distribution.
* @brief Publish/Subscriber interface for QuarkTS
*/
#ifndef Q_PUBSUB_H
    #define Q_PUBSUB_H

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
    /**
    * @brief Initilize Publish/Subscribe module
    * @note  This function must be invoked before any usage of PS module
    * @return #qTrue on success, otherwise returns #qFalse.
    */
    qBool_t qPS_Init( void );

    /**
    * @brief Subscribe state machine to a particular signal
    * @param[in]  sig signal type to which subscriber wants to subscribe
    * @param[in]  sub subscribber that will be notified when signal occures
    * @return #qTrue on success, otherwise returns #qFalse.
    */
    qBool_t qPS_Sub( qSM_SignalType_t sig,
                     qSM_t *sub );

    /**
    * @brief Ubsubscribe state machine from a particular signal
    * @param[in]  sig signal type to which subscriber wants to subscribe
    * @param[in]  sub subscribber that will be notified when signal occures
    * @return #qTrue on success, otherwise returns #qFalse.
    */
    qBool_t qPS_UnSub( qSM_SignalType_t sig,
                       qSM_t *sub );

    /**
    * @brief Verify if state machine is subscribed to a particular signal
    * @param[in]  sig signal type to which subscriber wants to subscribe
    * @param[in]  sub subscribber that is to be verified
    * @return #qTrue on success, otherwise returns #qFalse.
    */
    qBool_t qPS_IsSub( qSM_SignalType_t sig,
                       qSM_t *sub );

    /**
    * @brief Publish signal to all subscribers
    * @param[in]  sig signal type to be published
    * @param[in]  data pointer to a buffer with data to be published
    * @param[in]  dataSize data size in bytes
    * @return #qTrue on success, otherwise returns #qFalse.
    */
    qBool_t qPS_Pub( qSM_SignalType_t sig,
                     void *data,
                     size_t dataSize );

    /** @}*/

    #ifdef __cplusplus
    }
    #endif

#endif // Q_PUBSUB_H

