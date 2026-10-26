#include <stdio.h>
#include <stdlib.h>
#include "funcoes.c"

int main(){
    int op0 = 0, op1, op2, op3, op4, senhaAdm = 12345, senha;
    Livro livro1, *pLivro = &livro1;
    Leitor leitor1, *pLeitor = &leitor1;
    Emprestimos emprestimos1, *pEmprestimos = &emprestimos1;
    Funcionarios funcionario1, *pFuncionario = &funcionario1;

    while (op0 != 3){
        Inicio:
        printf("Seja bem vindo! Selecione o metodo de login\n1-Operador\n2-Administrador\n3-Sair\n");
        scanf("%d", &op0);
        switch(op0)
        {
            case 1:
                op1 = 0;
                while (op1 != 3)
                {
                    Operador:
                    printf("O que deseja fazer?\n1-Emprestimo\n2-Devolucao\n3-Voltar\n");
                    scanf("%d", &op1);
                    switch (op1)
                    {
                        case 1:
                            limparTodos(pEmprestimos, pLivro, pLeitor, pFuncionario);
                            printf("Digite o codigo do livro: ");
                            scanf("%d", &(pLivro->codigo));
                            printf("Digite o codigo do leitor: ");
                            scanf("%d", &(pLeitor->codigo));
                            realizarEmprestimo(pEmprestimos, pLeitor, pLivro);
                            goto Operador;
                        case 2:
                            limparTodos(pEmprestimos, pLivro, pLeitor, pFuncionario);
                            printf("Digite o codigo do emprestimo: ");
                            scanf("%d", &(pEmprestimos->codigo));
                            realizarDevolucao(pEmprestimos);
                            goto Operador;
                        case 3:
                            goto Inicio;
                        default:
                            printf("Opcao invalida\n");
                            goto Operador;
                    }
                }
                break;
            case 2:
                op2 = 0;
                printf("Digite a senha: ");
                scanf("%d", &senha);
                if (senha == senhaAdm)
                {
                    op2 = 0;
                    printf("Acesso Liberado\nO que deseja fazer?\n");
                    while (op2 != 6){
                        Administrador:
                        printf("1-Emprestimo\n2-Devolucao\n3-Cadastros\n4-Relatorios\n5-Voltar\n");
                        scanf("%d", &op2);
                        switch(op2)
                        {
                            case 1:
                                limparTodos(pEmprestimos, pLivro, pLeitor, pFuncionario);
                                printf("Digite o codigo do livro: ");
                                scanf("%d", &(pLivro->codigo));
                                printf("Digite o codigo do leitor: ");
                                scanf("%d", &(pLeitor->codigo));
                                realizarEmprestimo(pEmprestimos, pLeitor, pLivro);
                                goto Administrador;
                            case 2:
                                limparTodos(pEmprestimos, pLivro, pLeitor, pFuncionario);
                                printf("Digite o codigo do emprestimo: ");
                                scanf("%d", &(pEmprestimos->codigo));
                                realizarDevolucao(pEmprestimos);
                                goto Administrador;
                            case 3:
                                op3 = 0;
                                while (op3 != 4)
                                {
                                    Cadastros: 
                                    printf("1-Cadastrar livro\n2-Cadastrar leitor\n3-Cadastrar funcionario\n4-Voltar\n");
                                    scanf("%d",&op3);
                                    switch(op3)
                                    {
                                        case 1:
                                            limparLivro(pLivro);
                                            printf("Digite o titulo do livro: ");
                                            scanf("%s", pLivro->titulo);
                                            printf("Digite o nome do autor: ");
                                            scanf("%s", pLivro->autor);
                                            printf("Digite o genero do livro: ");
                                            scanf("%s", pLivro->genero);
                                            printf("Digite o status do livro: ");
                                            scanf("%s", pLivro->status);
                                            printf("Quantos desse livro voce deseja cadastrar: ");
                                            scanf("%d", &pLivro->qtd_total);
                                            cadastrarLivro(pLivro);
                                            goto Cadastros;
                                        case 2:
                                            limparLeitor(pLeitor);
                                            printf("Digite o nome do leitor: ");
                                            scanf("%s", pLeitor->nome);
                                            printf("Digite seu endereco: ");
                                            scanf("%s", pLeitor->endereco);
                                            pLeitor->qtd_emprestados = 0;
                                            pLeitor->hist_multas = 0;
                                            cadastrarLeitor(pLeitor);
                                            goto Cadastros;
                                        case 3:
                                            limparFuncionario(pFuncionario);
                                            printf("Digite o nome do funcionario: ");
                                            scanf("%s", pFuncionario->nome);
                                            printf("Digite o cargo do funcionario: ");
                                            scanf("%s", pFuncionario->cargo);
                                            incluirFuncionarios(pFuncionario);
                                            break;
                                        case 4:
                                            goto Administrador;
                                        default:
                                            printf("Opcao invalida!\n");
                                            goto Cadastros;
                                    }                                                                         
                                }
                                goto Administrador;
                            case 4:
                                Relatorios:
                                printf("1-Livros Disponiveis e Reservados\n2-Leitores com emprestimos e multas\n3-Voltar\n");
                                scanf("%d", &op4);
                                switch(op4)
                                {
                                    case 1:
                                        RelatorioLivros();
                                        goto Relatorios;
                                    case 2:
                                        RelatorioLeitores();
                                        goto Relatorios;
                                    case 3:
                                        goto Administrador;
                                    default:
                                        printf("Opcao invalida!\n");
                                        goto Relatorios;
                                }
                                goto Administrador;
                            case 5:
                                goto Inicio;
                            default:
                                printf("Opcao invalida!\n");
                                goto Administrador;
                        }   
                    }
                }
                else
                {
                    printf("Senha invalida! Acesso Negado\n");
                    goto Inicio;
                }
                break;
            case 3:
                printf("Finalizando o programa\n");
                return 1;
            default:
                printf("Opcao invalida!\n");
                break;
        }
    }
    return 0;
}