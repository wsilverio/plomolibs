/////////////////////////////////////////////////////////////////
// bibliotecas
/////////////////////////////////////////////////////////////////
#include <stdio.h>
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

/////////////////////////////////////////////////////////////////
// protótipo das funções privadas
/////////////////////////////////////////////////////////////////
ErrorStatus soft_timer_callback(HOURGLASS_ID id);

/////////////////////////////////////////////////////////////////
// implementação do exemplo
/////////////////////////////////////////////////////////////////

int main() {

    // Inicializa o módulo.
    hourglass_init();

    // Registra os Timers.
    hourglass_register_members(HOURGLASS_ID__TIMER_A, HOURGLASS_TIMER__5S, 666, HOURGLASS_MODE__ONE_SHOT, soft_timer_callback, ENABLE);

    return 0;
}

/**
 * @brief Esta função está assimilada ao soft-timers do exemplo.
 * @param id Identificação.
 * @return Conclusão da execução.
 */
ErrorStatus soft_timer_callback(HOURGLASS_ID id)
{
    ErrorStatus retValue = SUCCESS;

    // TODO: printar tempo tag

    switch(id)
    {
        case HOURGLASS_ID__TIMER_A:
        {
            printf("# Callback Timer A\n");
        } break;

        case HOURGLASS_ID__TIMER_B:
        {
            printf("# Callback Timer B\n");
        } break;

        case HOURGLASS_ID__TIMER_C:
        {
            printf("# Callback Timer C\n");
        } break;

        default:
        {
            printf("# ERRO Timer Callback\n");
            retValue = ERROR;
        } break;
    }

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
