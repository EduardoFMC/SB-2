# Projeto 2 (SB - 2022.2): Montador
Projeto 2 de Software Básico, disciplina ofertada na Universidade de Brasília (UnB) no semestre 2022.2. Desenvolvido por <a href="https://github.com/EduardoFMC">Eduardo Ferreira Marques Cavalcante</a> e <a href="https://github.com/ciriatico">Gabriel Mendes Ciriatico Guimarães</a>.

## Descrição ##
O trabalho implementa um tradutor em C++ da linguagem hipotética vista em aula para código em IA-32 (padrão INTEL). A etapa de pré-processamento do montador da linguagem hipotética é mantida.

## Especificações ##
O sistema operacional utilizado no desenvolvimento do projeto foi Ubuntu 22.04.1 LTS.

## Execução ##

### Compilar o montador ###

É necessário compilar o montador que traduzirá o código de linguagem hipotética para IA-32.

<code>g++ -std=c++17 -o montador main.cpp</code>

### Traduzir Assembly hipotético ###

Para traduzir o código em Assembly hipotético, é preciso passar o nome do arquivo .asm na chamada do montador. O código em IA-32 será escrito com o mesmo nome, com extensão .S.

<code>./montador filename.asm</code>

### Compilar o código em IA-32 ###

Com o arquivo objeto traduzido, obtemos o filename.S, que pode ser compilado via nasm.

<code>nasm -f elf -o output.o filename.S</code>

### Ligar o código Assembly ###

Agora, é preciso ligar o arquivo de saída.

<code>.ld -m elf_i386 output output.o</code>

### Executar o código ###

<code>./output</code>
