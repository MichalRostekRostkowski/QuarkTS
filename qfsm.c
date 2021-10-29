/*!
 * @file qfsm.c   
 * @author J. Camilo Gomez C.
 * @note This file is part of the QuarkTS distribution.
 **/

#include "qfsm.h"
#include <string.h>

#if ( Q_FSM == 1 )

typedef qByte_t qSM_LCA_t;

static void qStateMachine_Transition( qSM_t *m, 
                                      qSM_State_t *target, 
                                      const qSM_TransitionHistoryMode_t mHistory );
static qSM_LCA_t qStateMachine_LevelsToLeastCommonAncestor( qSM_t * const m, 
                                                            qSM_State_t * const target );
static void qStateMachine_ExitUpToLeastCommonAncestor( qSM_t * const m, 
                                                       qSM_LCA_t lca );
static void qStateMachine_PrepareHandler( qSM_UnprotectedHandler_t h, 
                                          const qSM_Signal_t xSignal, 
                                          qSM_State_t * const s );
static qSM_Status_t qStateMachine_InvokeStateCallback( qSM_t * const m, 
                                                       qSM_State_t * const s, 
                                                       qSM_UnprotectedHandler_t h );
static qSM_State_t* qStateMachine_StateOnExit( qSM_t * const m, 
                                               qSM_State_t * const s );
static void qStateMachine_StateOnEntry( qSM_t * const m, 
                                        qSM_State_t * const s );
static qSM_State_t* qStateMachine_StateOnStart( qSM_t * const m, 
                                                qSM_State_t * const s );
static qSM_Status_t qStateMachine_StateOnSignal( qSM_t * const m, 
                                                 qSM_State_t * const s, 
                                                 const qSM_Signal_t sig );
static void qStateMachine_TracePathandRetraceEntry( qSM_t * const m, 
                                                    qSM_State_t **entryPath );
static void qStateMachine_TraceOnStart( qSM_t * const m, 
                                        qSM_State_t **entryPath );
static qSM_Signal_t qStateMachine_CheckForSignals( qSM_t * const m, 
                                                    const qSM_Signal_t sig );
static void qStateMachine_SweepTransitionTable( qSM_State_t * const currentState, 
                                                qSM_UnprotectedHandler_t h );
static void qStateMachine_TimeoutCheckSignals( qSM_t * const m );
static void qStateMachine_TimeoutPerformSpecifiedActions( qSM_t * const m, 
                                                          qSM_State_t *current, 
                                                          const qSM_Signal_t sig );

#define QSM_TSOPT_INDEX_MASK    ( 0x00FFFFFFuL )

/*============================================================================*/
/*When a transition its performed, all exit actions must precede any actions
associated with the transition, which must precede any entry actions associated
with the newly entered state(s). To discover which exit actions to execute, it
is necessary to find the Least Common Ancestor (LCA) of the source and target
states. History mode is also handled here.
*/
static void qStateMachine_Transition( qSM_t *m, 
                                      qSM_State_t *target, 
                                      const qSM_TransitionHistoryMode_t mHistory )
{
    /*now, perform the exit actions by first finding the LCA*/
    qStateMachine_ExitUpToLeastCommonAncestor( m,  qStateMachine_LevelsToLeastCommonAncestor( m, target ) );
    /*then, handle the required history mode*/
    if ( qSM_TRANSITION_NO_HISTORY == mHistory ) {
        target->qPrivate.lastRunningChild = target->qPrivate.initState; /*just restore the default transition*/
    }
    else if ( qSM_TRANSITION_SHALLOW_HISTORY == mHistory ) {
        if ( NULL != target->qPrivate.lastRunningChild ) {
            /*restore the default transition in the last running nested state*/
            target->qPrivate.lastRunningChild->qPrivate.lastRunningChild = target->qPrivate.lastRunningChild->qPrivate.initState;
        }   
    }
    else { 
        /*
        nothing to do : qSM_TRANSITION_DEEP_HISTORY handled by default
        all deep history its preserved in the "lastRunningChild" attribute,
        so there is not need to change it. The qStateMachine_StateOnStart will 
        assign this by default.
        */
    }
    m->qPrivate.next = target; /*notify the state machine that there was indeed a transition*/
}
/*============================================================================*/
/*
To discover which exit actions to execute, it is necessary to find the LCA(Least
Common Ancestor) of the source and target states. So this function returns the
number of levels from the current state to the LCA.
*/
static qSM_LCA_t qStateMachine_LevelsToLeastCommonAncestor( qSM_t * const m, 
                                                            qSM_State_t * const target )
{
    qSM_LCA_t xLca = 0u;

    if ( m->qPrivate.source == target ) { /*recursive transition, only a level needs to be performed*/
        xLca = 1u;
    }
    else {
        qSM_State_t *s, *t;
        qBool_t xBreak = qFalse; /*just to be in compliance with the MISRAC2012-Rule-15.5*/
        qSM_LCA_t n = 0u;
        
        for ( s = m->qPrivate.source ; ( NULL != s ) && ( qFalse == xBreak ) ; s = s->qPrivate.parent ) {
            for ( t = target ; NULL != t ; t = t->qPrivate.parent ) {
                if ( s == t ) {
                    xLca = n;
                    xBreak = qTrue;
                    break;
                }
            }
            ++n;
        }        
    }
    
    return xLca;  
}
/*============================================================================*/
/*
This function its used to exit current states and all superstates up to LCA
*/
static void qStateMachine_ExitUpToLeastCommonAncestor( qSM_t * const m, 
                                                       qSM_LCA_t lca )
{
    qSM_State_t *s = m->qPrivate.current;

    while ( s != m->qPrivate.source ) {
        s = qStateMachine_StateOnExit( m, s );  
    }
    while ( 0u != lca-- ) {
        s = qStateMachine_StateOnExit( m, s ); 
    }
    m->qPrivate.current = s;    
}
/*============================================================================*/
/*
This function fill the qSM_Handler_t argument with the common data for all the
required actions.
*/
static void qStateMachine_PrepareHandler( qSM_UnprotectedHandler_t h, 
                                          const qSM_Signal_t sig, 
                                          qSM_State_t * const s )
{
    h->Signal = sig;
    h->NextState = NULL;
    h->StartState = NULL;
    h->Status = qSM_STATUS_NULL;    
    h->TransitionHistory = qSM_TRANSITION_NO_HISTORY;
    h->StateData = s->qPrivate.Data;
    h->state = s;
}
/*============================================================================*/
/*
This function invokes the state callback including the surrounding callback if available
*/
static qSM_Status_t qStateMachine_InvokeStateCallback( qSM_t *m, 
                                                       qSM_State_t * const s, 
                                                       qSM_UnprotectedHandler_t h )
{
    if ( NULL != m->qPrivate.surrounding ) {
        h->Status = qSM_STATUS_BEFORE_ANY;
        /*cstat -MISRAC2012-Rule-11.3 -CERT-EXP39-C_d*/
        m->qPrivate.surrounding( (qSM_Handler_t)h ); /*cast allowed, struct layout its compatible*/
        /*cstat +MISRAC2012-Rule-11.3 +CERT-EXP39-C_d*/
    }
    if ( NULL != s->qPrivate.sCallback ) {
        h->Status = qSM_STATUS_NULL;
        /*cstat -MISRAC2012-Rule-11.3 -CERT-EXP39-C_d*/
        h->Status = s->qPrivate.sCallback( (qSM_Handler_t)h ); /*execute the state callback (Transitions here are not allowed)*/ /*cast allowed, struct layout its compatible*/
        /*cstat +MISRAC2012-Rule-11.3 +CERT-EXP39-C_d*/
    }
    else {
        h->Status = qSM_STATUS_EXIT_FAILURE;
    }

    if ( NULL != m->qPrivate.surrounding ) { /*this property can change from the callback, so check it again*/
        if ( h->Status < qSM_STATUS_EXIT_FAILURE ) {
            h->Status = qSM_STATUS_EXIT_FAILURE;
        }
        /*cstat -MISRAC2012-Rule-11.3 -CERT-EXP39-C_d*/
        m->qPrivate.surrounding( (qSM_Handler_t)h ); /*cast allowed, struct layout compatible*/
        /*cstat +MISRAC2012-Rule-11.3 +CERT-EXP39-C_d*/
    }

    return h->Status;
}
/*============================================================================*/
static qSM_State_t* qStateMachine_StateOnExit( qSM_t * const m, 
                                               qSM_State_t * const s )
{ 
    qSM_UnprotectedHandler_t h = &m->qPrivate.handler;
    
    qStateMachine_PrepareHandler( h, QSM_SIGNAL_EXIT, s );
    (void)qStateMachine_InvokeStateCallback( m, s, h );
    
    if ( ( NULL != m->qPrivate.TimeSpec ) && ( NULL != s->qPrivate.tdef ) ) {
        qStateMachine_TimeoutPerformSpecifiedActions( m, s , QSM_SIGNAL_EXIT );
    }
    s->qPrivate.parent->qPrivate.lastRunningChild  = s; 

    return s->qPrivate.parent;
}
/*============================================================================*/
static void qStateMachine_StateOnEntry( qSM_t * const m, 
                                        qSM_State_t * const s )
{ 
    qSM_UnprotectedHandler_t h = &m->qPrivate.handler;
    
    qStateMachine_PrepareHandler( h, QSM_SIGNAL_ENTRY, s );
    (void)qStateMachine_InvokeStateCallback( m, s, h );
    if ( ( NULL != m->qPrivate.TimeSpec ) && ( NULL != s->qPrivate.tdef ) ) {
        qStateMachine_TimeoutPerformSpecifiedActions( m, s , QSM_SIGNAL_ENTRY );
    }
}
/*============================================================================*/
static qSM_State_t* qStateMachine_StateOnStart( qSM_t * const m, 
                                                qSM_State_t * const s )
{
    qSM_UnprotectedHandler_t h = &m->qPrivate.handler;

    qStateMachine_PrepareHandler( h, QSM_SIGNAL_START, s );
    (void)qStateMachine_InvokeStateCallback( m, s, h );
    
    if ( NULL != h->StartState ) { /*changes from callback takes more precedence*/
        /*cstat -MISRAC2012-Rule-11.5 -CERT-EXP36-C_b*/
        m->qPrivate.next = (qSM_State_t*)h->StartState;
        /*cstat +MISRAC2012-Rule-11.5 +CERT-EXP36-C_b*/
    }
    else {
        if ( NULL != s->qPrivate.lastRunningChild ) {
            m->qPrivate.next = s->qPrivate.lastRunningChild;  /*try to preserve history*/
        }
    }

    return m->qPrivate.next;
}
/*============================================================================*/
static qSM_Status_t qStateMachine_StateOnSignal( qSM_t * const m, 
                                                 qSM_State_t * const s, 
                                                 const qSM_Signal_t sig )
{
    qSM_UnprotectedHandler_t h = &m->qPrivate.handler;
    qSM_Status_t status;
    
    qStateMachine_PrepareHandler( h, sig, s );
    if ( NULL != s->qPrivate.tTable ) {
        qStateMachine_SweepTransitionTable( s, h ); /*evaluate the transition table if available*/
    }
    status = qStateMachine_InvokeStateCallback( m, s, h );
 
    if ( NULL != h->NextState ) { /*perform the transition if available*/
        /*cstat -MISRAC2012-Rule-11.5 -CERT-EXP36-C_b*/
        qStateMachine_Transition( m, (qSM_State_t*)h->NextState, h->TransitionHistory );
        /*cstat +MISRAC2012-Rule-11.5 +CERT-EXP36-C_b*/
        status = qSM_STATUS_SIGNAL_HANDLED; /*the signal is assumed to be handled if the transition occurs*/
    }

    return status;
}
/*============================================================================*/
/*
Entry actions must be executed in order form the least deeply nested to the 
most deeply nested state. This is opposite to the normal navigability of the 
module data-structure design. So this is solved by first recording the entry
path from the LCA to the target, then playing it  backwards. with execution
of entry actions. qStateMachine_TracePathandRetraceEntry and qStateMachine_TraceOnStart
are used to handle this after the transition perform all the required exit actions.
*/
static void qStateMachine_TracePathandRetraceEntry( qSM_t * const m, 
                                                    qSM_State_t **trace )
{
    qSM_State_t *s;

    *trace = NULL;
    for ( s = m->qPrivate.next ; s != m->qPrivate.current ; s = s->qPrivate.parent ) {
        *(++trace) = s;  /* trace path to target */
    }
    while ( NULL != ( s = *trace-- ) ) {  /* retrace entry from LCA */
        qStateMachine_StateOnEntry( m, s );
    }
    m->qPrivate.current = m->qPrivate.next;
    m->qPrivate.next = NULL;    
}
/*============================================================================*/
static void qStateMachine_TraceOnStart( qSM_t * const m, 
                                        qSM_State_t **entryPath )
{
    while ( NULL != qStateMachine_StateOnStart( m, m->qPrivate.current ) ) {
        qStateMachine_TracePathandRetraceEntry( m, entryPath );
    }
}
/*============================================================================*/
/*
This function checks for available signals. It initially checks for time signals 
if the state machine has a timeout-specification object installed. If any timeout expires, 
the signal is put on the available recipient.
If the state machine has the signal-queue installed, the signal that is in front of the 
queue is obtained. In case of there is no signal-queue or the queue its empty, 
the input argument and the exclusion variable are verified.
Rules: 
- A signal coming from the signal-queue has the higher precedence.
- A valid input argument overides the exclusion variable.
*/
static qSM_Signal_t qStateMachine_CheckForSignals( qSM_t * const m, 
                                                   const qSM_Signal_t sig )
{
    qSM_Signal_t xSignal = sig;
    
    if ( NULL != m->qPrivate.TimeSpec ) {
        qStateMachine_TimeoutCheckSignals( m ); /*use the available recipient: signal-queue or the EV*/
    }
    #if ( Q_QUEUES == 1 )
        if ( NULL != m->qPrivate.queue ) {
            qSM_Signal_t ReceivedSignal;

            if ( qTrue == qQueue_Receive( m->qPrivate.queue, &ReceivedSignal ) ) {
                xSignal = ReceivedSignal; 
            }
        }
    #endif
    if ( ( QSM_SIGNAL_NONE == xSignal ) && ( QSM_SIGNAL_NONE != m->qPrivate.SignalNot ) ) { 
        xSignal = m->qPrivate.SignalNot;
        m->qPrivate.SignalNot = QSM_SIGNAL_NONE; 
    }

    return xSignal;
}
/*============================================================================*/
qBool_t qStateMachine_Run( qSM_t * const m, 
                           qSM_Signal_t sig )
{
    qBool_t retValue = qFalse;

    if ( NULL != m ) {
        qSM_State_t *entryPath[ Q_FSM_MAX_NEST_DEPTH ];
        qSM_State_t *s;
      
        sig = qStateMachine_CheckForSignals( m, sig );

        /*Enter here only once to start the top state*/
        if ( NULL == m->qPrivate.current ) { 
            m->qPrivate.current = &m->qPrivate.top;
            m->qPrivate.next = NULL;
            qStateMachine_StateOnEntry( m, m->qPrivate.current );
            qStateMachine_TraceOnStart( m, entryPath );
        }
        /*evaluate the hierarchy until the signal is handled*/
        for ( s = m->qPrivate.current ; NULL != s ; s = s->qPrivate.parent ) {
            m->qPrivate.source = s; /* level of outermost event handler */
            if ( qSM_STATUS_SIGNAL_HANDLED == qStateMachine_StateOnSignal( m, s, sig ) ) {
                if ( NULL != m->qPrivate.next ) {  /* state transition taken? */
                    /*execute entry/start actions in the rest of the hierarchy after transition*/
                    qStateMachine_TracePathandRetraceEntry( m, entryPath );
                    qStateMachine_TraceOnStart( m, entryPath );
                }
                retValue = qTrue;
                break;/* signal processed */
            }
        }   
    }

    return retValue;
}
/*============================================================================*/
qBool_t qStateMachine_Setup( qSM_t * const m, 
                             qSM_StateCallback_t topFcn, 
                             qSM_State_t * const init, 
                             qSM_SurroundingCallback_t sFcn, 
                             void *pData )
{
    qBool_t retValue = qFalse;

    if ( ( NULL != m ) ) {
        (void)memset( m, 0, sizeof( qSM_t ) );
        m->qPrivate.current = NULL;
        m->qPrivate.next = NULL;
        m->qPrivate.source = NULL;
        m->qPrivate.Data = pData;
        m->qPrivate.surrounding = sFcn;
        m->qPrivate.handler.machine = m;
        m->qPrivate.handler.Data = pData;
        #if ( Q_QUEUES == 1 )
            m->qPrivate.queue = NULL;
        #endif
        m->qPrivate.TimeSpec = NULL;
        /*subscribe the built-in top state*/
        retValue = qStateMachine_StateSubscribe( m, &m->qPrivate.top, NULL, topFcn, init, NULL );
        m->qPrivate.top.qPrivate.parent = NULL;
    }

    return retValue;
}
/*============================================================================*/
qBool_t qStateMachine_StateSubscribe( qSM_t * const m, 
                                      qSM_State_t * const s, 
                                      qSM_State_t * const parent, 
                                      qSM_StateCallback_t sFcn, 
                                      qSM_State_t * const init, 
                                      void *pData )
{
    qBool_t retValue = qFalse;

    if ( NULL != s ) {
        (void)memset( s, 0, sizeof(qSM_State_t) );
        s->qPrivate.Data = pData;
        s->qPrivate.lastRunningChild = init;
        s->qPrivate.initState = init;
        s->qPrivate.sCallback = sFcn;
        s->qPrivate.parent = ( NULL == parent )? &m->qPrivate.top : parent ;
        s->qPrivate.tTable = NULL;
        s->qPrivate.tEntries = (size_t)0u;
        s->qPrivate.tdef = NULL;
        s->qPrivate.nTm = (size_t)0u;
        retValue = qTrue;
    }

    return retValue;
}
/*============================================================================*/
qBool_t qStateMachine_Set_StateTransitions( qSM_State_t * const s, 
                                            qSM_Transition_t * const table, 
                                            const size_t n )
{
    qBool_t retValue = qFalse;

    if ( ( NULL != s ) && ( NULL != table ) && ( n > 0u ) ) {
        s->qPrivate.tTable = table;
        s->qPrivate.tEntries = n;
        retValue = qTrue;
    }

    return retValue;
}
/*============================================================================*/
qBool_t qStateMachine_InstallSignalQueue( qSM_t * const m, 
                                          qQueue_t *q )
{
    qBool_t retValue = qFalse;
    #if ( Q_QUEUES == 1 )
        if ( ( NULL != m ) && ( NULL != q ) ) {
            if ( ( qTrue == qQueue_IsReady( q ) ) && ( sizeof(qSM_Signal_t) == q->qPrivate.ItemSize ) ) {
                m->qPrivate.queue = q; /*install the queue*/
                retValue = qTrue;
            }
        }
    #else
        Q_UNUSED( m );
        Q_UNUSED( queue );
    #endif   
     
    return retValue;
}
/*============================================================================*/
static void qStateMachine_SweepTransitionTable( qSM_State_t * const currentState, 
                                                qSM_UnprotectedHandler_t h )
{
    size_t i, n;
    qBool_t transitionAllowed;
    qSM_Transition_t *iTransition;
    /*cstat -MISRAC2012-Rule-11.5 -CERT-EXP36-C_b*/
    qSM_Transition_t *table = (qSM_Transition_t *)currentState->qPrivate.tTable;
    /*cstat +MISRAC2012-Rule-11.5 +CERT-EXP36-C_b*/
    n = currentState->qPrivate.tEntries;
    for ( i = 0u ; i < n ; ++i ) {
        iTransition = &table[ i ];  /*get the i-element from the table*/
        transitionAllowed = qTrue; /*if no signal-guard available, allow the transition by default*/
        if ( h->Signal == iTransition->xSignal ) { /*table entry match*/
            if ( NULL != iTransition->Guard ) { /*if signal-guard available, just run the guard function*/
                /*cstat -MISRAC2012-Rule-11.3 -CERT-EXP39-C_d*/
                transitionAllowed = iTransition->Guard( (qSM_Handler_t)h ); /*cast allowed, struct layout compatible*/
                /*cstat +MISRAC2012-Rule-11.3 +CERT-EXP39-C_d*/
            }
            if ( qTrue == transitionAllowed ) { 
                if ( NULL != iTransition->NextState ) { /*target state available?*/
                    h->NextState = iTransition->NextState; /*set the transition from the table*/    
                    h->TransitionHistory = iTransition->HistoryMode; /*set the history mode form the table*/
                    break;
                }
            }
        }
    }
}
/*============================================================================*/
qBool_t qStateMachine_SendSignal( qSM_t * const m, 
                                  qSM_Signal_t sig, 
                                  const qBool_t isUrgent )
{
    qBool_t retValue = qFalse;

    if ( ( NULL != m ) && ( sig < QSM_SIGNAL_NONE ) ) {
        #if ( Q_QUEUES == 1 )
            if ( NULL != m->qPrivate.queue ) { /*check if the state-machine has a signal queue*/
                retValue = qQueue_SendGeneric( m->qPrivate.queue, &sig, (qQueue_Mode_t)isUrgent );  
            }
        #else
            Q_UNUSED( isUrgent );
        #endif
        
        if ( ( qFalse == retValue ) && ( QSM_SIGNAL_NONE == m->qPrivate.SignalNot ) ) {
            m->qPrivate.SignalNot = sig;
            retValue = qTrue;
        }
    }

    return retValue;
}
/*============================================================================*/
static void qStateMachine_TimeoutCheckSignals( qSM_t * const m )
{
    qSM_TimeoutSpec_t *ts = m->qPrivate.TimeSpec;
    size_t i;

    for ( i = 0u ; i < (size_t)Q_FSM_MAX_TIMEOUTS ; ++i ) {
        if ( qTrue == qSTimer_Expired( &ts->builtin_timeout[ i ] ) ) {
            #if ( Q_QUEUES == 1 )
                (void)qStateMachine_SendSignal( m, QSM_SIGNAL_TIMEOUT( i ), qFalse );   
            #endif
            if ( 0uL  != ( ts->isPeriodic & ( 1uL <<  i ) ) ) {
                (void)qSTimer_Reload( &ts->builtin_timeout[ i ] );
            }
            else {
                (void)qSTimer_Disarm( &ts->builtin_timeout[ i ] );
            }
        } 
    }
}
/*============================================================================*/
static void qStateMachine_TimeoutPerformSpecifiedActions( qSM_t * const m, 
                                                          qSM_State_t *current, 
                                                          const qSM_Signal_t sig )
{
    qSM_TimeoutStateDefinition_t *tbl = current->qPrivate.tdef;
    size_t n = current->qPrivate.nTm;    

    if ( ( n > 0u ) && ( NULL != tbl ) ) {
        size_t i;
        qSM_TimeoutSpecOptions_t setCheck, resetCheck;

        if ( QSM_SIGNAL_ENTRY == sig ) {
            setCheck = QSM_TSOPT_SET_ENTRY; 
            resetCheck = QSM_TSOPT_RST_ENTRY;
        }
        else {
            setCheck = QSM_TSOPT_SET_EXIT; 
            resetCheck = QSM_TSOPT_RST_EXIT;
        }
        for ( i = 0u ; i < n ; ++i ) { /*loop table */
            qSM_TimeoutSpecOptions_t opt = tbl[ i ].options;
            qIndex_t index = opt & QSM_TSOPT_INDEX_MASK; /*get the timeout index*/
            
            if ( index < (qIndex_t)Q_FSM_MAX_TIMEOUTS ) { /*state match and index is valid?*/
                qSTimer_t *tmr = &m->qPrivate.TimeSpec->builtin_timeout[ index ]; /*get the timeout handle*/
                qTime_t tValue = tbl[ i ].xTimeout;
                
                if ( 0uL != ( opt & setCheck ) ) {
                    if ( 0uL == ( opt & QSM_TSOPT_KEEP_IF_SET ) ) {
                        (void)qSTimer_Set( tmr, tValue  );        
                    }
                    if ( 0uL != ( opt & QSM_TSOPT_PERIODIC ) ) {
                        m->qPrivate.TimeSpec->isPeriodic |= (qUINT32_t)( ( 1uL << index ) );         
                    }
                    else {
                        m->qPrivate.TimeSpec->isPeriodic &= (qUINT32_t)(~( 1uL << index ) );      
                    }
                }   
                if ( 0uL != ( opt & resetCheck ) ) {
                    (void)qSTimer_Disarm( tmr );
                }     
            }
        }
    }
}
/*============================================================================*/
qBool_t qStateMachine_InstallTimeoutSpec( qSM_t * const m, 
                                          qSM_TimeoutSpec_t * const ts )
{
    qBool_t retValue = qFalse;

    if ( ( NULL != m ) && ( NULL != ts ) ) {
        size_t i;

        (void)memset( ts, 0, sizeof(qSM_TimeoutSpec_t) );
        m->qPrivate.TimeSpec = ts;
        for ( i = 0u ; i < (size_t)Q_FSM_MAX_TIMEOUTS ; ++i ) {
            (void)qSTimer_Disarm( &m->qPrivate.TimeSpec->builtin_timeout[ i ] );
        }
        retValue = qTrue;
    }

    return retValue;
}
/*============================================================================*/
qBool_t qStateMachine_Set_StateTimeouts( qSM_State_t * const s, 
                                         qSM_TimeoutStateDefinition_t *tdef, 
                                         const size_t n )
{
    qBool_t retValue = qFalse;

    if ( ( NULL != s ) && ( NULL != tdef ) && ( n > (size_t)0uL ) ) {
        s->qPrivate.tdef = tdef;
        s->qPrivate.nTm= n;
        retValue = qTrue;
    }

    return retValue;
}
/*============================================================================*/
qBool_t qStateMachine_TimeoutSet( qSM_t * const m, 
                                  const qIndex_t xTimeout, 
                                  const qTime_t t )
{
    qBool_t retValue = qFalse;

    if ( ( NULL != m ) && ( xTimeout <= (qIndex_t)Q_FSM_MAX_TIMEOUTS ) ) {
        if ( NULL != m->qPrivate.TimeSpec ) {
            retValue = qSTimer_Set( &m->qPrivate.TimeSpec->builtin_timeout[ xTimeout ], t  );
        }
    }

    return retValue;
}
/*============================================================================*/
qBool_t qStateMachine_TimeoutStop( qSM_t * const m, 
                                   const qIndex_t xTimeout )
{
    qBool_t retValue = qFalse;

    if ( ( NULL != m->qPrivate.TimeSpec ) && ( xTimeout < (qIndex_t)Q_FSM_MAX_TIMEOUTS ) ) {
        #if ( Q_QUEUES == 1 )
            size_t cnt;
            qSM_Signal_t xSignal;
            
            cnt = qQueue_Count( m->qPrivate.queue );
            while ( 0u != cnt-- ) {
                if ( qTrue == qQueue_Receive( m->qPrivate.queue , &xSignal ) ) {
                    if ( xSignal != QSM_SIGNAL_TIMEOUT( xTimeout ) ) { /*keep the non-timeout signals*/
                        (void)qQueue_SendToBack( m->qPrivate.queue , &xSignal );
                    }
                }
            }           
        #endif
        retValue = qSTimer_Disarm( &m->qPrivate.TimeSpec->builtin_timeout[ xTimeout ] );
    }    

    return retValue;
}
/*============================================================================*/
void* qStateMachine_Get_Machine( qSM_t * const m, 
                                 const qSM_Attribute_t a )
{
    void *xAttribute = NULL;

    if ( NULL != m ) {
        switch ( a ) {
            case qSM_ATTRIB_STATE_TOP:
                xAttribute = &m->qPrivate.top;
                break;  
            case qSM_ATTRIB_STATE_CURRENT:
                xAttribute = m->qPrivate.current;
                break;           
            case qSM_ATTRIB_SIGNAL_QUEUE:
                xAttribute = m->qPrivate.queue;
                break;         
            case qSM_ATTRIB_TIMESPEC:
                xAttribute = m->qPrivate.TimeSpec;
                break;       
            case qSM_ATTRIB_DATA:
                xAttribute = m->qPrivate.Data;
                break;                
            default:
                break;
        }
    }

    return xAttribute;
}
/*============================================================================*/
void* qStateMachine_Get_State( qSM_State_t * const s, 
                               const qSM_Attribute_t a )
{
    void *xAttribute = NULL;

    if ( NULL != s ) {
        switch ( a ) {
            case qSM_ATTRIB_COMPOSITE_INITSTATE:
                xAttribute = s->qPrivate.initState;    
                break;
            case qSM_ATTRIB_COMPOSITE_LASTSTATE:   
                xAttribute = s->qPrivate.lastRunningChild;        
                break;
            case qSM_ATTRIB_COMPOSITE_PARENT:    
                xAttribute = s->qPrivate.parent;              
                break;     
            case qSM_ATTRIB_DATA:
                xAttribute = s->qPrivate.Data;
                break;        
            case qSM_ATTRIB_TRANSITION_TABLE:
                xAttribute = s->qPrivate.tTable;
                break;                          
            default:
                break;
        }
    }

    return xAttribute;
}
/*============================================================================*/
qBool_t qStateMachine_Set_StateCallback( qSM_State_t * const state, 
                                         qSM_StateCallback_t sFcn )
{
    qBool_t retValue = qFalse;

    if ( NULL != state ) {
        state->qPrivate.sCallback = sFcn;
        retValue = qTrue;
    }

    return retValue;
}
/*============================================================================*/
qBool_t qStateMachine_Set_MachineSurrounding( qSM_t * const m, 
                                              qSM_SurroundingCallback_t sFcn )
{
    qBool_t retValue = qFalse;

    if ( NULL != m ) {
        m->qPrivate.surrounding = sFcn;
        retValue = qTrue;
    }

    return retValue;
}
/*============================================================================*/

#endif /*#if ( Q_FSM == 1 )*/