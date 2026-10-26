# Sistema de Biblioteca — Estrutura de Dados 1

Sistema de gerenciamento de biblioteca desenvolvido em C para a disciplina de Estrutura de Dados 1, na UTFPR. O programa permite operações de empréstimo e devolução, cadastros, reservas e relatórios, armazenando os registros em arquivos de texto.

## Estrutura

- `src/` — código-fonte em C.
- `data/` — arquivos de dados usados para armazenar livros, leitores, funcionários, empréstimos e reservas.
- `README.md` — instruções do projeto.

## Requisitos

- GCC ou outro compilador C compatível.

## Compilar e executar

Na pasta raiz do projeto, execute:

```bash
gcc src/biblioteca.c -o biblioteca
./biblioteca
```

No Windows, compile com:

```bash
gcc src/biblioteca.c -o biblioteca.exe
biblioteca.exe
```

Execute o programa a partir da pasta raiz para que os caminhos dos arquivos em `data/` sejam encontrados corretamente.

## Acesso administrativo

A senha de administrador definida no código é `12345`.

## Observação

Os arquivos em `data/` contêm dados de exemplo e podem ser alterados pelo programa durante o uso. Faça uma cópia deles antes de testar operações que modifiquem os registros.
