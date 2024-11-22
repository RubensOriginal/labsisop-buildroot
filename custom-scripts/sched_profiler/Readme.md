O programa sched_profiler avalia o desempenho dos sistemas de escalonamento no linux. Para isso, programa cria várias threads que escrevem caracteres em um buffer compartilhado.

Para utilizar o programa, siga os seguintes passos:

## Compilação

Para compilar o programa, utilize o comando:

```sh
gcc -o sched_profiler sched_profiler.c -lpthread
```

## Execução

Para executar o programa, utilize o comando:

```sh
./sched_profiler <tamanho_buffer> <num_threads> <policy>
```

- `<tamanho_buffer>`: Tamanho do buffer compartilhado.
- `<num_threads>`: Número de threads a serem criadas.
- `<policy>`: Política de escalonamento a ser utilizada (1 para SCHED_FIFO, 2 para SCHED_RR, 5 para SCHED_IDLE e 7 para SCHED_LOW_IDLE).

Exemplo de execução:

```sh
./sched_profiler 1000 4 1
```

## Saída

O programa exibirá a saída sem processamento e a saída com processamento, mostrando o número de vezes que o processador executou a thread. Exemplo de saída:

```sh
> sched_profiler 100 3 1

All threads ready!
Saída Sem Processamento:
CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAABBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBB

Saída Com Processamento:
CAB

A = 1
B = 1
C = 1
```
## Limpeza

Para limpar os arquivos gerados pela compilação, utilize o comando:

```sh
rm sched_profiler
```