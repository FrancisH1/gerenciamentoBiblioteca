#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_LIVROS_EMPRESTADOS 5
#define MULTA 0.50

typedef struct{
    int codigo;
    char nome[30], cargo[30];
}Funcionarios;

typedef struct{
    int codigo;
    char titulo[30], autor[30], genero[30], status[30];
    int num_reservas, qtd_total;
}Livro;

typedef struct{
    int codigo;
    char nome[30], endereco[30];
    int qtd_emprestados, hist_multas;
}Leitor;

typedef struct{
    int codigo, cod_livro, cod_leitor;
    char data_emp[20], data_devp[15], status[15];
}Emprestimos;

typedef struct{
    int codigo, codigo_livro, codigo_leitor;
    char data_reserva[20];
}Reserva;


void reservarLivro(Leitor *pLeitor, Livro *pLivro, Reserva *pReserva){
    int i, qtd_leitores, qtd_livros, qtd_reservas = 0;
    int dia_sistema, mes_sistema, ano_sistema;

    time_t agora = time(NULL);
    struct tm *data = localtime(&agora);
    dia_sistema = data->tm_mday;
    mes_sistema = data->tm_mon + 1;
    ano_sistema = data->tm_year + 1900;
    pReserva->codigo = qtd_reservas+1;
    pReserva->codigo_livro = pLivro->codigo;
    pReserva->codigo_leitor = pLeitor->codigo;
    sprintf(pReserva->data_reserva, "%d-%d-%d", ano_sistema, mes_sistema, dia_sistema);
    
    FILE *arquivo_reserva = fopen("data/reservas.txt", "r");
    if (arquivo_reserva != NULL){    
        fscanf(arquivo_reserva, "%d\n", &qtd_reservas);
        Reserva *pReservaTemp = (Reserva *) malloc((qtd_reservas) * sizeof(Reserva));
        for (i=0; i<qtd_reservas; i++){
            fscanf(arquivo_reserva, "%d %d %d %s\n", &(pReservaTemp[i].codigo), &(pReservaTemp[i].codigo_livro), &(pReservaTemp[i].codigo_leitor), pReservaTemp[i].data_reserva);
        } // Grava as informações
        fclose(arquivo_reserva);

        arquivo_reserva = fopen("data/reservas.txt", "w");
        fprintf(arquivo_reserva, "%d\n", qtd_reservas+1); //aumenta o numero de reservas
        for (i=0; i<qtd_reservas; i++){
            fprintf(arquivo_reserva, "%d %d %d %s\n", pReservaTemp[i].codigo, pReservaTemp[i].codigo_livro, pReservaTemp[i].codigo_leitor, pReservaTemp[i].data_reserva);
        } // reescreve as informações
        free(pReservaTemp);
    }
    else{
        arquivo_reserva = fopen("data/reservas.txt", "w");
        fprintf(arquivo_reserva, "%d\n", qtd_reservas+1); //cria o arquivo se não existir
    }
    fprintf(arquivo_reserva, "%d %d %d %s\n", pReserva->codigo, pReserva->codigo_livro, pReserva->codigo_leitor, pReserva->data_reserva);
    fclose(arquivo_reserva);

    FILE *arquivo_livro = fopen("data/livros.txt", "r");
    if (arquivo_livro != NULL){
        fscanf(arquivo_livro, "%d\n", &qtd_livros);
        Livro *pLivroTemp = (Livro *) malloc((qtd_livros) * sizeof(Livro));
        for (i=0; i<qtd_livros; i++){
            fscanf(arquivo_livro, "%d %s %s %s %s %d %d", &(pLivroTemp[i].codigo), pLivroTemp[i].titulo, pLivroTemp[i].autor, pLivroTemp[i].genero, pLivroTemp[i].status, &(pLivroTemp[i].num_reservas), &(pLivroTemp[i].qtd_total));
        }
        fclose(arquivo_livro);
        strcpy(pLivroTemp[pLivro->codigo-1].status, "reservado");
        pLivroTemp[pLivro->codigo-1].num_reservas += 1;
        arquivo_livro = fopen("data/livros.txt", "w");
        fprintf(arquivo_livro, "%d\n", qtd_livros);
        for (i=0; i<qtd_livros; i++){
            fprintf(arquivo_livro, "%d %s %s %s %s %d %d\n", pLivroTemp[i].codigo, pLivroTemp[i].titulo, pLivroTemp[i].autor, pLivroTemp[i].genero, pLivroTemp[i].status, pLivroTemp[i].num_reservas, pLivroTemp[i].qtd_total);
        }
        free(pLivroTemp);
        fclose(arquivo_livro);
    }
    else{
        printf("Erro ao abrir o arquivo de livros.");
    }
    printf("Livro reservado com sucesso!\n");
}

void calcularMulta(Emprestimos *pEmpr, int cod_leitor){
    int i, qtd_leitores, qtd_empr;
    int dia, mes, ano, dia_sistema, mes_sistema, ano_sistema;
    
    FILE *arquivo_empr = fopen("data/emprestimos.txt", "r");
    if (arquivo_empr != NULL){
        fscanf(arquivo_empr, "%d\n", &qtd_empr);
        Emprestimos *pEmprTemp = (Emprestimos *) malloc((qtd_empr) * sizeof(Emprestimos));
        for (i=0; i<qtd_empr; i++){
            fscanf(arquivo_empr, "%d %d %d %s %s %s\n", &(pEmprTemp[i].codigo), &(pEmprTemp[i].cod_livro), &(pEmprTemp[i].cod_leitor), pEmprTemp[i].data_emp, pEmprTemp[i].data_devp, pEmprTemp[i].status);
        }
        fclose(arquivo_empr);
        
        sscanf(pEmprTemp[pEmpr->codigo-1].data_devp, "%d-%d-%d", &ano, &mes, &dia);
        printf("Data de devolucao: %d-%d-%d\n", ano, mes, dia);
        
        time_t agora = time(NULL);
        struct tm *data_sistema = localtime(&agora);
        ano_sistema = data_sistema->tm_year + 1900;
        mes_sistema = data_sistema->tm_mon + 1;
        dia_sistema = data_sistema->tm_mday;
        printf("Data atual: %d-%d-%d\n", ano_sistema, mes_sistema, dia_sistema);

        sscanf(asctime(data_sistema), "%d-%d-%d", &ano_sistema, &mes_sistema, &dia_sistema);
        if (ano_sistema > ano || (ano_sistema == ano && mes_sistema > mes) || (ano_sistema == ano && mes_sistema == mes && dia_sistema > dia)){
            int dias_atraso = (ano_sistema - ano)*365 + (mes_sistema - mes)*30 + (dia_sistema - dia);
            float multa = dias_atraso * MULTA;
            printf("O livro foi devolvido com %d dias de atraso.\n", dias_atraso);
            printf("Multa: R$ %.2f\n", multa);
        }
        else{
            printf("O livro foi devolvido no prazo.\n");
            return;
        }
        free(pEmprTemp);
    }
    else{
        printf("Arquivo nao encontrado.\n");
    }

    FILE *arquivo_leitor = fopen("data/leitores.txt", "r");
    if (arquivo_leitor != NULL){
        fscanf(arquivo_leitor, "%d\n", &qtd_leitores);
        Leitor *pLeitorTemp = (Leitor *) malloc((qtd_leitores) * sizeof(Leitor));
        for (i=0; i<qtd_leitores; i++){
            fscanf(arquivo_leitor, "%d %s %s %d %d\n", &(pLeitorTemp[i].codigo), pLeitorTemp[i].nome, pLeitorTemp[i].endereco, &(pLeitorTemp[i].qtd_emprestados), &(pLeitorTemp[i].hist_multas));
        } //Grava as informações
        fclose(arquivo_leitor);
        
        pLeitorTemp[cod_leitor-1].hist_multas += 1; // aumenta o numero de multas do leitor
        
        arquivo_leitor = fopen("data/leitores.txt", "w");
        fprintf(arquivo_leitor, "%d\n", qtd_leitores);
        for (i=0; i<qtd_leitores; i++){
            fprintf(arquivo_leitor, "%d %s %s %d %d\n", pLeitorTemp[i].codigo, pLeitorTemp[i].nome, pLeitorTemp[i].endereco, pLeitorTemp[i].qtd_emprestados, pLeitorTemp[i].hist_multas);
        } //reescreve as informações
        free(pLeitorTemp);
        fclose(arquivo_leitor);
    }
    else{
        printf("Arquivo nao encontrado.\n");
    }
}

void realizarDevolucao(Emprestimos *pEmpr){
    int i, qtd_leitores, qtd_livros, qtd_empr, cod_livro, cod_leitor;
    
    FILE *arquivo_empr = fopen("data/emprestimos.txt", "r");
    if (arquivo_empr != NULL)
    {
        fscanf(arquivo_empr, "%d\n", &qtd_empr);
        Emprestimos *pEmprTemp = (Emprestimos *) malloc ((qtd_empr) * sizeof(Emprestimos));
        for (i=0; i < qtd_empr; i++)
        {
            fscanf(arquivo_empr, "%d %d %d %s %s %s\n", &(pEmprTemp[i].codigo), &(pEmprTemp[i].cod_livro), &(pEmprTemp[i].cod_leitor), pEmprTemp[i].data_emp, pEmprTemp[i].data_devp, pEmprTemp[i].status);
        } // grava as informações
        strcpy(pEmprTemp[pEmpr->codigo-1].status, "finalizado"); // finaliza o emprestimo
        cod_livro = pEmprTemp[pEmpr->codigo-1].cod_livro;
        cod_leitor = pEmprTemp[pEmpr->codigo-1].cod_leitor;
        fclose(arquivo_empr);
        
        arquivo_empr = fopen("data/emprestimos.txt", "w");
        fprintf(arquivo_empr, "%d\n", qtd_empr);
        for (i=0; i < qtd_empr; i++)
        {
            fprintf(arquivo_empr, "%d %d %d %s %s %s\n", pEmprTemp[i].codigo, pEmprTemp[i].cod_livro, pEmprTemp[i].cod_leitor, pEmprTemp[i].data_emp, pEmprTemp[i].data_devp, pEmprTemp[i].status);
        } // reescreve as informações
        fclose(arquivo_empr);
        free(pEmprTemp);
    }   
    else{
        printf("Falha ao abrir o arquivo de emprestimos.");
        return;
    }
    
    FILE *arquivo_leitor = fopen("data/leitores.txt", "r");
    if (arquivo_leitor != NULL)
    {
        fscanf(arquivo_leitor, "%d\n", &qtd_leitores);
            
        Leitor *pLeitorTemp = (Leitor *) malloc((qtd_leitores) * sizeof(Leitor));
        for (i=0; i<qtd_leitores; i++)
        {
            fscanf(arquivo_leitor, "%d %s %s %d %d\n", &(pLeitorTemp[i].codigo), pLeitorTemp[i].nome, pLeitorTemp[i].endereco, &(pLeitorTemp[i].qtd_emprestados), &(pLeitorTemp[i].hist_multas));
        } 
        fclose(arquivo_leitor); //Grava as informações

        arquivo_leitor = fopen("data/leitores.txt","w");
        fprintf(arquivo_leitor, "%d\n", qtd_leitores);
        pLeitorTemp[cod_leitor-1].qtd_emprestados -= 1; // diminui o numero de emprestimos do leitor
        for (i=0; i<qtd_leitores; i++)
        {
            fprintf(arquivo_leitor, "%d %s %s %d %d\n", pLeitorTemp[i].codigo, pLeitorTemp[i].nome, pLeitorTemp[i].endereco, pLeitorTemp[i].qtd_emprestados, pLeitorTemp[i].hist_multas);
        }  // reescreve as informações
        free(pLeitorTemp);
    }
    fclose(arquivo_leitor);
    
    FILE *arquivo_livro = fopen("data/livros.txt","r");
    if (arquivo_livro != NULL)
    {
        fscanf(arquivo_livro, "%d\n", &qtd_livros);
        Livro *pLivroTemp = (Livro *)malloc(qtd_livros * sizeof(Livro));
        for (i=0; i<qtd_livros; i++){
            fscanf(arquivo_livro, "%d %s %s %s %s %d %d\n", &(pLivroTemp[i].codigo), pLivroTemp[i].titulo, pLivroTemp[i].autor, pLivroTemp[i].genero, pLivroTemp[i].status, &(pLivroTemp[i].num_reservas), &(pLivroTemp[i].qtd_total));
        } // grava as informações
        fclose(arquivo_livro);

        arquivo_livro = fopen("data/livros.txt", "w");
        pLivroTemp[cod_livro-1].qtd_total += 1; // aumenta a quantidade de livros disponiveis
        strcpy(pLivroTemp[cod_livro-1].status, "disponivel");
        fprintf(arquivo_livro, "%d\n", qtd_livros);
        for(i=0; i<qtd_livros; i++)
        {
            fprintf(arquivo_livro, "%d %s %s %s %s %d %d\n", pLivroTemp[i].codigo, pLivroTemp[i].titulo, pLivroTemp[i].autor, pLivroTemp[i].genero, pLivroTemp[i].status, pLivroTemp[i].num_reservas, pLivroTemp[i].qtd_total);    
        } // reescreve as informações
        free(pLivroTemp);
    }
    fclose(arquivo_livro);
  
    printf("Livro devolvido com sucesso.\n");
    calcularMulta(pEmpr, cod_leitor);
}

void realizarEmprestimo(Emprestimos *pEmpr, Leitor *pLeitor, Livro *pLivro){
    int i, qtd_leitores, qtd_livros, qtd_empr;

    FILE *arquivo_leitor = fopen("data/leitores.txt", "r");
    if (arquivo_leitor != NULL)
    {
        fscanf(arquivo_leitor, "%d\n", &qtd_leitores);
        if(pLeitor->codigo > qtd_leitores){
            printf("Esse leitor nao esta cadastrado.\n");
            return;
        }
        Leitor *pLeitorTemp = (Leitor *) malloc((qtd_leitores) * sizeof(Leitor));
        for (i=0; i<qtd_leitores; i++){
            fscanf(arquivo_leitor, "%d %s %s %d %d\n", &(pLeitorTemp[i].codigo), pLeitorTemp[i].nome, pLeitorTemp[i].endereco, &(pLeitorTemp[i].qtd_emprestados), &(pLeitorTemp[i].hist_multas));
        } //Grava as informações
        if (pLeitorTemp[pLeitor->codigo-1].qtd_emprestados >= MAX_LIVROS_EMPRESTADOS){
            fclose(arquivo_leitor);
            free(pLeitorTemp);
            printf("Numero maximo de emprestimos atingido.\n"); //Verifica se o leitor atingiu o limite de emprestimos
            return;
        }
        else{
            fclose(arquivo_leitor);
            arquivo_leitor = fopen("data/leitores.txt", "w");
            fprintf(arquivo_leitor, "%d\n", qtd_leitores);
            pLeitorTemp[pLeitor->codigo-1].qtd_emprestados += 1; // aumenta o numero de emprestimos do leitor
            for (i=0; i<qtd_leitores; i++)
            {
                fprintf(arquivo_leitor, "%d %s %s %d %d\n", pLeitorTemp[i].codigo, pLeitorTemp[i].nome, pLeitorTemp[i].endereco, pLeitorTemp[i].qtd_emprestados, pLeitorTemp[i].hist_multas);
            } //Reescreve as informações com o novo numero de emprestimos}
        }
        fclose(arquivo_leitor);
        free(pLeitorTemp);
    }
    else{
        printf("Falha ao abrir o arquivo de leitores\n");
        return;
    }

    FILE *arquivo_livro = fopen("data/livros.txt", "r");
    if (arquivo_livro != NULL)
    {
        fscanf(arquivo_livro, "%d\n", &qtd_livros);
        if(pLivro->codigo > qtd_livros){
            printf("Esse livro nao esta cadastrado.\n");
            return;
        }
        Livro *pLivroTemp = (Livro *) malloc((qtd_livros) * sizeof(Livro));
        for (i=0; i<qtd_livros; i++){
            fscanf(arquivo_livro, "%d %s %s %s %s %d %d\n", &(pLivroTemp[i].codigo), pLivroTemp[i].titulo, pLivroTemp[i].autor, pLivroTemp[i].genero, pLivroTemp[i].status, &(pLivroTemp[i].num_reservas), &(pLivroTemp[i].qtd_total));
        } //Grava as informações
        if (pLivroTemp[pLivro->codigo-1].qtd_total == 0){
            fclose(arquivo_livro);
            free(pLivroTemp);
            printf("Nao ha mais exemplares deste livro. Será feita uma reserva.\n");
            Reserva reserva, *pReserva = &reserva;
            reservarLivro(pLeitor, pLivro, pReserva);
            return;
        }
        else{
            fclose(arquivo_livro);
            arquivo_livro = fopen("data/livros.txt", "w");
            fprintf(arquivo_livro, "%d\n", qtd_livros);
            pLivroTemp[pLivro->codigo-1].qtd_total -= 1; // diminui o numero de exemplares
            for (i=0; i<qtd_livros; i++)
            {
                fprintf(arquivo_livro, "%d %s %s %s %s %d %d\n", pLivroTemp[i].codigo, pLivroTemp[i].titulo, pLivroTemp[i].autor, pLivroTemp[i].genero, pLivroTemp[i].status, pLivroTemp[i].num_reservas, pLivroTemp[i].qtd_total);
            } //Reescreve as informações com o novo numero de exemplares
        }
        fclose(arquivo_livro);
        free(pLivroTemp);
    }
    else{
        printf("Falha ao abrir o arquivo de livros\n");
        return;
    }

    time_t agora = time(NULL);
    struct tm *data_sistema = localtime(&agora);
    int ano_sistema = data_sistema->tm_year + 1900, ano_dev;
    int mes_sistema = data_sistema->tm_mon + 1, mes_dev;
    int dia_sistema = data_sistema->tm_mday, dia_dev;
    if (dia_sistema > 23){
        mes_dev = mes_sistema + 1;
        if (mes_dev > 12){
            mes_dev = 1;
            ano_dev = ano_sistema + 1;
        }
        dia_dev = dia_sistema - 23;
    }
    else{
        dia_dev = dia_sistema + 7;
        mes_dev = mes_sistema;
        ano_dev = ano_sistema;
    }
    
    FILE *arquivo_empr = fopen("data/emprestimos.txt", "r");
    if (arquivo_empr != NULL)
    {
        fscanf(arquivo_empr, "%d\n", &qtd_empr);
        Emprestimos *pEmprTemp = (Emprestimos *) malloc((qtd_empr) * sizeof(Emprestimos));
        for (i = 0; i < qtd_empr; i++)
        {
            fscanf(arquivo_empr, "%d %d %d %s %s %s\n", &(pEmprTemp[i].codigo), &(pEmprTemp[i].cod_livro), &(pEmprTemp[i].cod_leitor), pEmprTemp[i].data_emp, pEmprTemp[i].data_devp, pEmprTemp[i].status);
        }   //le as informações existentes
        fclose(arquivo_empr);

        pEmpr->codigo = qtd_empr + 1;
        pEmpr->cod_livro = pLivro->codigo;
        pEmpr->cod_leitor = pLeitor->codigo;
        sprintf(pEmpr->data_emp, "%d-%d-%d", ano_sistema, mes_sistema, dia_sistema);
        sprintf(pEmpr->data_devp, "%d-%d-%d", ano_dev, mes_dev, dia_dev);
        strcpy(pEmpr->status, "ativo");
    
        arquivo_empr = fopen("data/emprestimos.txt", "w");
        fprintf(arquivo_empr, "%d\n", qtd_empr + 1); //aumenta o tamanho do vetor
        for (i = 0; i < qtd_empr; i++)
        {
            fprintf(arquivo_empr, "%d %d %d %s %s %s\n", pEmprTemp[i].codigo, pEmprTemp[i].cod_livro, pEmprTemp[i].cod_leitor, pEmprTemp[i].data_emp, pEmprTemp[i].data_devp, pEmprTemp[i].status);
        }   //reescreve as informações
        free(pEmprTemp);
    }
    else
    {
        arquivo_empr = fopen("data/emprestimos.txt", "w"); //cria o arquivo se não existir
        qtd_empr = 1;
        fprintf(arquivo_empr, "%d", qtd_empr); 
    }
    printf("Emprestimo realizado com sucesso!\n");
    fprintf(arquivo_empr, "%d %d %d %s %s %s", pEmpr->codigo, pEmpr->cod_livro, pEmpr->cod_leitor, pEmpr->data_emp, pEmpr->data_devp, pEmpr->status); //adiciona o novo emprestimo
    fclose(arquivo_empr);
}

void cadastrarLivro(Livro *pLivro)
{
    int i, qtd_livro;
    FILE *arquivo = fopen("data/livros.txt", "r");
    if (arquivo != NULL)
    {
        fscanf(arquivo, "%d\n", &qtd_livro);
        Livro *pLivroTemp = (Livro *) malloc((qtd_livro) * sizeof(Livro));
        for (i = 0; i < qtd_livro; i++)
        {
            fscanf(arquivo, "%d %s %s %s %s %d %d\n", &(pLivroTemp[i].codigo), pLivroTemp[i].titulo, pLivroTemp[i].autor, pLivroTemp[i].genero, pLivroTemp[i].status, &(pLivroTemp[i].num_reservas), &(pLivroTemp[i].qtd_total));
        }   //le as informações existentes
        fclose(arquivo);

        arquivo = fopen("data/livros.txt", "w");
        fprintf(arquivo, "%d\n", qtd_livro + 1); //aumenta o tamanho do vetor
        for (i = 0; i < qtd_livro; i++)
        {
            fprintf(arquivo, "%d %s %s %s %s %d %d\n", pLivroTemp[i].codigo, pLivroTemp[i].titulo, pLivroTemp[i].autor, pLivroTemp[i].genero, pLivroTemp[i].status, pLivroTemp[i].num_reservas, pLivroTemp[i].qtd_total);
        }   //reescreve as informações
        free(pLivroTemp);
    }
    else
    {
        arquivo = fopen("data/livros.txt", "w"); //cria o arquivo se não existir
        qtd_livro = 1;
        fprintf(arquivo, "%d", qtd_livro);
    }
    pLivro->codigo = qtd_livro+1, pLivro->num_reservas = 0;
    fprintf(arquivo, "%d %s %s %s %s %d %d", pLivro->codigo, pLivro->titulo, pLivro->autor, pLivro->genero, pLivro->status, pLivro->num_reservas, pLivro->qtd_total); //adiciona o novo livro
    fclose(arquivo);
}

void cadastrarLeitor(Leitor *pLeitor)
{
    int i, qtd_leitor;
    FILE *arquivo = fopen("data/leitores.txt", "r");
    if (arquivo != NULL) //verifica se o arquivo existe
    {
        fscanf(arquivo, "%d\n", &qtd_leitor);
        Leitor *pLeitorTemp = (Leitor *) malloc((qtd_leitor) * sizeof(Leitor));
        for (i = 0; i < qtd_leitor; i++)
        {
            fscanf(arquivo, "%d %s %s %d %d\n", &(pLeitorTemp[i].codigo), pLeitorTemp[i].nome, pLeitorTemp[i].endereco, &(pLeitorTemp[i].qtd_emprestados), &(pLeitorTemp[i].hist_multas));
        }   //le as informações existentes
        fclose(arquivo);
        
        arquivo = fopen("data/leitores.txt", "w");
        fprintf(arquivo, "%d\n", qtd_leitor + 1); //aumenta o tamanho do vetor
        for (i = 0; i < qtd_leitor; i++)
        {
            fprintf(arquivo, "%d %s %s %d %d\n", pLeitorTemp[i].codigo, pLeitorTemp[i].nome, pLeitorTemp[i].endereco, pLeitorTemp[i].qtd_emprestados, pLeitorTemp[i].hist_multas);
        }   //reescreve as informações
        free(pLeitorTemp);
    }
    else
    {
        arquivo = fopen("data/leitores.txt", "w"); //cria o arquivo se não existir
        qtd_leitor = 1;
        fprintf(arquivo, "%d", qtd_leitor);
    }
    pLeitor->codigo = qtd_leitor+1, pLeitor->qtd_emprestados = 0, pLeitor->hist_multas = 0;
    fprintf(arquivo, "%d %s %s %d %d", pLeitor->codigo, pLeitor->nome, pLeitor->endereco, pLeitor->qtd_emprestados, pLeitor->hist_multas); //adiciona o novo leitor
    fclose(arquivo);
}

void incluirFuncionarios(Funcionarios *pFun)
{
    int i, qtd_func;
    FILE *arquivo = fopen("data/funcionarios.txt", "r");
    if (arquivo != NULL) //verifica se o arquivo existe
    {
        fscanf(arquivo, "%d\n", &qtd_func);
        Funcionarios *pFuncTemp = (Funcionarios *) malloc((qtd_func) * sizeof(Funcionarios));
        for (i = 0; i < qtd_func; i++)
        {
            fscanf(arquivo, "%d %s %s\n", &(pFuncTemp[i].codigo), pFuncTemp[i].nome, pFuncTemp[i].cargo);
        }   //le as informações existentes
        fclose(arquivo);
        
        arquivo = fopen("data/funcionarios.txt", "w");
        fprintf(arquivo, "%d\n", qtd_func + 1); //aumenta o tamanho do vetor
        for (i = 0; i < qtd_func; i++)
        {
            fprintf(arquivo, "%d %s %s\n", pFuncTemp[i].codigo, pFuncTemp[i].nome, pFuncTemp[i].cargo);
        }   //reescreve as informações
        free(pFuncTemp);
    }
    else
    {
        arquivo = fopen("data/funcionarios.txt", "w"); //cria o arquivo se não existir
        qtd_func = 1;
        fprintf(arquivo, "%d", qtd_func);
    }
    pFun->codigo = qtd_func+1;
    fprintf(arquivo, "%d %s %s", pFun->codigo, pFun->nome, pFun->cargo); //adiciona o novo funcionario
    fclose(arquivo);
}

void RelatorioLeitores(){
    int i, qtd_leitores;
    FILE *arquivo = fopen("data/leitores.txt", "r");
    if (arquivo == NULL){ //verifica se o arquivo existe
        printf("Arquivo nao encontrado\n");
        return;
    }
    fscanf(arquivo, "%d\n", &qtd_leitores);  
    Leitor *pLeitorTemp = (Leitor *) malloc((qtd_leitores) * sizeof(Leitor));
    for (i = 0; i < qtd_leitores; i++){
        fscanf(arquivo, "%d %s %s %d %d\n", &(pLeitorTemp[i].codigo), pLeitorTemp[i].nome, pLeitorTemp[i].endereco, &(pLeitorTemp[i].qtd_emprestados), &(pLeitorTemp[i].hist_multas));
    }   //le as informações existentes
    fclose(arquivo);

    printf("Relatorio de leitores\n");
    for (i = 0; i < qtd_leitores; i++){
        if (pLeitorTemp[i].qtd_emprestados > 0 || pLeitorTemp[i].hist_multas > 0){
            printf("Codigo: %d\n", pLeitorTemp[i].codigo);
            printf("Nome: %s\n", pLeitorTemp[i].nome);
            printf("Endereco: %s\n", pLeitorTemp->endereco);
            printf("Quantidade de emprestimos ativos: %d\n", pLeitorTemp[i].qtd_emprestados);
            printf("Quantidade de multas: %d\n", pLeitorTemp[i].hist_multas);
        }
    }
    free(pLeitorTemp);
}

void RelatorioLivros(){
    int i, qtd_livros, qtd_emprestimos;
    FILE *arquivo = fopen("data/livros.txt", "r");
    if (arquivo == NULL){ //verifica se o arquivo existe
        printf("Arquivo nao encontrado\n");
        return;
    }
    fscanf(arquivo, "%d\n", &qtd_livros);  
    Livro *pLivroTemp = (Livro *) malloc((qtd_livros) * sizeof(Livro));
    for (i = 0; i < qtd_livros; i++){
        fscanf(arquivo, "%d %s %s %s %s %d %d\n", &(pLivroTemp[i].codigo), pLivroTemp[i].titulo, pLivroTemp[i].autor, pLivroTemp[i].genero, pLivroTemp[i].status, &(pLivroTemp[i].num_reservas), &(pLivroTemp[i].qtd_total));
    }   //le as informações existentes
    fclose(arquivo);

    FILE *arquivo_emprestimos = fopen("data/emprestimos.txt", "r");
    if (arquivo_emprestimos == NULL){ //verifica se o arquivo existe
        printf("Arquivo nao encontrado\n");
        return;
    }
    fscanf(arquivo_emprestimos , "%d\n", &qtd_emprestimos);  
    Emprestimos *pEmprTemp = (Emprestimos *) malloc((qtd_emprestimos) * sizeof(Emprestimos));
    for (i = 0; i < qtd_emprestimos; i++){
        fscanf(arquivo_emprestimos, "%d %d %d %s %s %s\n", &(pEmprTemp[i].codigo), &(pEmprTemp[i].cod_livro), &(pEmprTemp[i].cod_leitor), pEmprTemp[i].data_emp, pEmprTemp[i].data_devp, pEmprTemp[i].status);
    }   //le as informações existentes
    fclose(arquivo_emprestimos);

    printf("Relatorio de livros disponiveis e reservados\n");
    for (i = 0; i < qtd_livros; i++){
        if (strcmp(pLivroTemp[i].status, "disponivel") == 0){
            printf("Codigo: %d\n", pLivroTemp[i].codigo);
            printf("Titulo: %s\n", pLivroTemp[i].titulo);
            printf("Status: Disponivel\n");
        }
        if (pEmprTemp[i].cod_livro == pLivroTemp[i].codigo && strcmp(pLivroTemp[i].status, "reservado") == 0){
            printf("Codigo: %d\n", pLivroTemp[i].codigo);
            printf("Titulo: %s\n", pLivroTemp[i].titulo);
            printf("Status: Reservado\n");
        }
    }
    free(pLivroTemp);
    free(pEmprTemp);
}

void limparEmprestimos(Emprestimos *emprestimo) {
    emprestimo->codigo = 0;
    emprestimo->cod_livro = 0;
    emprestimo->cod_leitor = 0;
    strcpy(emprestimo->data_emp, "");
    strcpy(emprestimo->data_devp, "");
    strcpy(emprestimo->status, "");
}

void limparLivro(Livro *livro) {
    livro->codigo = 0;
    strcpy(livro->titulo, "");
    strcpy(livro->autor, "");
    strcpy(livro->genero, "");
    strcpy(livro->status, "");
    livro->num_reservas = 0;
    livro->qtd_total = 0;
}

void limparLeitor(Leitor *leitor) {
    leitor->codigo = 0;
    strcpy(leitor->nome, "");
    strcpy(leitor->endereco, "");
    leitor->qtd_emprestados = 0;
    leitor->hist_multas = 0;
}

void limparFuncionario(Funcionarios *funcionario) {
    funcionario->codigo = 0;
    strcpy(funcionario->nome, "");
    strcpy(funcionario->cargo, "");
}

void limparTodos(Emprestimos *emprestimo, Livro *livro, Leitor *leitor, Funcionarios *funcionario) {
    limparEmprestimos(emprestimo);
    limparLivro(livro);
    limparLeitor(leitor);
    limparFuncionario(funcionario);
}