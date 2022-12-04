#include <stdio.h>
#include <unistd.h>
#include <stdbool.h>
#include <pthread.h>
#include <signal.h>

#include <QuarkTS.h>
#include <system_signals.h>

#include <hsm_basic.h>

#define TIMER_TICK  0.001
#define TIMER_TICK_USLEEP (__useconds_t)(TIMER_TICK * 1000000)

//
// QuarkTS tick callback
//
void *tick(void *p)
{
  (void)p;

  printf("Timer tick: %f s\n", TIMER_TICK);

  while (true) {
    qClock_SysTick ();
    usleep (TIMER_TICK_USLEEP);
  }
}

//
// Debug messages function
//
void put_char( void *sp , const char c )
{
  (void)sp;

  putchar(c);
}

//
// Signal handlers
//
void sig_handler (int sig)
{
  if ( sig == SIGUSR1 ) {
    qStateMachine_SendSignal (&basic_state_machine, SYS_SIG_ON, NULL, 0, qFalse);
  } else if ( sig == SIGUSR2 ) {
    qStateMachine_SendSignal (&basic_state_machine, SYS_SIG_OFF, NULL, 0, qFalse);
  }
}

void hsms_init ()
{
  hsm_basic_init ();
}

int main()
{
  pthread_t tick_thread;

  //
  // Set function for Debug and Trace messages
  //
  qTrace_Set_OutputFcn (put_char);

  //
  // Create thread with ticker for QuarkTS
  //
  pthread_create (&tick_thread, NULL, tick, NULL);

  //
  // Set system signal handlers
  // SIGUSR1 - SYS_SIG_ON
  // SIGUSR2 - SYS_SIG_OFF
  //
  signal (SIGUSR1, sig_handler);
  signal (SIGUSR2, sig_handler);

  qOS_Setup( NULL , TIMER_TICK , NULL );

  hsms_init ();

  qOS_Run ();

  pthread_join (tick_thread, NULL);

  return 0;
}