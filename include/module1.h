#ifndef MODULE1_H
#define MODULE1_H

class Module1 {
public:
    Module1(); // Construtor
    ~Module1(); // Destrutor

    void initModule1(); // Inicializa a conexão Wi-Fi
    void runModule1(); // Mantém a conexão Wi-Fi ativa

private:
    bool ledStatusMain; // Variável de estado do LED
};

#endif // MODULE1