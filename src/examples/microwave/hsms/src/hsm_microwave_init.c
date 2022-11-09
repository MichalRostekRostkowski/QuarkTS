#include <QuarkTS.h>
#include <hsm_microwave_callbacks.h>
#include <system_signals.h>

qTask_t microwave_task;
qSM_t microwave_state_machine;
static qQueue_t top_sigqueue;
static qSM_Signal_t top_sigqueue_stack [10];
static qSM_TimeoutSpec_t spectimeout;

static qSM_State_t microwave_door_closed_state;
static qSM_State_t   microwave_off_state;
static qSM_State_t   microwave_heating_state;
static qSM_State_t     microwave_toasting_state;
static qSM_State_t     microwave_baking_state;
static qSM_State_t microwave_door_opened_state;

//
// Transitions
//
qSM_Transition_t microwave_door_closed_state_transitions [] =
{
  { SYS_SIG_OPEN,   NULL, &microwave_door_opened_state,   0},
};

qSM_Transition_t microwave_off_state_transitions [] =
{
  { SYS_SIG_TOAST,  NULL, &microwave_toasting_state,    0},
  { SYS_SIG_BAKE,   NULL, &microwave_baking_state,      0}
};

qSM_Transition_t microwave_heating_state_transitions [] =
{
  { SYS_SIG_OFFTIMEOUT,  NULL, &microwave_off_state, 0},
};

qSM_Transition_t microwave_door_opened_state_transitions [] =
{
  { SYS_SIG_CLOSE,  NULL, &microwave_door_closed_state, 0}
};

//
// Timeouts
//
qSM_TimeoutStateDefinition_t microwave_heating_state_timeouts [] =
{
  { 3.0f, QSM_TSOPT_INDEX (1) | QSM_TSOPT_SET_ENTRY | QSM_TSOPT_RST_EXIT }
};

void hsm_microwave_init ()
{
  qStateMachine_Setup (&microwave_state_machine, NULL, &microwave_door_closed_state, NULL , NULL );

  qQueue_Setup (&top_sigqueue, top_sigqueue_stack, sizeof ( qSM_Signal_t ), qFLM_ArraySize ( top_sigqueue_stack ) );

  qStateMachine_InstallSignalQueue (&microwave_state_machine, &top_sigqueue );
  qStateMachine_InstallTimeoutSpec (&microwave_state_machine, &spectimeout);

  qStateMachine_StateSubscribe (&microwave_state_machine,     &microwave_door_closed_state,   QSM_STATE_TOP,                microwave_door_closed_state_callback,    &microwave_off_state, NULL);
    qStateMachine_StateSubscribe (&microwave_state_machine,   &microwave_off_state,           &microwave_door_closed_state, microwave_off_state_callback,             NULL, NULL);
    qStateMachine_StateSubscribe (&microwave_state_machine,   &microwave_heating_state,       &microwave_door_closed_state, microwave_heating_state_callback,         NULL, NULL);
      qStateMachine_StateSubscribe (&microwave_state_machine, &microwave_toasting_state,      &microwave_heating_state,     microwave_toasting_state_callback,        NULL, NULL);
      qStateMachine_StateSubscribe (&microwave_state_machine, &microwave_baking_state,        &microwave_heating_state,     microwave_baking_state_callback,          NULL, NULL);
  qStateMachine_StateSubscribe (&microwave_state_machine,     &microwave_door_opened_state,   QSM_STATE_TOP,                microwave_door_opened_state_callback,     NULL, NULL);

  qStateMachine_Set_StateTransitions (&microwave_door_closed_state, microwave_door_closed_state_transitions,  qFLM_ArraySize(microwave_door_closed_state_transitions));
  qStateMachine_Set_StateTransitions (&microwave_off_state,         microwave_off_state_transitions,          qFLM_ArraySize(microwave_off_state_transitions));
  qStateMachine_Set_StateTransitions (&microwave_heating_state,     microwave_heating_state_transitions,      qFLM_ArraySize(microwave_heating_state_transitions));
  qStateMachine_Set_StateTransitions (&microwave_door_opened_state, microwave_door_opened_state_transitions,  qFLM_ArraySize(microwave_door_opened_state_transitions));

  qStateMachine_Set_StateTimeouts (&microwave_toasting_state, microwave_heating_state_timeouts, qFLM_ArraySize (microwave_heating_state_timeouts));
  qStateMachine_Set_StateTimeouts (&microwave_baking_state,   microwave_heating_state_timeouts, qFLM_ArraySize (microwave_heating_state_timeouts));

  qOS_Add_StateMachineTask (&microwave_task, &microwave_state_machine, qLowest_Priority, 0.1f, qEnabled, NULL);
}