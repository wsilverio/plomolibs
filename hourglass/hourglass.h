/**
*
*         _.-"""-._
*    _.-""         ""-._
*  :"-.               .-":
*  '"-_"-._       _.-".-"'      @file   hourglass.h
*    ||T+._"-._.-"_.-"|         @author Wendeu
*    ||:   "-.|.-" : ||         @date   Oct 2018
*    || .   ' :|  .  ||         @brief  Soft-timer de 16-bits.
*    ||  .   '|| .   ||
*    ||   ';.:||'    ||
*    ||    '::||     ||
*    ||      )||     ||
*    ||     ':||     ||
*    ||   .' :||.    ||
*    ||  ' . :||.'   ||
*    ||.'-  .:|| -'._||
*  .-'": .::::||:. : "'-.
*  :"-.'::::::||::'  .-":
*   "-."-._"--:"  .-".-"
*      "-._"-._.-".-"
*          "-.|.-"
*/

#ifndef TEMPLATE_H
#define TEMPLATE_H

/////////////////////////////////////////////////////////////////
// bibliotecas
/////////////////////////////////////////////////////////////////
// nativas
#include <pthread.h> /// exemplo

// usuário
#include "plomodefs.h"
/////////////////////////////////////////////////////////////////
// definições
/////////////////////////////////////////////////////////////////

#define HOURGLASS_MASTER_TIMER_BASE_IN_MS 100

/////////////////////////////////////////////////////////////////
// dados personalizados, enumerações
/////////////////////////////////////////////////////////////////

/**
 * @brief Enumeração dos soft-timers do projeto.
 * @note Em ordem sequencial.
 */
typedef enum tagHOURGLASS_ID
{
    // IDENTIFICAÇÃO---------- | sand | reload | modo      | func                | estado
    HOURGLASS_ID__TIMER_A, /// | 5s    | ------ | one-shot  | soft_timer_callback | run
    HOURGLASS_ID__TIMER_B, /// | 2s    | 2s     | periódico | soft_timer_callback | wait
    HOURGLASS_ID__TIMER_C, /// | 1s    | 2,5s   | periódico | NULL                | run
    HOURGLASS_ID__TIMER_D, /// | 0,5s  | 0,5s   | one-shot  | soft_timer_callback | wait

    // ...
    HOURGLASS_TOTAL_ID /// Total de timers.
} HourglassId;

/**
 * @brief Constantes temporais baseadas em \a HOURGLASS_TIME_BASE_IN_MS. Devem ser
 *        alteradas manualmente conforme o valor de HOURGLASS_TIME_BASE_IN_MS.
 */
typedef enum tagHOURGLASS_TIMEOUT
{
    HOURGLASS_TIMER__OFF = 0,
    HOURGLASS_TIMER__100MS = 1,
    HOURGLASS_TIMER__200MS = 2,
    HOURGLASS_TIMER__300MS = 3,
    HOURGLASS_TIMER__400MS = 4,
    HOURGLASS_TIMER__500MS = 5,
    HOURGLASS_TIMER__1S = 10,
    HOURGLASS_TIMER__1S2 = 12,
    HOURGLASS_TIMER__2S = 20,
    HOURGLASS_TIMER__2S5 = 25,
    HOURGLASS_TIMER__3S = 30,
    HOURGLASS_TIMER__4S = 40,
    HOURGLASS_TIMER__5S = 50,
    HOURGLASS_TIMER__10S = 100,

} HourglassTimeout;

/**
 * @brief Modos de operação do timer.
 */
typedef enum tagHOURGLASS_MODES
{
    HOURGLASS_MODE__ONE_SHOT, /// Apenas uma contagem.
    HOURGLASS_MODE__PERIODIC, /// Contagem periódica.

    // ...
    HOURGLASS_TOTAL_MODES /// Contagem dos modos.
} HourglassMode;

/**
 * @brief Tipo das funções de callback.
 */
typedef ErrorStatus (*hourglass_callback)(HourglassId);
//typedef ErrorStatus (*hourglass_callback)(void*);

/////////////////////////////////////////////////////////////////
// variáveis globais
/////////////////////////////////////////////////////////////////
pthread_mutex_t gMutex1; /// exemplo
/////////////////////////////////////////////////////////////////
// macros
/////////////////////////////////////////////////////////////////
#define DISABLE_MASTER_INTERRUPT() do{ pthread_mutex_lock( &gMutex1 ); }while(0) /// exemplo
#define ENABLE_MASTER_INTERRUPT() do{ pthread_mutex_unlock( &gMutex1 ); }while(0) /// exemplo
/////////////////////////////////////////////////////////////////
// protótipo das funções públicas
/////////////////////////////////////////////////////////////////
void hourglass_init(void);
void hourglass_register_members(HourglassId id, HourglassTimeout timeout, HourglassTimeout reload, HourglassMode mode, /*void *param,*/ hourglass_callback func, FunctionalState status);
void hourglass_set_status(HourglassId id, FunctionalState newStatus);
FunctionalState get_status(HourglassId id);
void hourglass_restart(HourglassId id);
void hourglass_set_time(HourglassId id, HourglassTimeout newTime, FunctionalState newStatus);
uint16_t hourglass_get_time(HourglassId id);
void hourglass_set_reload(HourglassId id, HourglassTimeout reload);
uint16_t hourglass_get_reload(HourglassId id);
bool hourglass_is_timedout(HourglassId id);
bool hourglass_is_timedout_with_cleaning(HourglassId id);
void hourglass_clear_timeout_flag(HourglassId id);

void hourglass_timertick(void);

#endif // TEMPLATE_H
