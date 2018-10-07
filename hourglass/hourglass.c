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
    HOURGLASS_TIMEOUT count; /// contador.
    HOURGLASS_TIMEOUT reload; /// recarga do contador (se \a mode = HOURGLASS_MODE__PERIODIC).
    HOURGLASS_MODE mode; /// modo de operação.
    FunctionalState status; /// sinaliza que o timer está rodando.
    void *param; /// parâmetro a ser passado para a função de callback.
    hourglass_callback callback_func; /// função de callback.
    bool timedout; /// sinaliza que ocorreu, ao menos, um estouro.
} mHOURGLASS[HOURGLASS_TOTAL_ID];

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
//#define IS_VALID_ID(id) (bool)((id) < GET_ARRAY_LENGTH(mHOURGLASS))

/////////////////////////////////////////////////////////////////
// protótipo das funções privadas
/////////////////////////////////////////////////////////////////
void hourglass_deinit(HOURGLASS_ID id);
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
    for (HOURGLASS_ID id = (HOURGLASS_ID)0; id < HOURGLASS_TOTAL_ID; ++id)
    {
        hourglass_deinit(id);
    }

    mInit = true;
}

/**
 * @brief Deinicializa um determinado timer.
 * @param id Identificação.
 */
void hourglass_deinit(HOURGLASS_ID id)
{
    assert_param(IS_VALID_ID(id));

    mHOURGLASS[id].count = HOURGLASS_TIMER__OFF;
    mHOURGLASS[id].reload = HOURGLASS_TIMER__OFF;
    mHOURGLASS[id].mode = HOURGLASS_MODE__ONE_SHOT;
    mHOURGLASS[id].status = DISABLE;
    //mHOURGLASS[id].param = NULL;
    mHOURGLASS[id].callback_func = NULL;
    mHOURGLASS[id].timedout = false;
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
void hourglass_register_members(HOURGLASS_ID id, HOURGLASS_TIMEOUT timeout, HOURGLASS_TIMEOUT reload, HOURGLASS_MODE mode, /*void *param,*/ hourglass_callback func, FunctionalState status)
{
    assert_param(IS_VALID_ID(id) && mInit);

    mHOURGLASS[id].count = timeout;
    mHOURGLASS[id].reload = reload;
    mHOURGLASS[id].mode = mode;
    //mHOURGLASS[id].param = param;
    mHOURGLASS[id].callback_func = func;
    mHOURGLASS[id].timedout = false;

    mHOURGLASS[id].status = status;
}

/**
 * @brief Configura um novo status.
 * @param id Identificação.
 * @param newStatus Novo status.
 */
void hourglass_set_status(HOURGLASS_ID id, FunctionalState newStatus)
{
    assert_param(IS_VALID_ID(id) && mInit);

    DISABLE_MASTER_INTERRUPT();
    mHOURGLASS[id].status = newStatus;
    ENABLE_MASTER_INTERRUPT();
}

/**
 * @brief Captura o status atual do timer.
 * @param id Identificação.
 * @return Status atual do timer.
 */
FunctionalState get_status(HOURGLASS_ID id)
{
    assert_param(IS_VALID_ID(id) && mInit);

    DISABLE_MASTER_INTERRUPT();
    FunctionalState copy = mHOURGLASS[id].status;
    ENABLE_MASTER_INTERRUPT();

    return copy;
}

/**
 * @brief Reinicia o timer.
 * @param id Identificação
 */
void hourglass_restart(HOURGLASS_ID id)
{
    assert_param(IS_VALID_ID(id) && mInit);

    DISABLE_MASTER_INTERRUPT();
    mHOURGLASS[id].count = mHOURGLASS[id].reload;
    mHOURGLASS[id].timedout = false;
    mHOURGLASS[id].status = ENABLE;
    ENABLE_MASTER_INTERRUPT();
}

/**
 * @brief Configura um novo tempo de estouro.
 * @param id Identificação.
 * @param newTime Novo tempo para contagem.
 * @param newStatus Novo estado do timer.
 */
void hourglass_set_time(HOURGLASS_ID id, HOURGLASS_TIMEOUT newTime, FunctionalState newStatus)
{
    assert_param(IS_VALID_ID(id) && mInit);

    DISABLE_MASTER_INTERRUPT();
    mHOURGLASS[id].count = newTime;
    mHOURGLASS[id].timedout = false;
    mHOURGLASS[id].status = newStatus;
    ENABLE_MASTER_INTERRUPT();
}

/**
 * @brief Captura o valor atual do contador.
 * @param id Identificação.
 * @return Valor do contador.
 */
uint16_t hourglass_get_time(HOURGLASS_ID id)
{
    assert_param(IS_VALID_ID(id) && mInit);

    DISABLE_MASTER_INTERRUPT();
    uint16_t copy = mHOURGLASS[id].count;
    ENABLE_MASTER_INTERRUPT();

    return copy;
}

/**
 * @brief Define um novo tempo para recarga do contador.
 * @param id Identificação.
 * @param reload Valor da recarga.
 */
void hourglass_set_reload(HOURGLASS_ID id, HOURGLASS_TIMEOUT reload)
{
    assert_param(IS_VALID_ID(id) && mInit);

    DISABLE_MASTER_INTERRUPT();
    mHOURGLASS[id].reload = reload;
    ENABLE_MASTER_INTERRUPT();
}

/**
 * @brief Captura o valor atual da recarga.
 * @param id Identificação.
 * @return Valor da recarga.
 */
uint16_t hourglass_get_reload(HOURGLASS_ID id)
{
    assert_param(IS_VALID_ID(id) && mInit);

    DISABLE_MASTER_INTERRUPT();
    uint16_t copy = mHOURGLASS[id].reload;
    ENABLE_MASTER_INTERRUPT();

    return copy;
}

/**
 * @brief Verifica se um determinado timer foi estourado.
 * @param id Identificação.
 * @return false, se não estourou o tempo, true caso contrário.
 */
bool hourglass_is_timedout(HOURGLASS_ID id)
{
    assert_param(IS_VALID_ID(id) && mInit);

    DISABLE_MASTER_INTERRUPT();
    bool status = mHOURGLASS[id].timedout;
    ENABLE_MASTER_INTERRUPT();

    return status;
}

/**
 * @brief Função semelhante à \a hourglass_is_timedout(), porém limpa a flag de timeout.
 *        Deve ser utilizada quando não há função de callback (NULL) ou outro caso específico.
 * @param id Identificação.
 * @return false, se não estourou o tempo, true caso contrário.
 */
bool hourglass_is_timedout_with_cleaning(HOURGLASS_ID id)
{
    assert_param(IS_VALID_ID(id) && mInit);

    DISABLE_MASTER_INTERRUPT();
    bool copy = mHOURGLASS[id].timedout;
    mHOURGLASS[id].timedout = false;
    ENABLE_MASTER_INTERRUPT();

    return copy;
}

/**
 * @brief Limpa manualmente a flag de estouro.
 * @param id Identificação.
 */
void hourglass_clear_timeout_flag(HOURGLASS_ID id)
{
    assert_param(IS_VALID_ID(id) && mInit);

    DISABLE_MASTER_INTERRUPT();
    mHOURGLASS[id].timedout = false;
    ENABLE_MASTER_INTERRUPT();
}

/**
 * @brief Atualização temporal do módulo. Deve ser chamada pelo handler da interrupção do Timer Master.
 */
void hourglass_timertick(void)
{
    assert_param(mInit);

    // Percorre todo o array.
    for (HOURGLASS_ID id = (HOURGLASS_ID)0; id < HOURGLASS_TOTAL_ID; ++id)
    {
        // Verifica se o timer está rodando.
        if (mHOURGLASS[id].status != DISABLE)
        {
            // Verifica se o contador está ativo.
            if (mHOURGLASS[id].count != 0)
            {
                // Decrementa o contador;
                --mHOURGLASS[id].count;

                // Verifica se o decremento gerou timeout.
                if (mHOURGLASS[id].count == 0)
                {
                    // TIMEOUT.
                    ///////////

                    // Sinaliza o timeout.
                    mHOURGLASS[id].timedout = true;

                    // Verifica se foi registrada uma função de callback.
                    if (mHOURGLASS[id].callback_func != NULL)
                    {
                        // Lança a função de callback e captura o retorno.
                        // Se a função for bem concluída, então a flag de timeout será limpa.
                        if (mHOURGLASS[id].callback_func(id) == SUCCESS)
                        //if (mHOURGLASS[id].callback_func(mHOURGLASS[id].param) == SUCCESS)
                        {
                            // A função de callback já concluiu o trabalho.
                            ///////////////////////////////////////////////

                            // Limpa a flag de timeout.
                            mHOURGLASS[id].timedout = false;
                        }
                    }

                    // Verifica se o timer é periódico.
                    if (mHOURGLASS[id].mode == HOURGLASS_MODE__PERIODIC)
                    {
                        // Então recarrega o timer.
                        mHOURGLASS[id].count = mHOURGLASS[id].reload;
                    }
                    else
                    {
                        // Senão, pára o timer.
                        mHOURGLASS[id].status = DISABLE;
                    }
                }
            }
        }
    }
}
