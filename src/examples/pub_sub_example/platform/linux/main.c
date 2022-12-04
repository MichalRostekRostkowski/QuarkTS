#include <stdio.h>
#include <unistd.h>
#include <stdbool.h>
#include <pthread.h>
#include <zmq.h>

#include <QuarkTS.h>
#include <system_signals.h>

#include <hsm_pse_init.h>

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
  char buf[32];
  int i, n;

  ctx = zmq_ctx_new ();
  sink = zmq_socket (ctx, ZMQ_PULL);
  zmq_connect (sink, "tcp://localhost:5555");

  while (1) {
    memset(buf, 0, sizeof(buf));
    n = zmq_recv (sink, buf, sizeof(buf), 0);
    if (n > 0) {
      if (strncmp(buf, "SYS_SIG_TYPE_1", sizeof("SYS_SIG_TYPE_1")) == 0) {
        printf("sig SYS_SIG_TYPE_1\n");
        qStateMachine_SendSignal ( NULL, SYS_SIG_TYPE_1, NULL, 0, qFalse );
      } else if (strncmp(buf, "SYS_SIG_TYPE_2", sizeof("SYS_SIG_TYPE_2")) == 0) {
        printf("sig SYS_SIG_TYPE_2\n");
        qStateMachine_SendSignal ( NULL, SYS_SIG_TYPE_2, NULL, 0, qFalse );
      } else if (strncmp(buf, "SYS_SIG_TYPE_3", sizeof("SYS_SIG_TYPE_3")) == 0) {
        printf("sig SYS_SIG_TYPE_3\n");
        qStateMachine_SendSignal ( NULL, SYS_SIG_TYPE_3, NULL, 0, qFalse );
      } else if (strncmp(buf, "SYS_SIG_TYPE_4", sizeof("SYS_SIG_TYPE_4")) == 0) {
        printf("sig SYS_SIG_TYPE_4\n");
        qStateMachine_SendSignal ( NULL, SYS_SIG_TYPE_4, NULL, 0, qFalse );
      }
    }
  }

  zmq_close (sink);
  zmq_ctx_destroy (ctx);
}

void hsms_init ()
{
  hsm_pse_init ();
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