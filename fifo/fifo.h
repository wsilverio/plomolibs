/**
*  ._________________.
*  | _______________ |
*  | I             I |
*  | I cd plomo/   I |      @file   fifo.h
*  | I rm -rf fifo I |      @author Wendeu
*  | I             I |      @date   Oct 2018
*  | I_____________I |      @brief  Implementação simples para uma FIFO ("First in, first out")
*  !_________________!              de até 255 posições, com dados do tipo "uint8_t".
*     ._[_______]_.
* .___|___________|___.
* |::: ____           |
* |    ~~~~ [CD-DVD]  |
* !___________________!
*/

#ifndef FIFO_H
#define FIFO_H

/////////////////////////////////////////////////////////////////
// bibliotecas
/////////////////////////////////////////////////////////////////
// nativas

// usuário
#include "plomodefs.h"
/////////////////////////////////////////////////////////////////
// definições
/////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////
// dados personalizados, enumerações
/////////////////////////////////////////////////////////////////
typedef struct tagFIFO
{
    uint8_t size;       /// tamanho do buffer, em bytes.
    uint8_t *buffer;    /// ponteiro para o array externo.
    uint8_t readIndex;  /// índice de leitura.
    uint8_t writeIndex; /// índice de escrita.
} FIFO;
/////////////////////////////////////////////////////////////////
// variáveis globais
/////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////
// macros
/////////////////////////////////////////////////////////////////
/**
*  \def INIT_FIFO(fifo, buffer)
*   @brief      Inicializa e assimila o objeto \a fifo ao array externo \a buffer.
*   @param      fifo objeto FIFO.
*   @param      buffer array externo (uint8_t[]).
*   @returns    None.
*/
#define INIT_FIFO(fifo, buffer) \
    do{ \
    fifo.buffer = buffer; \
    fifo.size = sizeof(buffer); \
    fifo.readIndex = fifo.writeIndex = 0; \
    }while(0)
/////////////////////////////////////////////////////////////////
// protótipo das funções públicas
/////////////////////////////////////////////////////////////////
uint8_t fifo_data_length(FIFO *fifo);
bool fifo_push(FIFO *fifo, uint8_t data);
bool fifo_pull(FIFO *fifo, uint8_t *data);

#endif // FIFO_H
