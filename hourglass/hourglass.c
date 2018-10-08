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

/////////////////////////////////////////////////////////////////
// bibliotecas
/////////////////////////////////////////////////////////////////
// nativas
#include <string.h>

// usuário
#include "hourglass.h"
/////////////////////////////////////////////////////////////////
// definições
/////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////
// dados personalizados, enumerações
/////////////////////////////////////////////////////////////////

/**
 * @brief Array de timers.
 */
struct
{
    HourglassTimeout sand; /// contador.
    HourglassTimeout reload; /// recarga do contador (se \a mode = HOURGLASS_MODE__PERIODIC).
    HourglassMode mode; /// modo de operação.
    FunctionalState status; /// sinaliza que o timer está rodando.
    void *param; /// parâmetro a ser passado para a função de callback.
    hourglass_callback callback_func; /// função de callback.
    bool timedout; /// sinaliza que ocorreu, ao menos, um estouro.
} mHourglass[HOURGLASS_TOTAL_ID];

/////////////////////////////////////////////////////////////////
// variáveis globais
/////////////////////////////////////////////////////////////////
pthread_mutex_t gMutex1 = PTHREAD_MUTEX_INITIALIZER;
/////////////////////////////////////////////////////////////////
// variáveis privadas do módulo
/////////////////////////////////////////////////////////////////

bool mInit = false; /// Sinaliza que o módulo foi inicializado.

/////////////////////////////////////////////////////////////////
// macros
/////////////////////////////////////////////////////////////////

/** \def IS_VALID_ID(id)
 * @brief Verifica se o id é válido.
 * @param id ID a ser verificado.
 */
#define IS_VALID_ID(id) ((bool)((id) < HOURGLASS_TOTAL_ID)) // ***macro insegura***
//#define IS_VALID_ID(id) (bool)((id) < GET_ARRAY_LENGTH(mHourglass))

/////////////////////////////////////////////////////////////////
// protótipo das funções privadas
/////////////////////////////////////////////////////////////////
void hourglass_deinit(HourglassId id);
/////////////////////////////////////////////////////////////////
// implementação do módulo
/////////////////////////////////////////////////////////////////

/**
 * @brief Rotina de inicialização do módulo (array \a mHOURGLASS). Deve ser chamada antes da configuração do timer Master.
 * @param None.
 * @return None.
 */
void hourglass_init(void)
{
    for (HourglassId id = (HourglassId)0; id < HOURGLASS_TOTAL_ID; ++id)
    {
        hourglass_deinit(id);
    }

    mInit = true;
}

/**
 * @brief Deinicializa um determinado timer.
 * @param id Identificação.
 */
void hourglass_deinit(HourglassId id)
{
    assert_param(IS_VALID_ID(id));

    mHourglass[id].sand = HOURGLASS_TIMER__OFF;
    mHourglass[id].reload = HOURGLASS_TIMER__OFF;
    mHourglass[id].mode = HOURGLASS_MODE__ONE_SHOT;
    mHourglass[id].status = DISABLE;
    //mHourglass[id].param = NULL;
    mHourglass[id].callback_func = NULL;
    mHourglass[id].timedout = false;
}

/**
 * @brief Registra os parâmetros do objeto mHOURGLASS.
 * @param id identificação do objeto.
 * @param timeout tempo de estouro.
 * @param reload tempo para recarga.
 * @param mode modo de operação.
 * @param param parâmetro da função de callback.
 * @param func função de callback.
 * @param launch se deve iniciar a contagem imediatamente.
 */
void hourglass_register_members(HourglassId id, HourglassTimeout timeout, HourglassTimeout reload, HourglassMode mode, /*void *param,*/ hourglass_callback func, FunctionalState status)
{
    assert_param(IS_VALID_ID(id) && mInit);

    mHourglass[id].sand = timeout;
    mHourglass[id].reload = reload;
    mHourglass[id].mode = mode;
    //mHourglass[id].param = param;
    mHourglass[id].callback_func = func;
    mHourglass[id].timedout = false;

    mHourglass[id].status = status;
}

/**
 * @brief Configura um novo status.
 * @param id Identificação.
 * @param newStatus Novo status.
 */
void hourglass_set_status(HourglassId id, FunctionalState newStatus)
{
    assert_param(IS_VALID_ID(id) && mInit);

    DISABLE_MASTER_INTERRUPT();
    mHourglass[id].status = newStatus;
    ENABLE_MASTER_INTERRUPT();
}

/**
 * @brief Captura o status atual do timer.
 * @param id Identificação.
 * @return Status atual do timer.
 */
FunctionalState get_status(HourglassId id)
{
    assert_param(IS_VALID_ID(id) && mInit);

    DISABLE_MASTER_INTERRUPT();
    FunctionalState copy = mHourglass[id].status;
    ENABLE_MASTER_INTERRUPT();

    return copy;
}

/**
 * @brief Reinicia o timer.
 * @param id Identificação
 */
void hourglass_restart(HourglassId id)
{
    assert_param(IS_VALID_ID(id) && mInit);

    DISABLE_MASTER_INTERRUPT();
    mHourglass[id].sand = mHourglass[id].reload;
    mHourglass[id].timedout = false;
    mHourglass[id].status = ENABLE;
    ENABLE_MASTER_INTERRUPT();
}

/**
 * @brief Configura um novo tempo de estouro.
 * @param id Identificação.
 * @param newTime Novo tempo para contagem.
 * @param newStatus Novo estado do timer.
 */
void hourglass_set_time(HourglassId id, HourglassTimeout newTime, FunctionalState newStatus)
{
    assert_param(IS_VALID_ID(id) && mInit);

    DISABLE_MASTER_INTERRUPT();
    mHourglass[id].sand = newTime;
    mHourglass[id].timedout = false;
    mHourglass[id].status = newStatus;
    ENABLE_MASTER_INTERRUPT();
}

/**
 * @brief Captura o valor atual do contador.
 * @param id Identificação.
 * @return Valor do contador.
 */
uint16_t hourglass_get_time(HourglassId id)
{
    assert_param(IS_VALID_ID(id) && mInit);

    DISABLE_MASTER_INTERRUPT();
    uint16_t copy = mHourglass[id].sand;
    ENABLE_MASTER_INTERRUPT();

    return copy;
}

/**
 * @brief Define um novo tempo para recarga do contador.
 * @param id Identificação.
 * @param reload Valor da recarga.
 */
void hourglass_set_reload(HourglassId id, HourglassTimeout reload)
{
    assert_param(IS_VALID_ID(id) && mInit);

    DISABLE_MASTER_INTERRUPT();
    mHourglass[id].reload = reload;
    ENABLE_MASTER_INTERRUPT();
}

/**
 * @brief Captura o valor atual da recarga.
 * @param id Identificação.
 * @return Valor da recarga.
 */
uint16_t hourglass_get_reload(HourglassId id)
{
    assert_param(IS_VALID_ID(id) && mInit);

    DISABLE_MASTER_INTERRUPT();
    uint16_t copy = mHourglass[id].reload;
    ENABLE_MASTER_INTERRUPT();

    return copy;
}

/**
 * @brief Verifica se um determinado timer foi estourado.
 * @param id Identificação.
 * @return false, se não estourou o tempo, true caso contrário.
 */
bool hourglass_is_timedout(HourglassId id)
{
    assert_param(IS_VALID_ID(id) && mInit);

    DISABLE_MASTER_INTERRUPT();
    bool status = mHourglass[id].timedout;
    ENABLE_MASTER_INTERRUPT();

    return status;
}

/**
 * @brief Função semelhante à \a hourglass_is_timedout(), porém limpa a flag de timeout.
 *        Deve ser utilizada quando não há função de callback (NULL) ou outro caso específico.
 * @param id Identificação.
 * @return false, se não estourou o tempo, true caso contrário.
 */
bool hourglass_is_timedout_with_cleaning(HourglassId id)
{
    assert_param(IS_VALID_ID(id) && mInit);

    DISABLE_MASTER_INTERRUPT();
    bool copy = mHourglass[id].timedout;
    mHourglass[id].timedout = false;
    ENABLE_MASTER_INTERRUPT();

    return copy;
}

/**
 * @brief Limpa manualmente a flag de estouro.
 * @param id Identificação.
 */
void hourglass_clear_timeout_flag(HourglassId id)
{
    assert_param(IS_VALID_ID(id) && mInit);

    DISABLE_MASTER_INTERRUPT();
    mHourglass[id].timedout = false;
    ENABLE_MASTER_INTERRUPT();
}

/**
 * @brief Atualização temporal do módulo. Deve ser chamada pelo handler da interrupção do Timer Master.
 */
void hourglass_timertick(void)
{
    assert_param(mInit);

    // Percorre todo o array.
    for (HourglassId id = (HourglassId)0; id < HOURGLASS_TOTAL_ID; ++id)
    {
        // Verifica se o timer está rodando.
        if (mHourglass[id].status != DISABLE)
        {
            // Verifica se o contador está ativo.
            if (mHourglass[id].sand != 0)
            {
                // Decrementa o contador;
                --mHourglass[id].sand;

                // Verifica se o decremento gerou timeout.
                if (mHourglass[id].sand == 0)
                {
                    // TIMEOUT.
                    ///////////

                    // Sinaliza o timeout.
                    mHourglass[id].timedout = true;

                    // Verifica se foi registrada uma função de callback.
                    if (mHourglass[id].callback_func != NULL)
                    {
                        // Lança a função de callback e captura o retorno.
                        // Se a função for bem concluída, então a flag de timeout será limpa.
                        if (mHourglass[id].callback_func(id) == SUCCESS)
                        //if (mHourglass[id].callback_func(mHourglass[id].param) == SUCCESS)
                        {
                            // A função de callback já concluiu o trabalho.
                            ///////////////////////////////////////////////

                            // Limpa a flag de timeout.
                            mHourglass[id].timedout = false;
                        }
                    }

                    // Verifica se o timer é periódico.
                    if (mHourglass[id].mode == HOURGLASS_MODE__PERIODIC)
                    {
                        // Então recarrega o timer.
                        mHourglass[id].sand = mHourglass[id].reload;
                    }
                    else
                    {
                        // Senão, pára o timer.
                        mHourglass[id].status = DISABLE;
                    }
                }
            }
        }
    }
}
