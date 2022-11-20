#include <QuarkTS.h>
#include <system_signals.h>

qTask_t basic_task;
qSM_t basic_state_machine;
static qQueue_t top_sigqueue;
static qSM_Signal_t top_sigqueue_stack [10];

static qSM_State_t basic_on_state;
static qSM_State_t basic_off_state;

//
// Transitions
//
qSM_Transition_t basic_on_state_transitions [] =
{
  { SYS_SIG_OFF,  NULL, &basic_off_state, 0}
};

qSM_Transition_t basic_off_state_transitions [] =
{
  { SYS_SIG_ON, NULL, &basic_on_state, 0}
};

//
// Callbacks
//
qSM_Status_t basic_top_state_callback (qSM_Handler_t h)
{
  qSM_Status_t RetVal = qSM_STATUS_EXIT_SUCCESS;

  switch (h->Signal.type) {
    case QSM_SIGNAL_TYPE_ENTRY :
      qDebug_Message("TOP|E");
      break;
    case QSM_SIGNAL_TYPE_EXIT :
      qDebug_Message("TOP|X");
      break;
    case QSM_SIGNAL_TYPE_START :
      qDebug_Message("TOP|S");
      break;
  }

  return RetVal;
}

qSM_Status_t basic_on_state_callback (qSM_Handler_t h)
{
  qSM_Status_t RetVal = qSM_STATUS_EXIT_SUCCESS;

  switch (h->Signal.type) {
    case QSM_SIGNAL_TYPE_ENTRY :
      qDebug_Message("ON|E");
      break;
    case QSM_SIGNAL_TYPE_EXIT :
      qDebug_Message("ON|X");
      break;
    case QSM_SIGNAL_TYPE_START :
      qDebug_Message("ON|S");
      break;
  }

  return RetVal;
}

qSM_Status_t basic_off_state_callback (qSM_Handler_t h)
{
  qSM_Status_t RetVal = qSM_STATUS_EXIT_SUCCESS;

  switch (h->Signal.type) {
    case QSM_SIGNAL_TYPE_ENTRY :
      qDebug_Message("OFF|E");
      break;
    case QSM_SIGNAL_TYPE_EXIT :
      qDebug_Message("OFF|X");
      break;
    case QSM_SIGNAL_TYPE_START :
      qDebug_Message("OFF|S");
      break;
  }

  return RetVal;
}

void hsm_basic_init ()
{
  qStateMachine_Setup (&basic_state_machine, NULL, &basic_on_state, NULL , NULL );

  qQueue_Setup (&top_sigqueue, top_sigqueue_stack, sizeof ( qSM_Signal_t ), qFLM_ArraySize ( top_sigqueue_stack ) );

  qStateMachine_InstallSignalQueue (&basic_state_machine, &top_sigqueue );

  qStateMachine_StateSubscribe (&basic_state_machine, &basic_on_state,       QSM_STATE_TOP,   basic_on_state_callback,    NULL, NULL);  
  qStateMachine_StateSubscribe (&basic_state_machine, &basic_off_state,      QSM_STATE_TOP,   basic_off_state_callback,   NULL, NULL);  

  qStateMachine_Set_StateTransitions (&basic_on_state, basic_on_state_transitions, qFLM_ArraySize(basic_on_state_transitions));
  qStateMachine_Set_StateTransitions (&basic_off_state, basic_off_state_transitions, qFLM_ArraySize(basic_off_state_transitions));
 
  qOS_Add_StateMachineTask (&basic_task, &basic_state_machine, qLowest_Priority, 0.1f, qEnabled, NULL);
}