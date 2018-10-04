/**
*  ._________________.
*  | _______________ |      @file   fifo.h
*  | I             I |      @author Wendeu
*  | I cd plomo/   I |      @date   Oct 2018
*  | I rm -rf fifo I |      @brief  Implementação simples para uma FIFO ("First in, first out")
*  | I             I |              de até 254 posições, com dados do tipo "uint8_t".
*  | I_____________I |
*  !_________________!      Como usar:
*     ._[_______]_.             Declarar um objeto do tipo FIFO. Ter em mente que seu tamanho será 1 unidade menor que o do seu buffer;
* .___|___________|___.         Declarar o buffer: vetor de bytes (uint8_t) com tamanho entre 2 e 255 (inclusivos);
* |::: ____           |         Inicializar a fifo e assimilá-la a seu buffer através da macro INIT_FIFO;
* |    ~~~~ [CD-DVD]  |         Manipular a fifo através das funções fifo_push() e fifo_pull().
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
*   @param      extBuffer array externo (uint8_t[]) com sizeof() entre 2 e 255.
*/
#define INIT_FIFO(fifo, extBuffer) \
    do{ \
    fifo.buffer = extBuffer; \
    fifo.size = sizeof(extBuffer); \
    fifo.readIndex = fifo.writeIndex = 0; \
    }while(0)

/////////////////////////////////////////////////////////////////
// protótipo das funções públicas
/////////////////////////////////////////////////////////////////
uint8_t fifo_data_length(FIFO *fifo);
bool fifo_push(FIFO *fifo, uint8_t data);
bool fifo_pull(FIFO *fifo, uint8_t *data);

#endif // FIFO_H
