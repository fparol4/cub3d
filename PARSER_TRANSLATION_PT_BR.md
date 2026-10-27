# Guia da migração: `cub3d-parser` → `source/map`

> Nota: este guia registra a integração anterior. O componente agora está em
> `source/parser/`, com interface em `header/source/parser.h`; seus nomes
> atuais recuperam nomes originais como `parser_controller` e `valid_map`.

Este guia compara o código que está em `cub3d-parser/core/src/parser/` com a
implementação integrada em `source/map/`. O objetivo é localizar **cada função**,
entender seu novo nome ou sua substituição e saber o que mudou no comportamento.
As descrições se referem ao código atual destes diretórios; `PARSER.md` explica
o parser isolado em mais detalhes, mas não descreve toda a integração com o jogo.

## 1. Como ler os dois fluxos

### Antes: programa isolado

```text
cub3d-parser/core/main.c
  └─ parser_controller(path, &data)
       ├─ split_archive: abre e separa configurações e linhas do mapa
       ├─ valid_texture: confere a presença das seis configurações
       └─ valid_map: caracteres, jogador, fechamento e conectividade
  └─ free_data(&data), tanto em sucesso quanto em erro
```

`t_data` era o resultado final. O programa apenas dizia se o mapa era válido;
não precisava preparar posição, direção, cores ou grade para o motor gráfico.

### Agora: componente do jogo

```text
source/main.c → runtime_args → runtime_setup
  └─ map_parse(g, path)
       ├─ cria t_map_data zerado
       ├─ map_read_archive → map_parse_config → map_parse_color
       │                     └─ map_rectangularize
       ├─ map_validate → map_connectivity_validate
       └─ _map_transfer → _map_apply_player
  └─ setup_game → texture_load → janela e renderização
  └─ runtime_cleanup → map_cleanup
```

`t_map_data` é **temporário**. Só depois de validar tudo, `_map_transfer` move
seu `t_map` para `g->map` e prepara `g->player`. As funções do parser usam
principalmente `1` para sucesso e `0` para falha. A interface do jogo inverte
essa convenção: `map_parse` retorna `0` em sucesso e `1` em erro, pois usa
`common_error`. `map_connectivity_validate` também pode retornar `-1` se não
conseguir alocar memória; `map_validate` só aceita o valor `1`.

### Onde cada arquivo foi parar

| Arquivo antigo em `core/src/parser/` | Arquivo(s) novo(s) em `source/map/` |
| --- | --- |
| `parser_controller.c` | `map.c` |
| `read_archive.c` | `map_reader.c` e `map_grid.c` |
| `parse_config.c` | `map_config.c` e parte de `map_reader.c` |
| `parse_color.c` | `map_color.c` |
| `map_cells.c` | `map_cells.c` e `map_validate.c` |
| `valid_map.c` | `map_validate.c` |
| `valid_routes.c` | `map_validate.c` e `map.c` |
| `map_connectivity.c` | `map_connectivity.c` |
| `map_connectivity_utils.c` | `map_cells.c` e `map_connectivity.c` |

Fora dessa pasta, o `main` de `cub3d-parser/core/main.c` foi substituído pelo
fluxo `source/main.c` → `runtime_args` → `runtime_setup` → `map_parse`. O novo
`main` inicia o jogo após o parsing; o antigo apenas imprimia o resultado da
validação e liberava `t_data`.

## 2. Tradução das estruturas e dos campos

| No parser isolado | No jogo | O que mudou |
| --- | --- | --- |
| `t_data` | `t_map_data` durante o parsing; depois `t_map` + `t_player` em `t_game` | O estado temporário foi separado do estado usado pelo jogo. |
| `north_texture`, `south_texture`, `west_texture`, `east_texture` | `map.no_texture`, `map.so_texture`, `map.we_texture`, `map.ea_texture` | Os caminhos passam a pertencer a `t_map`. |
| `floor` e `ceiling` (`t_rgb` com `r`, `g`, `b`) | `map.color_floor` e `map.color_ceiling` (`int`) | As três componentes são empacotadas como `0xRRGGBB`. |
| `floor_set`, `ceiling_set` e ponteiros de textura usados como presença | `fields`, com bits `CUB_FL`, `CUB_CL`, `CUB_NO`, `CUB_SO`, `CUB_WE`, `CUB_EA` | Um único conjunto de bits registra as seis configurações e detecta duplicatas. |
| `map`, `map_width`, `map_height` | `map.grid`, `map.width`, `map.height` | A grade é normalizada para ter linhas da mesma largura. |
| `player_x`, `player_y`, `player_dir` em `t_data` | Mesmo nome em `t_map_data`; depois `g->player` | Coordenadas inteiras e letra existem só durante o parsing; o jogo recebe posição e vetores. |
| `t_flood` | `t_map_flood` | A matriz `visited[y][x]` vira um vetor plano; `tail` vira `size_t` e aparece `capacity`. |
| `t_point` do parser | `t_point` em `header/core/struct.h` | Mesmo conceito de coordenadas `x` e `y`, agora compartilhado pelo jogo. |
| Sem equivalente | `map_started` em `t_map_data` | Registra que a leitura já entrou na seção do mapa. |

Os tipos temporários e suas funções estão declarados em `header/source/map.h`.
`t_map`, `t_player` e `t_game` estão em `header/core/struct.h`. Os bits de
configuração e os valores `T_FLOOR`/`T_WALL` estão em `header/core/enum.h`.

## 3. Dicionário de **todas** as funções antigas

As funções com `_` no começo do novo nome são `static`: só podem ser chamadas
no próprio arquivo `.c`. Uma seta para mais de uma função indica que a
responsabilidade foi dividida; “incorporada” indica que não existe nova função
com a mesma fronteira.

### Controle, leitura e separação do arquivo

| Função antiga | Função nova ou destino | Tradução e diferença principal |
| --- | --- | --- |
| `parser_controller` | `map_parse` em `map.c` | Coordena leitura e validação; agora também faz a transferência ao jogo, limpa falhas e informa erro por `common_error`. |
| `split_archive` | `map_read_archive` em `map_reader.c` | Verifica extensão `.cub`, abre, lê e fecha o arquivo; agora chama `map_rectangularize` antes de validar. `get_next_line` virou `ft_get_next_line` da libft do projeto. |
| `map_line` | Incorporada em `_map_process_line` | Continua removendo apenas `\n` da linha de mapa, preservando os espaços; deixou de existir como função separada. |
| `append_map_line` | `_map_append_line` e `_map_resize_grid` | Adiciona linha, mantém `NULL` final, altura e maior largura. O `realloc` foi trocado por alocação, cópia dos ponteiros e liberação do vetor antigo; há verificações de limite. |
| `is_map_line` | `_map_is_start` + `map_started` em `_map_process_line` | Detecta o início do mapa. Aceita parede, chão ou jogador na primeira posição útil e impede que `NO`, `SO`, `WE` e `EA` sejam confundidos com linhas iniciadas por direção. |
| `process_line` | `_map_process_line` | Ignora linhas vazias antes do mapa, encaminha configuração ou linha de mapa e cuida da memória da linha. Depois que o mapa começa, todas as linhas seguintes pertencem a ele. |

### Configurações e cores

| Função antiga | Função nova ou destino | Tradução e diferença principal |
| --- | --- | --- |
| `trim_line` | Chamadas diretas a `ft_strtrim` em `_map_process_line` e `_map_parse_texture` | Não há helper próprio. A configuração também remove `\r`, além de espaço, tabulação e `\n`; os espaços do mapa original são preservados. |
| `texture_destination` | `_map_texture_field` + `_map_texture_slot` em `map_config.c` | Uma função identifica o bit `CUB_*`; a outra encontra o ponteiro de destino em `t_map`. |
| `parse_texture_config` | `_map_parse_texture` | Confere separador, duplicata via `fields`, caminho não vazio; grava o caminho e marca o bit correspondente. Não abre o arquivo XPM. |
| `parse_color_config` | `_map_parse_color_config` | Escolhe piso ou teto, rejeita duplicata via `fields` e chama `map_parse_color`; grava `int` em vez de `t_rgb`. |
| `parse_config` | `map_parse_config` | Despacha `NO/SO/WE/EA/F/C` e rejeita linhas desconhecidas. |
| `parse_rgb_value` | `_map_parse_component` + `_map_skip_space` em `map_color.c` | Lê um número decimal de `0` a `255`, tolerando espaços/tabulações em torno dele. O ponteiro avança no texto original. |
| `free_parts` | Sem equivalente | O novo parser não usa `ft_split` nem aloca um vetor de componentes RGB. |
| `parse_color` | `map_parse_color`, com `_map_parse_rgb` e `_map_parse_separator` | Exige exatamente três componentes separados por duas vírgulas e converte para `0xRRGGBB`. |
| `valid_texture` | Primeira condição de `map_validate` | Apesar do nome antigo, só verificava se quatro caminhos e duas cores foram informados. Agora essa presença é `(fields & CUB_REQ_FIELDS) == CUB_REQ_FIELDS`. A existência do XPM é tratada depois, em `texture_load`. |

### Células e validação da grade

| Função antiga | Função nova ou destino | Tradução e diferença principal |
| --- | --- | --- |
| `is_player` | `map_is_player` em `map_cells.c` | Reconhece `N`, `S`, `E` ou `W`; mudou sobretudo o prefixo do nome. |
| `is_walkable` | `map_is_walkable` | Reconhece `T_FLOOR` (`'0'`) ou uma letra de jogador. |
| `map_at` | `map_cell_at` | Continua devolvendo espaço para coordenadas fora da grade. Agora acessa diretamente `grid[y][x]`, pois todas as linhas foram preenchidas até `width`; também verifica ponteiros nulos. |
| `is_closed_cell` | `_map_cell_closed` em `map_validate.c` | Verifica os quatro vizinhos de cada célula caminhável; todos devem ser parede ou caminháveis, nunca vazio/fora do mapa. |
| `inspect_map_line` | `_map_inspect_line` | Rejeita caracteres desconhecidos e guarda a última posição/direção encontrada. Antes devolvia a quantidade de jogadores naquela linha ou `-1`; agora incrementa um contador recebido por ponteiro e devolve sucesso/falha. |
| `find_player` | `_map_player_count` | Percorre linhas, usa `_map_inspect_line` e devolve o total de jogadores, ou `-1` em caractere inválido. |
| `validate_closed_cells` | `_map_cells_closed` | Percorre toda a grade e aplica `_map_cell_closed` às células caminháveis. |
| `valid_map` | `map_validate` | Exige seis configurações, grade com altura e largura mínimas de 3, um jogador, fechamento e conectividade. Antes a presença das configurações era verificada separadamente por `valid_texture`. |

### Busca de conectividade e limpeza

| Função antiga | Função nova ou destino | Tradução e diferença principal |
| --- | --- | --- |
| `count_walkable_cells` | `map_count_walkable` em `map_cells.c` | Conta chão e marcador do jogador; usa `size_t` e percorre a largura normalizada. |
| `allocate_visited` | Incorporada em `_map_flood_init` | Antes alocava uma linha de visitas por linha do mapa; agora aloca um único vetor de `width * height` posições. |
| `add_point` | `_map_flood_add` em `map_connectivity.c` | Adiciona célula caminhável ainda não visitada à fila. O retorno mudou: `1` agora também significa “nada a adicionar”; `0` significa falha de capacidade. |
| `add_neighbours` | `_map_flood_neighbours` | Tenta esquerda, direita, cima e baixo; agora propaga uma eventual falha de `_map_flood_add`. |
| `init_flood` | `_map_flood_init` | Calcula capacidade, verifica multiplicações, aloca fila e visitados, e insere o jogador. A capacidade agora é o total de posições da grade. |
| `run_flood` | `_map_flood_run` | Executa busca em largura pela fila; devolve a quantidade alcançada, ou `0` se a expansão falhar. |
| `validate_map_connectivity` | `map_connectivity_validate` | Compara quantidade alcançada com `map_count_walkable`, libera fila/visitados e rejeita regiões desconectadas. |
| `free_data` | `map_cleanup` em `map.c`, chamado diretamente ou por `runtime_cleanup` | Libera grade e caminhos das texturas. Agora recebe `t_map *`, zera apenas o mapa e respeita a transferência de posse para `g->map`. |

## 4. Funções novas sem equivalente direto

Estas funções aparecem no componente novo e não eram apenas renomeações:

| Função nova | Papel |
| --- | --- |
| `_map_texture_field` | Transforma `NO/SO/WE/EA` nos bits usados para presença e duplicatas; participa da substituição de `texture_destination`. |
| `_map_parse_separator` | Exige uma vírgula na posição corrente durante a leitura de RGB. |
| `_map_parse_rgb` | Encadeia três componentes e dois separadores e exige fim de string; participa da substituição de `parse_color`. |
| `_map_free_grid` em `map_grid.c` | Libera uma grade temporária ou a antiga depois da cópia. |
| `_map_pad_row` | Copia uma linha e preenche o restante com espaços até `width`. |
| `_map_copy_grid` | Constrói uma nova grade retangular; em falha, desfaz as alocações parciais. |
| `map_rectangularize` | Troca a grade irregular pela cópia retangular antes da validação. |
| `_map_apply_player` em `map.c` | Converte a letra `N/S/E/W` em posição central, vetor de direção e plano da câmera. |
| `_map_transfer` | Move o mapa validado de `t_map_data` para `t_game` e troca o marcador do jogador por chão. |
| `map_parse` | Além de suceder `parser_controller`, é a nova interface que entrega dados prontos ao jogo. |

As outras funções novas já estão na tabela anterior, inclusive as que foram
divididas ou incorporadas. Assim, as tabelas cobrem todas as funções definidas
nos nove arquivos antigos e nos oito arquivos de `source/map/`.

## 5. Mudanças de comportamento que merecem atenção

### Linhas irregulares e acesso à grade

O parser antigo mantinha comprimentos diferentes e `map_at` consultava
`ft_strlen(data->map[y])` para evitar ler além de cada linha. O novo primeiro
calcula a maior largura, depois `map_rectangularize` completa as linhas
menores com espaços. Por exemplo:

```text
Antes:  "11111"  "1N011"  "1111"
Depois: "11111"  "1N011"  "1111 "
```

Esse espaço representa vazio, não parede. A validação continua rejeitando
qualquer chão ou jogador que toque esse vazio. A grade retangular permite que
o jogo use `grid[y][x]` diretamente em movimento, raycast e minimapa.

### Cores e configuração

`F 220,100,0` antes produzia `t_rgb {220, 100, 0}`. Agora produz
`color_floor == 0xDC6400`. A leitura sequencial de RGB exige exatamente duas
vírgulas: o `ft_split` antigo ignorava campos vazios entre vírgulas, de modo
que alguns textos malformados podiam resultar em três partes aparentemente
válidas. A configuração agora usa bits para identificar tanto presença quanto
duplicatas. `valid_texture` nunca testou se um arquivo XPM realmente existe;
o jogo carrega os XPMs depois do parser, em `source/texture.c`.

### Jogador e posse da memória

O marcador, por exemplo `N` em `(x=2, y=1)`, vira a posição contínua
`(2.5, 1.5)` no centro da célula. Para `N`, a direção é `(0, -1)` e o plano
perpendicular é `(PLAYER_PLANE_SCALE, 0)`. `_map_transfer` substitui `N` por
`T_FLOOR` na grade porque colisão e raycast esperam chão embaixo do jogador.

Antes, `main` criava `t_data`, chamava o controlador e sempre chamava
`free_data`. Agora `map_parse` cria `t_map_data` zerado: se leitura ou
validação falhar, `map_cleanup(&data.map)` libera o estado parcial; se tiver
sucesso, `_map_transfer` passa a posse dos ponteiros para `g->map` e zera
`data.map`. Mais tarde, `runtime_cleanup` chama `map_cleanup(&g->map)`.
Isso evita liberar a grade ou os caminhos duas vezes.

### Por que a conectividade continua separada do fechamento?

O fechamento verifica **cada vizinho** de uma célula caminhável. Uma sala
isolada ainda pode estar completamente cercada por paredes, então passar no
fechamento não prova que o jogador chega a todos os espaços caminháveis.
`map_connectivity_validate` percorre a grade a partir do jogador e compara o
total alcançado com `map_count_walkable`.

## 6. Roteiro para estudar ou depurar

1. Comece em `source/map/map.c`, na função `map_parse`, e acompanhe a ordem
   `map_read_archive` → `map_validate` → `_map_transfer`.
2. Siga uma linha `NO textures/north.xpm` por `_map_process_line`,
   `map_parse_config` e `_map_parse_texture`. Observe quando `CUB_NO` é marcado.
3. Siga uma linha `F 220,100,0` por `_map_parse_color_config` e
   `map_parse_color`. Compare `0xDC6400` com as componentes originais.
4. Siga `N` pela leitura da grade, `_map_player_count`, a busca de
   conectividade e `_map_apply_player`. Veja a letra virar `T_FLOOR` só após
   a validação.
5. Para investigar uma rejeição, identifique a etapa: extensão/leitura em
   `map_read_archive`, identificador/valor em `map_parse_config`, formato da
   grade em `map_validate`, ou regiões isoladas em
   `map_connectivity_validate`. O erro público do parser é genérico:
   `Invalid map file`.

Para exercitar só o parser, sem depender de uma janela X11, execute
`bash tests/test_maps.sh` na raiz do projeto. O script compila um pequeno
driver com os arquivos de `source/map/`; nos mapas válidos, também confere
se os caminhos das texturas podem ser lidos. Ele não testa a decodificação
das imagens XPM nem a renderização.
