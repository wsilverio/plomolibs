/////////////////////////////////////////////////////////////////
// bibliotecas
/////////////////////////////////////////////////////////////////
// nativas
#include <stdio.h> // printf
// usuário
#include "fifo.h"

/////////////////////////////////////////////////////////////////
// definições
/////////////////////////////////////////////////////////////////
/// Tamanho definido pelo usuário
#define FIFO_SIZE 5

/////////////////////////////////////////////////////////////////
// macros
/////////////////////////////////////////////////////////////////
#if !((FIFO_SIZE >= 1) && (FIFO_SIZE <= 254))
#error "O tamanho da Fifo deve ser maior ou igual a 1 e menor ou igual a 254."
#endif

/////////////////////////////////////////////////////////////////
// variáveis privadas do módulo
/////////////////////////////////////////////////////////////////
/// Objeto fifo
Fifo fifo;
/// Array a armazenar os dados da fifo.
/// @note o tamanho do buffer deve ser 1 unidade maior que o tamanho da fifo.
/// @note soma-se "1" pois os índices (escrita/leitura) nunca alcançam \a fifo.size.
/// @note o buffer não deve ser menor que 2, nem maior que 255.
/// @note o buffer não deve ser manipulado pelo usuário.
uint8_t buffer[FIFO_SIZE+1];

/////////////////////////////////////////////////////////////////
// implementação do exemplo
/////////////////////////////////////////////////////////////////
int main()
{
    /// Inicializa a fifo.
    INIT_FIFO(fifo, buffer);

    printf("#0 Fifo inicializada\n");
    printf("    Tamanho*: %d\n", fifo.size);
    printf("    Tamanho disponivel: %d\n", fifo.size-1);
    printf("    Numero de dados na fifo: %d\n\n", fifo_data_length(&fifo));

    // Preenche a fifo com bytes de 0 ... (FIFO_SIZE-1)
    // e testa a tentativa de escrita com a fifo cheia.
    for (uint8_t i = 0; i < FIFO_SIZE+1; ++i)
    {
        // Tenta escrever o byte "i" na fifo.
        if (fifo_push(&fifo, i))
        {
            // Ok, dado escrito.
            ////////////////////
            printf("#1 Sucesso ao escrever o byte: %d\n", i);
        }
        else
        {
            // Erro ao escrever (fifo cheia).
            /////////////////////////////////
            printf("#2 Erro ao escrever o byte: %d. Fifo cheia!\n", i);
        }

        printf("    Numero de dados na fifo: %d\n\n", fifo_data_length(&fifo));
    }

    uint8_t data; /// Variavel a armazenar os dados retirados da fifo.

    // Lê o dado mais antigo.
    if (fifo_pull(&fifo, &data))
    {
        // Ok. Dado resgatado.
        //////////////////////

        printf("#3 Sucesso ao resgatar o dado mais antigo: %d\n", data);
        printf("    Numero de dados na fifo: %d\n\n", fifo_data_length(&fifo));
    }
    else
    {
        // Erro. Não deveria.
        /////////////////////

        return 1;
    }

    // Se espera "sucesso".
    printf("#4 %s ao escrever o dado: 123\n", fifo_push(&fifo, 123) ? "Sucesso" : "Erro");
    printf("    Numero de dados na fifo: %d\n\n", fifo_data_length(&fifo));

    if (fifo_pull(&fifo, &data))
    {
        printf("#5 Sucesso ao resgatar o dado mais antigo: %d\n", data);
        printf("    Numero de dados na fifo: %d\n\n", fifo_data_length(&fifo));
    }

    // Regata todos os dados e testa a leitura com fifo vazia.
    for (uint8_t i = 0; i < FIFO_SIZE; ++i)
    {
        if (fifo_pull(&fifo, &data))
        {
            printf("#6 Sucesso ao resgatar o dado mais antigo: %d\n", data);
        }
        else
        {
            printf("#7 Erro ao resgatar o dado mais antigo. Fifo vazia!\n");
        }
        printf("    Numero de dados na fifo: %d\n\n", fifo_data_length(&fifo));
    }

    return 0;
}
