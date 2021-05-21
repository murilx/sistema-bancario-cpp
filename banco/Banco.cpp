#include <stdio.h>
#include <vector>
#include "Banco.hpp"

void Banco::testaTotal() throw (totalIncorreto) {
    /* Testa o balanço total do banco e emite um erro
       caso o balanço atual seja diferente do balanço inicial */
    int total_teste = 0;

    for(int i = 0; i < (int)this->contas.size(); i++) {
        total_teste += this->contas.at(i)->getSaldo();
    }

    if(this->getTotal() != total_teste) throw totalIncorreto();
}

Conta* Banco::testaEntrada(int id) throw (entradaInvalida) {
    /* Valida os IDs enviados pelo usuário e se existir alguma conta com esse
       ID, retorna a conta */
    Conta *aux;
    int found;

    // Verifica se a conta enviada é uma conta que existe
    // no banco
    for(int i = 0; i < (int)this->contas.size(); i++) {
        if(id == this->contas.at(i)->getID()) {
            aux = this->contas.at(i);
            found = 1;
            break;
        } else found = 0;
    }

    // Envia um erro de entrada invalida caso o ID inserido
    // não corresponda com nenhum ID das contas
    if(!found) throw entradaInvalida();

    return (aux);
}

void Banco::mostraClientes() {
    /* Mostra todos os clientes cadastrados no banco */
    printf("\nClientes: \n");

    for(int i = 0; i < (int)this->clientes.size(); i++) {
        printf("%02d. %s\n", i + 1,
            this->clientes.at(i)->getNome().c_str());
    }

    printf("\n");
}

void Banco::mostraContas() {
    /* Mostra todas as contas cadastradas no banco */
    printf("\nContas: \n");

    for(int i = 0; i < (int)this->contas.size(); i++) {
        printf("Conta ID: %05d \nDono da conta: %s\n\n",
            this->contas.at(i)->getID(),
            this->contas.at(i)->getCliente()->getNome().c_str());
    }

    printf("\n");
}

void Banco::mostraSaldo(int id) throw (entradaInvalida) {
    /* Mostra o saldo de uma conta especifica 
       emite um erro caso seja inserido um ID de conta errado */
    Conta *aux;

    aux = this->testaEntrada(id);

    // Mostra o saldo da conta
    printf("\nConta ID: %05d \nSaldo: %lf\n\n",
        aux->getID(),
        aux->getSaldo()/100.00);
}

void Banco::mostraExtrato(int id) throw (entradaInvalida) {
    /* Mostra todas as transferências de uma conta */
    Conta *aux;
    std::vector<Transferencia*> transferencias;

    aux = testaEntrada(id);
    transferencias = aux->getTransferencias();

    // Mostra as transferencias
    for(int i = 0; i < (int)transferencias.size(); i++) {
        printf("\nTransferencia %d\n", i);

        // Verifica se recebeu ou enviou
        if(transferencias.at(i)->getIdEm() == aux->getID()) {
            printf("Destinatario: %05d\n", transferencias.at(i)->getIdDes());
        }

        if(transferencias.at(i)->getIdDes() == aux->getID()) {
            printf("Emissario: %05d\n", transferencias.at(i)->getIdEm());
        }

        printf("Valor transferido: %lf\n\n", transferencias.at(i)->getValor()/100.00);
    } 
}

void Banco::transferencia(int id_em, int id_des, int valor) throw (entradaInvalida, totalIncorreto, saldoInsuficiente) {
    /* Envia dinheiro do emissario pro destinatário e registra o valor na conta */
    Conta *em;
    Conta *des;

    em  = this->testaEntrada(id_em);
    des = this->testaEntrada(id_des);

    // Validação do balanço total
    this->testaTotal();

    // Verifica se o emissário tem saldo suficiente
    if(em->getSaldo() < valor) throw saldoInsuficiente();

    des->setSaldo(des->getSaldo() + valor);
    em->setSaldo(em->getSaldo()   - valor);


    // Validação do balanço total
    this->testaTotal();

    // Registra a transferencia
    em->setTransferencia(id_em, id_des, valor);
    des->setTransferencia(id_em, id_des, valor);
}  

void Banco::mostraTotal() throw (totalIncorreto) {
    /* Mostra o saldo de todas as contas do banco */
    this->testaTotal();
    printf("\nBalanco geral: %lf\n", this->total/100.00);

    for(int i = 0; i < (int)this->contas.size(); i++) {
        mostraSaldo(this->contas.at(i)->getID());
    }
}