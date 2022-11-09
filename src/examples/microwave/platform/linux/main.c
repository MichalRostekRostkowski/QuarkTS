#include <stdio.h>
#include <unistd.h>
#include <stdbool.h>
#include <pthread.h>
#include <zmq.h>

#include <QuarkTS.h>
#include <system_signals.h>

#include <hsm_microwave_init.h>

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
void *msg_handler (void *p)
{
  void *ctx;
  void *sink;
  char buf[10];
  int i, n;

  ctx = zmq_ctx_new ();
  sink = zmq_socket (ctx, ZMQ_PULL);
  zmq_connect (sink, "tcp://localhost:5555");

  while (1) {
    n = zmq_recv (sink, buf, sizeof(buf), 0);
    if (n > 0) {
      if (strncmp(buf, "open", 4) == 0) {
        printf("open\n");
        qStateMachine_SendSignal (&microwave_state_machine, SYS_SIG_OPEN, qFalse);
      } else if (strncmp(buf, "close", 5) == 0) {
        printf("close\n");
        qStateMachine_SendSignal (&microwave_state_machine, SYS_SIG_CLOSE, qFalse);
      } else if (strncmp(buf, "toast", 4) == 0) {
        printf("toast\n");
        qStateMachine_SendSignal (&microwave_state_machine, SYS_SIG_TOAST, qFalse);
      } else if (strncmp(buf, "bake", 4) == 0) {
        printf("bake\n");
        qStateMachine_SendSignal (&microwave_state_machine, SYS_SIG_BAKE, qFalse);
      }
    }
  }

  zmq_close (sink);
  zmq_ctx_destroy (ctx);
}

void hsms_init ()
{
  hsm_microwave_init ();
}

int main()
{
  pthread_t tick_thread;
  pthread_t msg_thread;

  //
  // Set function for Debug and Trace messages
  //
  qTrace_Set_OutputFcn (put_char);

  //
  // Create thread with ticker for QuarkTS
  //
  pthread_create (&tick_thread, NULL, tick, NULL);

  //
  // Message handler thread
  //
  pthread_create (&msg_thread, NULL, msg_handler, NULL);

  qOS_Setup( NULL , TIMER_TICK , NULL );

  hsms_init ();

  qOS_Run ();

  pthread_join (tick_thread, NULL);

  return 0;
}