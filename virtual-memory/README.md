# UFSJ-SO-Virtual Memory
Aqui você encontrará um simulador capaz de lidar com tamanhos de página e memória variáveis, além de permitir testar diferentes políticas de substituição para fins de comparação. Com esta ferramenta, você poderá explorar os mecanismos fundamentais da memória virtual e avaliar o desempenho de diferentes estratégias de gerenciamento.

# Recursos
* Suporte a tamanhos de página e memória variáveis.
* Implementação das seguintes políticas de substituição:
  * Least Recently Used (LRU)
  * Not Recently Used (NRU)
  * Segunda Chance
* Implementação das seguintes métricas de avaliação:
  * Páginas lidas
  * Páginas escritas
  * Páginas sujas
  * Falta de página

# Como usar

1. Compile o código com o comando:
~~~
make
~~~

2. Execute o arquivo gerado com o comando:
~~~
./tp2virtual <politica> <entrada> <page-sz> <mem-sz>
~~~

3. Opções de política:
~~~
  - lru
  - nru
  - second-chance
~~~

