#include <QuarkTS.h>
#include <hsm_pse_callbacks.h>
#include <system_signals.h>

#include <stdio.h>

qTask_t pse_sa_task;
qTask_t pse_sb_task;

qSM_t pse_sa_state_machine;
qSM_t pse_sb_state_machine;

static qQueue_t pse_sa_sigqueue;
static qQueue_t pse_sb_sigqueue;
static qSM_Signal_t pse_sa_top_sigqueue_stack [10];
static qSM_Signal_t pse_sb_top_sigqueue_stack [10];

static qSM_State_t pse_sa_state;
static qSM_State_t  pse_sa1_state;
static qSM_State_t    pse_sa11_state;
static qSM_State_t    pse_sa12_state;
static qSM_State_t  pse_sa2_state;
static qSM_State_t    pse_sa21_state;
static qSM_State_t    pse_sa22_state;
static qSM_State_t pse_sb_state;
static qSM_State_t  pse_sb1_state;
static qSM_State_t    pse_sb11_state;
static qSM_State_t    pse_sb12_state;
static qSM_State_t  pse_sb2_state;
static qSM_State_t    pse_sb21_state;
static qSM_State_t    pse_sb22_state;

//
// Transitions
//
qSM_Transition_t pse_sa1_state_transitions [] =
{
  { SYS_SIG_TYPE_1, NULL, &pse_sa2_state, 0 },
};

qSM_Transition_t pse_sa2_state_transitions [] =
{
  { SYS_SIG_TYPE_2, NULL, &pse_sa1_state, 0 },
};

qSM_Transition_t pse_sb1_state_transitions [] =
{
  { SYS_SIG_TYPE_1, NULL, &pse_sb2_state, 0 },
};

qSM_Transition_t pse_sb2_state_transitions [] =
{
  { SYS_SIG_TYPE_2, NULL, &pse_sb1_state, 0 },
};

//
// Timeouts
//
qSM_TimeoutStateDefinition_t pse_heating_state_timeouts [] =
{
  { 3.0f, QSM_TSOPT_INDEX (1) | QSM_TSOPT_SET_ENTRY | QSM_TSOPT_RST_EXIT }
};

void hsm_pse_init ()
{
  //
  // SA
  //
  qStateMachine_Setup (&pse_sa_state_machine, NULL, &pse_sa_state, NULL , NULL );

  qQueue_Setup (&pse_sa_sigqueue, pse_sa_top_sigqueue_stack, sizeof ( qSM_Signal_t ), qFLM_ArraySize ( pse_sa_top_sigqueue_stack ) );

  qStateMachine_InstallSignalQueue (&pse_sa_state_machine, &pse_sa_sigqueue );

  qStateMachine_StateSubscribe (&pse_sa_state_machine,     &pse_sa_state,   QSM_STATE_TOP,                pse_sa_state_callback,    &pse_sa1_state, NULL);
    qStateMachine_StateSubscribe (&pse_sa_state_machine,     &pse_sa1_state,  &pse_sa_state,                pse_sa1_state_callback,   &pse_sa11_state, NULL);
      qStateMachine_StateSubscribe (&pse_sa_state_machine,     &pse_sa11_state,  &pse_sa1_state,                pse_sa11_state_callback,   NULL, NULL);
      qStateMachine_StateSubscribe (&pse_sa_state_machine,     &pse_sa12_state,  &pse_sa1_state,                pse_sa12_state_callback,   NULL, NULL);
    qStateMachine_StateSubscribe (&pse_sa_state_machine,     &pse_sa2_state,  &pse_sa_state,                pse_sa2_state_callback,   &pse_sa22_state, NULL);
      qStateMachine_StateSubscribe (&pse_sa_state_machine,     &pse_sa21_state,  &pse_sa2_state,                pse_sa21_state_callback,   NULL, NULL);
      qStateMachine_StateSubscribe (&pse_sa_state_machine,     &pse_sa22_state,  &pse_sa2_state,                pse_sa22_state_callback,   NULL, NULL);

  qStateMachine_Set_StateTransitions (&pse_sa1_state, pse_sa1_state_transitions, qFLM_ArraySize(pse_sa1_state_transitions));
  qStateMachine_Set_StateTransitions (&pse_sa2_state, pse_sa2_state_transitions, qFLM_ArraySize(pse_sa2_state_transitions));

  qStateMachine_SubscribeToSignal (&pse_sa_state_machine, SYS_SIG_TYPE_1);

  qOS_Add_StateMachineTask (&pse_sa_task, &pse_sa_state_machine, qLowest_Priority, 0.1f, qEnabled, NULL);

  //
  // SB
  //
  qStateMachine_Setup (&pse_sb_state_machine, NULL, &pse_sb_state, NULL , NULL );

  qQueue_Setup (&pse_sb_sigqueue, pse_sb_top_sigqueue_stack, sizeof ( qSM_Signal_t ), qFLM_ArraySize ( pse_sb_top_sigqueue_stack ) );

  qStateMachine_InstallSignalQueue (&pse_sb_state_machine, &pse_sb_sigqueue );

  qStateMachine_StateSubscribe (&pse_sb_state_machine,     &pse_sb_state,   QSM_STATE_TOP,                pse_sb_state_callback,    &pse_sb1_state, NULL);
    qStateMachine_StateSubscribe (&pse_sb_state_machine,     &pse_sb1_state,  &pse_sb_state,                pse_sb1_state_callback,   &pse_sb11_state, NULL);
      qStateMachine_StateSubscribe (&pse_sb_state_machine,     &pse_sb11_state,  &pse_sb1_state,                pse_sb11_state_callback,   NULL, NULL);
      qStateMachine_StateSubscribe (&pse_sb_state_machine,     &pse_sb12_state,  &pse_sb1_state,                pse_sb12_state_callback,   NULL, NULL);
    qStateMachine_StateSubscribe (&pse_sb_state_machine,     &pse_sb2_state,  &pse_sb_state,                pse_sb2_state_callback,   &pse_sb22_state, NULL);
      qStateMachine_StateSubscribe (&pse_sb_state_machine,     &pse_sb21_state,  &pse_sb2_state,                pse_sb21_state_callback,   NULL, NULL);
      qStateMachine_StateSubscribe (&pse_sb_state_machine,     &pse_sb22_state,  &pse_sb2_state,                pse_sb22_state_callback,   NULL, NULL);

  qStateMachine_Set_StateTransitions (&pse_sb1_state, pse_sb1_state_transitions, qFLM_ArraySize(pse_sb1_state_transitions));
  qStateMachine_Set_StateTransitions (&pse_sb2_state, pse_sb2_state_transitions, qFLM_ArraySize(pse_sb2_state_transitions));

  qOS_Add_StateMachineTask (&pse_sb_task, &pse_sb_state_machine, qLowest_Priority, 0.1f, qEnabled, NULL);
}