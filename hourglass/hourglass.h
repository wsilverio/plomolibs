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

// usuário
#include "plomodefs.h"
/////////////////////////////////////////////////////////////////
// definições
/////////////////////////////////////////////////////////////////

#define HOURGLASS_MASTER_TIMER_RELOAD_VALUE 0
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
    // IDENTIFICAÇÃO---------- | count | reload | modo      | param | func                | estado
    HOURGLASS_ID__TIMER_A, /// | 5s    | ------ | one-shot  | id    | soft_timer_callback | run
    HOURGLASS_ID__TIMER_B, /// | 2s    | 2s     | periódico | id    | soft_timer_callback | wait
    HOURGLASS_ID__TIMER_C, /// | 1s    | 2,5s   | periódico | ----- | NULL                | run
    HOURGLASS_ID__TIMER_D, /// | 0,5s  | ------ | one-shot  | id    | soft_timer_callback | wait

    // ...
    HOURGLASS_TOTAL_ID /// Total de timers.
} HOURGLASS_ID;

/**
 * @brief Constantes temporais baseadas em \a HOURGLASS_TIME_BASE_IN_MS. Devem ser
 *        alteradas manualmente conforme o valor de HOURGLASS_TIME_BASE_IN_MS.
 */
typedef enum tagHOURGLASS_TIMEOUT
{
    HOURGLASS_TIMER__OFF = 0,
    HOURGLASS_TIMER__100MS = 1*HOURGLASS_MASTER_TIMER_BASE_IN_MS,
    HOURGLASS_TIMER__200MS = 2*HOURGLASS_MASTER_TIMER_BASE_IN_MS,
    HOURGLASS_TIMER__300MS = 3*HOURGLASS_MASTER_TIMER_BASE_IN_MS,
    HOURGLASS_TIMER__400MS = 4*HOURGLASS_MASTER_TIMER_BASE_IN_MS,
    HOURGLASS_TIMER__500MS = 5*HOURGLASS_MASTER_TIMER_BASE_IN_MS,
    HOURGLASS_TIMER__1S = 10*HOURGLASS_MASTER_TIMER_BASE_IN_MS,
    HOURGLASS_TIMER__1S2 = 12*HOURGLASS_MASTER_TIMER_BASE_IN_MS,
    HOURGLASS_TIMER__2S = 20*HOURGLASS_MASTER_TIMER_BASE_IN_MS,
    HOURGLASS_TIMER__2S5 = 25*HOURGLASS_MASTER_TIMER_BASE_IN_MS,
    HOURGLASS_TIMER__3S = 30*HOURGLASS_MASTER_TIMER_BASE_IN_MS,
    HOURGLASS_TIMER__4S = 40*HOURGLASS_MASTER_TIMER_BASE_IN_MS,
    HOURGLASS_TIMER__5S = 50*HOURGLASS_MASTER_TIMER_BASE_IN_MS,
    HOURGLASS_TIMER__10S = 100*HOURGLASS_MASTER_TIMER_BASE_IN_MS,

} HOURGLASS_TIMEOUT;

/**
 * @brief Modos de operação do timer.
 */
typedef enum tagHOURGLASS_MODES
{
    HOURGLASS_MODE__ONE_SHOT, /// Apenas uma contagem.
    HOURGLASS_MODE__PERIODIC, /// Contagem periódica.

    // ...
    HOURGLASS_TOTAL_MODES /// Contagem dos modos.
} HOURGLASS_MODE;

/**
 * @brief Tipo das funções de callback.
 */
typedef ErrorStatus (*hourglass_callback)(HOURGLASS_ID);
//typedef ErrorStatus (*hourglass_callback)(void*);

/////////////////////////////////////////////////////////////////
// variáveis globais
/////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////
// macros
/////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////
// protótipo das funções públicas
/////////////////////////////////////////////////////////////////
void hourglass_init(void);
void hourglass_register_members(HOURGLASS_ID id, HOURGLASS_TIMEOUT timeout, HOURGLASS_TIMEOUT reload, HOURGLASS_MODE mode, /*void *param,*/ hourglass_callback func, FunctionalState status);
void hourglass_set_status(HOURGLASS_ID id, FunctionalState newStatus);
FunctionalState get_status(HOURGLASS_ID id);
void hourglass_restart(HOURGLASS_ID id);
void hourglass_set_time(HOURGLASS_ID id, HOURGLASS_TIMEOUT newTime, FunctionalState newStatus);
uint16_t hourglass_get_time(HOURGLASS_ID id);
void hourglass_set_reload(HOURGLASS_ID id, HOURGLASS_TIMEOUT reload);
uint16_t hourglass_get_reload(HOURGLASS_ID id);
bool hourglass_is_timedout(HOURGLASS_ID id);
bool hourglass_is_timedout_with_cleaning(HOURGLASS_ID id);
void hourglass_clear_timeout_flag(HOURGLASS_ID id);

void hourglass_timertick(void);

#endif // TEMPLATE_H
