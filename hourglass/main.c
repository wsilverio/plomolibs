/////////////////////////////////////////////////////////////////
// bibliotecas
/////////////////////////////////////////////////////////////////
#include <stdio.h>
#include <signal.h>
#include <errno.h>
#include <time.h>
#include <pthread.h>
#include <unistd.h>

#include "hourglass.h"
/////////////////////////////////////////////////////////////////
// definições
/////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////
// macros
/////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////
// variáveis privadas do módulo
/////////////////////////////////////////////////////////////////
static int thread_1_count;
/////////////////////////////////////////////////////////////////
// protótipo das funções privadas
/////////////////////////////////////////////////////////////////
ErrorStatus soft_timer_callback(HourglassId id);
static void *thread_1(void *arg);
/////////////////////////////////////////////////////////////////
// implementação do exemplo
/////////////////////////////////////////////////////////////////

static void *thread_2(void *arg)
{
    while (1)
    {
        if (hourglass_is_timedout_with_cleaning(HOURGLASS_ID__TIMER_C))
        {
            int seg = (thread_1_count/(HOURGLASS_MASTER_TIMER_BASE_IN_MS/10));
            int millis = (thread_1_count % seg);
            printf("#Timeout Timer 'C' @%d:%d\n", seg, millis);
            hourglass_restart(HOURGLASS_ID__TIMER_D);
        }
    }

    return NULL;
}

int main()
{
    // Inicializa o módulo.
    hourglass_init();

    // Registra os Timers.
    hourglass_register_members(HOURGLASS_ID__TIMER_A, HOURGLASS_TIMER__5S,    666,                    HOURGLASS_MODE__ONE_SHOT, soft_timer_callback, ENABLE);
    hourglass_register_members(HOURGLASS_ID__TIMER_B, HOURGLASS_TIMER__2S,    HOURGLASS_TIMER__2S,    HOURGLASS_MODE__PERIODIC, soft_timer_callback, DISABLE);
    hourglass_register_members(HOURGLASS_ID__TIMER_C, HOURGLASS_TIMER__1S,    HOURGLASS_TIMER__2S5,   HOURGLASS_MODE__PERIODIC, NULL,                ENABLE);
    hourglass_register_members(HOURGLASS_ID__TIMER_D, HOURGLASS_TIMER__500MS, HOURGLASS_TIMER__500MS, HOURGLASS_MODE__ONE_SHOT, soft_timer_callback, DISABLE);

    ///////////////////////////////////////////
    // Simulação do Master Timer
    pthread_t t_1;
    pthread_t t_2;
    sigset_t alarm_sig;
    sigemptyset(&alarm_sig);
    sigemptyset(&alarm_sig);
    for (int i = SIGRTMIN; i <= SIGRTMAX; i++)
    {
        sigaddset(&alarm_sig, i);
    }
    sigprocmask(SIG_BLOCK, &alarm_sig, NULL);
    pthread_create(&t_1, NULL, thread_1, NULL);
    pthread_create(&t_2, NULL, thread_2, NULL);
    sleep(20);
    printf("Thread 1 %d iterations\n", thread_1_count);
    ///////////////////////////////////////////

    return 0;
}

/**
 * @brief Esta função está assimilada ao soft-timers do exemplo.
 * @param id Identificação.
 * @return Conclusão da execução.
 */
ErrorStatus soft_timer_callback(HourglassId id)
{
    ErrorStatus retValue = SUCCESS;

    printf("#Callback Timer \'");

    switch(id)
    {
        case HOURGLASS_ID__TIMER_A:
        {
            printf("A");
        } break;

        case HOURGLASS_ID__TIMER_B:
        {
            printf("B");
        } break;

        case HOURGLASS_ID__TIMER_D:
        {
            printf("D");
        } break;

        default:
        {
            printf("ERROR");
            retValue = ERROR;
        } break;
    }

    int seg = (thread_1_count/(HOURGLASS_MASTER_TIMER_BASE_IN_MS/10));
    int millis = (thread_1_count % seg);
    printf("\' @%d:%d\n", seg, millis);

    return retValue;
}

/**
 * @brief Trava o programa caso algum assert seja falho.
 */
void assert_failed()
{
    while(true)
    {

    }
}

/**
 *  Threads para simulação do Timer master por: Chris Simmonds (chris@2net.co.uk) https://github.com/csimmonds/periodic-threads
 */

struct periodic_info
{
    int sig;
    sigset_t alarm_sig;
};

static int make_periodic(int unsigned period, struct periodic_info *info)
{
    static int next_sig;
    int ret;
    unsigned int ns;
    unsigned int sec;
    struct sigevent sigev;
    timer_t timer_id;
    struct itimerspec itval;

    /* Initialise next_sig first time through. We can't use static
       initialisation because SIGRTMIN is a function call, not a constant */
    if (next_sig == 0)
        next_sig = SIGRTMIN;
    /* Check that we have not run out of signals */
    if (next_sig > SIGRTMAX)
        return -1;
    info->sig = next_sig;
    next_sig++;
    /* Create the signal mask that will be used in wait_period */
    sigemptyset(&(info->alarm_sig));
    sigaddset(&(info->alarm_sig), info->sig);

    /* Create a timer that will generate the signal we have chosen */
    sigev.sigev_notify = SIGEV_SIGNAL;
    sigev.sigev_signo = info->sig;
    sigev.sigev_value.sival_ptr = (void *)&timer_id;
    ret = timer_create(CLOCK_MONOTONIC, &sigev, &timer_id);
    if (ret == -1)
        return ret;

    /* Make the timer periodic */
    sec = period / 1000000;
    ns = (period - (sec * 1000000)) * 1000;
    itval.it_interval.tv_sec = sec;
    itval.it_interval.tv_nsec = ns;
    itval.it_value.tv_sec = sec;
    itval.it_value.tv_nsec = ns;
    ret = timer_settime(timer_id, 0, &itval, NULL);
    return ret;
}

static void wait_period(struct periodic_info *info)
{
    int sig;
    sigwait(&(info->alarm_sig), &sig);
}

static void *thread_1(void *arg)
{
    struct periodic_info info;

    printf("Thread 1 period 100ms\n");
    make_periodic(HOURGLASS_MASTER_TIMER_BASE_IN_MS * 1000, &info);
    while (1)
    {
        DISABLE_MASTER_INTERRUPT();
        thread_1_count++;
        ENABLE_MASTER_INTERRUPT();
        wait_period(&info);

        hourglass_timertick(); /// <------------------------- HANDLER DO MASTER TIMER
    }
    return NULL;
}
