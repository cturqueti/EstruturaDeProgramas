// ---------------------------------------------------------------------------------------------------------------- //
// 
// Descrição: Este arquivo contém funções utilitárias que podem ser usadas em todo o projeto.
//
// O que pode ser adicionar:
// Funções de delay.
// Funções de conversão (por exemplo, de inteiro para string).
// Funções de manipulação de strings.
// 
//  Obs.: A palavra-chave inline diz ao compilador para incluir a implementação da função diretamente no local onde ela é chamada, evitando múltiplas definições.
//  Se a função for grande ou complexa, é melhor movê-la para um arquivo .cpp.
// ---------------------------------------------------------------------------------------------------------------- //

#ifndef UTILS_H
#define UTILS_H

class Utils {
public:
    static void customDelay(unsigned long ms);
};

#endif // UTILS_H
