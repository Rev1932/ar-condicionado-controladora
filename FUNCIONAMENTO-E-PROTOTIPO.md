# Funcionamento do Ar-Condicionado e Demonstração com Prototipagem em Arduino

**Documento de Literatura — Projeto de Sustentabilidade**
**Uso Consciente de Ar-Condicionado em Ambientes Compartilhados**

---

## Parte 1 — Como Funciona um Ar-Condicionado

### 1.1 Princípio Físico Fundamental

O ar-condicionado não "cria" frio. Ele **transfere calor** de um ambiente para outro, aproveitando uma propriedade física dos fluidos refrigerantes: ao mudar de estado (líquido → gás), eles absorvem calor do ambiente; ao voltar ao estado líquido (gás → líquido), eles liberam esse calor.

Este processo é chamado de **Ciclo de Refrigeração por Compressão de Vapor** e é o mesmo princípio que geladeiras e freezers utilizam.

### 1.2 Os Quatro Estágios do Ciclo de Refrigeração

```
                    AMBIENTE EXTERNO (quente)
                           ↑ calor liberado
                    ┌──────────────────────┐
                    │   CONDENSADOR (2)    │  ← gás quente resfria e
                    │   (unidade externa)  │    vira líquido
                    └──────────┬───────────┘
                               │ líquido de alta pressão
                    ┌──────────▼───────────┐
                    │  VÁLVULA DE EXPANSÃO  │  ← líquido é expandido,
                    │       (3)            │    perde pressão e temperatura
                    └──────────┬───────────┘
                               │ líquido frio de baixa pressão
                    ┌──────────▼───────────┐
                    │   EVAPORADOR (4)     │  ← líquido frio absorve calor
                    │   (unidade interna)  │    do ambiente e vira gás
                    └──────────┬───────────┘
                               ↑ calor absorvido
                    AMBIENTE INTERNO (resfriado)
                               │ gás de baixa pressão
                    ┌──────────▼───────────┐
                    │    COMPRESSOR (1)    │  ← comprime o gás, aumentando
                    │   (unidade externa)  │    pressão e temperatura
                    └──────────────────────┘
```

**Estágio 1 — Compressão:**
O compressor aspira o fluido refrigerante em estado gasoso e de baixa pressão, comprimindo-o mecanicamente. A compressão eleva tanto a pressão quanto a temperatura do gás — ele sai do compressor como um gás quente e de alta pressão.

**Estágio 2 — Condensação:**
O gás quente circula pela serpentina do condensador (unidade externa), onde um ventilador força a passagem do ar ambiente sobre ela. O ar mais frio do ambiente externo absorve o calor do gás, que se condensa, voltando ao estado líquido. É por isso que a unidade externa sempre sopra ar quente.

**Estágio 3 — Expansão:**
O líquido de alta pressão passa pela válvula de expansão termostática (TEV) ou por um tubo capilar. Essa restrição faz o líquido expandir subitamente, reduzindo drasticamente sua pressão e, consequentemente, sua temperatura. O fluido entra no evaporador frio.

**Estágio 4 — Evaporação:**
O líquido frio circula pela serpentina do evaporador (unidade interna). O ventilador interno força o ar do cômodo sobre essa serpentina fria. O calor do ar do cômodo é absorvido pelo fluido refrigerante, que se evapora (volta ao estado gasoso). O ar resfriado é soprado de volta para o ambiente. O gás retorna ao compressor e o ciclo recomeça.

---

### 1.3 Componentes de um Ar-Condicionado Split de Mercado

Um ar-condicionado split doméstico moderno é composto por dois conjuntos de componentes:

#### Unidade Interna (Evaporadora)

| Componente | Função |
|---|---|
| Evaporador (serpentina de cobre/alumínio) | Troca de calor com o ar do cômodo — resfria o ambiente |
| Ventilador centrífugo (blower) | Força a circulação do ar do cômodo sobre o evaporador |
| Motor do flap (direcionador de ar) | Orienta o fluxo de ar frio para diferentes direções |
| Placa de controle eletrônico (PCB) | Processa comandos do controle remoto e sensores |
| Sensor de temperatura NTC (termistor) | Mede a temperatura do ar de retorno do cômodo |
| Sensor de presença PIR (modelos inverter modernos) | Detecta presença e direciona o ar para onde há pessoas |
| Display LED/LCD | Exibe temperatura configurada e modo de operação |
| Filtro de ar | Remove poeira e partículas do ar circulante |
| Bandeja de condensado | Coleta a água condensada pelo resfriamento do ar úmido |
| Tubulação de drenagem | Descarta a água condensada para fora do ambiente |

#### Unidade Externa (Condensadora)

| Componente | Função |
|---|---|
| Compressor hermético rotativo ou scroll | Coração do sistema — comprime o fluido refrigerante |
| Condensador (serpentina com aletas de alumínio) | Troca de calor com o ar externo — libera o calor absorvido |
| Ventilador axial (hélice) | Força a circulação do ar externo sobre o condensador |
| Válvula de expansão termostática (TEV) | Controla a vazão e a expansão do fluido refrigerante |
| Válvula reversora de 4 vias | Inverte o ciclo para modo aquecimento (modelos heat pump) |
| Separador de óleo | Impede que óleo lubrificante siga o circuito de refrigeração |
| Pressostatos de alta e baixa pressão | Proteções de segurança contra pressão fora dos limites |
| Capacitores de partida e de trabalho | Auxiliam a partida e o funcionamento do motor do compressor |
| Tubulação de cobre (linha de líquido e de gás) | Interliga as unidades interna e externa, conduzindo o fluido |

#### Fluido Refrigerante

| Fluido | Uso | Observação |
|---|---|---|
| R-22 (Freon) | Equipamentos antigos (pré-2010) | Proibido para fabricação nova — agride a camada de ozônio |
| R-410A | Padrão dominante 2010–2022 | Sem dano ao ozônio, mas alto GWP (potencial de aquecimento global) |
| R-32 | Equipamentos modernos (2018+) | Menor GWP que R-410A, maior eficiência energética |

---

### 1.4 Sistema de Controle Eletrônico de um AR-Condicionado Real

O controle eletrônico moderno toma decisões baseadas em múltiplos parâmetros:

```
CONTROLE REMOTO / APP
         │
         ▼
    PCB PRINCIPAL
         │
    ┌────▼─────────────────┐
    │  LEITURA DE SENSORES  │
    │  - Temperatura atual  │ ← sensor NTC interno
    │  - Temperatura alvo   │ ← configuração do usuário
    │  - Presença           │ ← PIR (modelos modernos)
    │  - Modo: frio/calor   │
    └────┬─────────────────┘
         │
    ┌────▼──────────────────────────────┐
    │           LÓGICA DE DECISÃO        │
    │                                    │
    │  Se (temperatura_atual >           │
    │      temperatura_alvo + histerese) │
    │       → Liga compressor            │
    │                                    │
    │  Se (temperatura_atual <=          │
    │      temperatura_alvo)             │
    │       → Desliga compressor         │
    │         (ventilador continua)      │
    └────┬──────────────────────────────┘
         │
    ┌────▼─────────────────┐
    │     ATUADORES        │
    │  - Compressor        │
    │  - Ventilador interno│
    │  - Ventilador externo│
    │  - Flap              │
    └──────────────────────┘
```

> **Histerese:** Para evitar que o compressor ligue e desligue rapidamente (o que o danificaria), o controle utiliza uma faixa de tolerância. Se configurado para 24°C, o compressor liga quando a temperatura passa de 25°C (alvo + 1°C) e desliga quando cai a 23°C (alvo - 1°C). Essa tolerância é a histerese.

---

## Parte 2 — A Demonstração Prática com Arduino

### 2.1 O Que Foi Construído

O protótipo desenvolvido é uma **simulação funcional do sistema de controle de um ar-condicionado**, não do equipamento em si. Ele reproduz fielmente a lógica de decisão eletrônica presente na placa de controle (PCB) de qualquer ar-condicionado moderno, usando componentes acessíveis e de baixo custo.

| Componente Real (AR-Condicionado) | Equivalente no Protótipo Arduino |
|---|---|
| Sensor de presença PIR interno | Sensor PIR HC-SR501 |
| PCB de controle eletrônico | Arduino Uno R3 |
| Relé eletromecânico da placa | Módulo Relé 5V |
| Compressor + sistema de refrigeração | Micro Ventilador (Cooler) 5V |
| Alimentação da rede elétrica | Bateria 9V + Adaptador P4 |

### 2.2 Comportamento Demonstrado no Protótipo Atual

O protótipo construído implementa a primeira camada de inteligência de um ar-condicionado: **controle por presença**.

```
FLUXO DO PROTÓTIPO ATUAL (PIR apenas):

Início
  │
  ├─ PIR detecta movimento?
  │   ├─ SIM → Liga ventilador imediatamente
  │   └─ NÃO → Conta tempo sem movimento
  │               └─ Tempo > 30s? → Desliga ventilador
  │
  └─ (repete indefinidamente)
```

Este comportamento já demonstra o conceito central da tese: **o ambiente só consome energia de climatização quando há pessoas presentes**, eliminando o desperdício causado pelo esquecimento de desligar o aparelho.

---

## Parte 3 — Fluxo de Decisão Completo com Controle de Temperatura

### 3.1 Descrição do Comportamento

O fluxo completo integra dois critérios de decisão:

1. **Presença humana** — detectada pelo sensor PIR
2. **Temperatura ambiente** — medida por sensor dedicado e comparada com uma temperatura-alvo configurada

O sistema opera segundo as seguintes regras:

- **Ao detectar movimento:** verifica a temperatura do ambiente
  - Se a temperatura estiver **acima do limite configurado** → liga o ar-condicionado
  - Se a temperatura estiver **igual ou abaixo do limite** → permanece inativo (não há necessidade de climatização)
- **Com o ar-condicionado ligado:** continua monitorando a temperatura
  - Quando a temperatura atinge o valor configurado → desliga o ar-condicionado (o ambiente está climatizado)
  - Se nesse momento ainda há presença → mantém o monitoramento de temperatura (religuará se esquentar novamente)
- **Sem presença detectada:** desliga o ar-condicionado independentemente da temperatura

### 3.2 Diagrama de Fluxo de Decisão

```
┌─────────────────────────────────────────────────────────────┐
│                    INÍCIO / MONITORAMENTO                    │
└─────────────────────────┬───────────────────────────────────┘
                          │
                ┌─────────▼──────────┐
                │  PIR detectou      │
                │  movimento?        │
                └─────────┬──────────┘
                          │
              ┌───────────┴───────────┐
             NÃO                     SIM
              │                       │
              ▼                       ▼
   ┌──────────────────┐    ┌──────────────────────┐
   │  Sistema ligado? │    │  Lê temperatura atual │
   └────────┬─────────┘    │  do ambiente (DHT22)  │
            │              └──────────┬───────────┘
    ┌───────┴────────┐                │
   SIM              NÃO              │
    │                │               │
    ▼                ▼               ▼
┌──────────┐    ┌─────────┐  ┌───────────────────────────┐
│  Desliga │    │ Mantém  │  │  Temp. atual >             │
│  sistema │    │ inativo │  │  Temp. configurada?        │
└──────────┘    └─────────┘  └──────────┬────────────────┘
                                        │
                           ┌────────────┴────────────┐
                          SIM                       NÃO
                           │                         │
                           ▼                         ▼
              ┌────────────────────┐    ┌────────────────────────┐
              │   Liga o sistema   │    │  Mantém inativo        │
              │   (ventilador ON)  │    │  (ambiente já fresco)  │
              └────────┬───────────┘    └────────────────────────┘
                       │
                       │ (monitoramento contínuo com sistema ligado)
                       │
              ┌────────▼───────────────────┐
              │  Temp. atual <=            │
              │  Temp. configurada?        │
              └────────┬───────────────────┘
                       │
           ┌───────────┴───────────┐
          SIM                     NÃO
           │                       │
           ▼                       ▼
  ┌─────────────────┐    ┌──────────────────────┐
  │  Desliga sistema│    │  Mantém sistema ligado│
  │  (ambiente OK)  │    │  (ainda resfriando)   │
  └────────┬────────┘    └──────────────────────┘
           │
           ▼
  ┌─────────────────────────────────┐
  │  Retorna ao monitoramento de    │
  │  presença (PIR)                 │
  │  → Se esquentar de novo e       │
  │    houver presença, religará    │
  └─────────────────────────────────┘
```

### 3.3 Caso de Uso Completo — Exemplo Prático

**Cenário:** Sala configurada para 24°C. Temperatura inicial do ambiente: 28°C.

```
08:00 — Pessoa entra na sala
         PIR detecta movimento
         DHT22 lê: 28°C > 24°C (configurado)
         → LIGA o ventilador/ar-condicionado

08:20 — Ar-condicionado funcionando
         DHT22 lê: 24°C ≤ 24°C (configurado)
         → DESLIGA o ar-condicionado
         PIR ainda detecta presença → mantém monitoramento

08:35 — Temperatura volta a subir (ganho de calor do ambiente)
         DHT22 lê: 25°C > 24°C
         PIR ainda detecta presença
         → LIGA novamente o ar-condicionado

09:00 — Pessoa sai da sala
         PIR não detecta movimento por 30s (ou tempo configurado)
         → DESLIGA o ar-condicionado
         (independente da temperatura — sem presença, sem consumo)
```

---

## Parte 4 — Componentes para o Protótipo com Controle de Temperatura

Para implementar o fluxo de decisão completo descrito na Parte 3, é necessário adicionar um **sensor de temperatura e umidade** ao protótipo base.

### 4.1 Lista Completa de Componentes do Protótipo Avançado

#### Componentes Principais

| Componente | Quantidade | Função |
|---|---|---|
| Arduino Uno R3 | 1 | Microcontrolador — executa toda a lógica de decisão |
| Sensor de Movimento PIR HC-SR501 | 1 | Detecta presença humana pelo calor infravermelho |
| Sensor de Temperatura e Umidade DHT22 | 1 | Mede a temperatura atual do ambiente |
| Módulo Relé 5V | 1 | Interruptor eletrônico que liga/desliga o ventilador |
| Micro Ventilador (Cooler) 5V | 1 | Simula o compressor/sistema de refrigeração |
| Bateria 9V com adaptador P4 | 1 | Alimentação autônoma do sistema |

> **Por que DHT22 e não DHT11?**
> O DHT11 custa ~R$8 e mede temperatura com precisão de ±2°C — margem alta para controle de climatização.
> O DHT22 custa ~R$18 e oferece precisão de ±0,5°C, além de suportar temperaturas negativas (-40°C a +80°C).
> Para um projeto de controle de conforto térmico, o DHT22 é a escolha tecnicamente correta.

#### Materiais de Apoio

| Material | Quantidade | Observação |
|---|---|---|
| Protoboard 400 pontos | 1 | Distribuição de alimentação e conexões |
| Jumpers macho-macho | 10 | Conexões entre protoboard e Arduino |
| Jumpers macho-fêmea | 8 | Conexões com PIR e DHT22 (pinos fêmea) |
| Resistor 10kΩ | 1 | Pull-up obrigatório no pino de dados do DHT22 |
| Cabo USB tipo A/B | 1 | Upload do código e alimentação alternativa via PC |

#### Componente Opcional — Display

| Componente | Quantidade | Função |
|---|---|---|
| Display LCD 16x2 + Módulo I2C | 1 | Exibe temperatura atual, temperatura configurada e estado do sistema em tempo real |

> O módulo I2C reduz as conexões do LCD de 16 fios para apenas 4 (VCC, GND, SDA, SCL), simplificando muito a montagem.

---

### 4.2 Conexão do Sensor DHT22

O DHT22 possui 4 pinos (alguns módulos já vêm com 3 pinos e resistor embutido):

```
DHT22 (4 pinos):

    +-----+
    |     |
    | DHT |
    | 22  |
    |     |
    +--+--+--+--+
       |  |  |  |
      VCC DAT NC GND

Pino 1 (VCC) ──→ 5V
Pino 2 (DAT) ──→ Arduino D6 + Resistor 10kΩ até 5V (pull-up)
Pino 3 (NC)  ──→ não conectar
Pino 4 (GND) ──→ GND
```

O resistor de pull-up é necessário porque o protocolo de comunicação do DHT22 (single-wire) exige que o barramento fique em nível alto quando inativo.

---

### 4.3 Pinagem Completa do Protótipo Avançado

| Pino Arduino | Conectado a | Descrição |
|---|---|---|
| 5V | Protoboard (+) | Alimentação de todos os módulos |
| GND | Protoboard (-) | Terra comum |
| D6 | DHT22 DAT | Leitura de temperatura e umidade |
| D7 | PIR HC-SR501 OUT | Leitura do sensor de presença |
| D8 | Módulo Relé IN | Controle do relé (active low) |
| Jack DC | Bateria 9V (P4) | Alimentação principal |

---

### 4.4 Biblioteca Necessária para o DHT22

No Arduino IDE, instale a biblioteca antes de compilar o código:

1. Acesse `Sketch > Incluir Biblioteca > Gerenciar Bibliotecas`
2. Pesquise por `DHT sensor library`
3. Instale a biblioteca de **Adafruit** (versão mais recente)
4. Instale também a dependência `Adafruit Unified Sensor` quando solicitado

---

### 4.5 Estimativa de Custo — Protótipo Avançado

| Componente | Custo Estimado (R$) |
|---|---|
| Arduino Uno R3 | R$ 45 – R$ 65 |
| Sensor PIR HC-SR501 | R$ 12 – R$ 20 |
| Sensor DHT22 | R$ 15 – R$ 22 |
| Módulo Relé 5V | R$ 8 – R$ 15 |
| Micro Ventilador 5V | R$ 10 – R$ 18 |
| Bateria 9V + Adaptador P4 | R$ 12 – R$ 20 |
| Protoboard 400 pts + Jumpers | R$ 15 – R$ 25 |
| Resistor 10kΩ (pacote) | R$ 3 – R$ 6 |
| Display LCD 16x2 + I2C (opcional) | R$ 20 – R$ 35 |
| **Total sem display** | **R$ 120 – R$ 191** |
| **Total com display** | **R$ 140 – R$ 226** |

---

## Parte 5 — Componentes de um Ar-Condicionado Real de Mercado

Para contextualizar a escala e complexidade do equipamento que o protótipo representa, segue o levantamento completo dos componentes de um **ar-condicionado split residencial** de 9.000 BTU, padrão de mercado.

### 5.1 Unidade Interna (Evaporadora)

| Componente | Especificação Típica | Função |
|---|---|---|
| Serpentina evaporadora | Tubos de cobre Ø6-7mm com aletas de alumínio | Transfere calor do ar do cômodo para o fluido refrigerante |
| Motor do ventilador interno | Motor AC 220V / 20–35W | Aciona o blower centrífugo |
| Blower centrífugo | Policarbonato / diâmetro 80–100mm | Força circulação do ar sobre a serpentina |
| Motor do flap horizontal | Motor de passo 5V / 3W | Direciona o ângulo do fluxo de ar |
| Motor do flap vertical | Motor de passo 5V / 3W | Oscilação lateral do fluxo de ar |
| Placa de controle (PCB) | Microcontrolador STM32 ou similar | Processa sensores, comandos e controla todos os atuadores |
| Sensor NTC de temperatura | Termistor 10kΩ / precisão ±0,5°C | Mede temperatura do ar de retorno |
| Sensor NTC da serpentina | Termistor 10kΩ | Monitora temperatura da serpentina (prevenção de gelo) |
| Receptor IR | 38kHz | Recebe comandos do controle remoto |
| Display LED de 7 segmentos | 4 dígitos | Exibe temperatura configurada |
| Sensor PIR (modelos inverter) | Campo 120° | Detecta presença e otimiza direcionamento do ar |
| Filtro de ar | Malha de polipropileno | Remove poeira — requer limpeza periódica |
| Bandeja de condensado | PVC / polietileno | Coleta água condensada pelo contato do ar úmido com a serpentina fria |
| Tubulação de dreno | PVC Ø16mm | Descarta condensado para fora do ambiente |
| Capacitor do motor interno | 3–5µF / 450V AC | Auxilia partida e funcionamento do motor interno |
| Transformador de controle | 220V → 12V DC | Alimenta a PCB de controle |
| Filtro EMI | Componente passivo | Suprime interferência eletromagnética |

### 5.2 Unidade Externa (Condensadora)

| Componente | Especificação Típica | Função |
|---|---|---|
| Compressor hermético rotativo | 220V / 800–1000W (9.000 BTU) | Comprime o fluido refrigerante — componente mais caro do sistema |
| Serpentina condensadora | Tubos de cobre com aletas de alumínio | Libera o calor absorvido do ambiente interno para o externo |
| Motor do ventilador externo | Motor AC 220V / 30–50W | Aciona a hélice sobre o condensador |
| Ventilador axial (hélice) | Plástico / alumínio — diâmetro 300–400mm | Força circulação de ar externo sobre a serpentina |
| Válvula de expansão termostática (TEV) | Controle por bulbo de pressão | Regula a vazão do fluido refrigerante conforme a demanda |
| Tubo capilar (modelos básicos) | Cobre Ø0,5–1mm / 1–2m de comprimento | Restrição fixa — alternativa mais simples à TEV |
| Válvula reversora de 4 vias | Solenoide 220V | Inverte o sentido do ciclo (modo aquecimento — heat pump) |
| Separador de óleo | Cilindro metálico na saída do compressor | Retém óleo lubrificante para evitar que circule no sistema |
| Pressostato de alta pressão | Corte em ~41 bar | Desliga compressor se pressão exceder limite seguro |
| Pressostato de baixa pressão | Corte em ~1 bar | Desliga compressor se houver perda de carga (falta de gás) |
| Capacitor de partida | 30–50µF / 400V AC | Torque extra na partida do compressor |
| Capacitor de trabalho | 10–20µF / 400V AC | Melhora fator de potência durante operação |
| Acumulador de sucção | Reservatório metálico | Previne retorno de líquido ao compressor |
| Válvula de serviço (Schrader) | Rosca 1/4" SAE | Ponto de carga e verificação de pressão do gás |
| Placa de controle externa | Microcontrolador dedicado | Monitora pressostatos e controla ventilador externo |

### 5.3 Fluido Refrigerante e Tubulação

| Item | Especificação | Observação |
|---|---|---|
| Fluido refrigerante R-32 | ~400–600g (9.000 BTU) | Gás com menor impacto ambiental (GWP = 675 vs 2.088 do R-410A) |
| Linha de líquido (cobre) | Ø1/4" (6,35mm) — isolada | Conduz fluido líquido de alta pressão da unidade externa à interna |
| Linha de gás/sucção (cobre) | Ø3/8" (9,52mm) — isolada | Conduz fluido gasoso de baixa pressão da interna à externa |
| Isolamento térmico (armaflex) | Espessura 9–13mm | Evita troca de calor das tubulações com o ambiente |
| Cabo de força | 3 condutores 2,5mm² | Alimentação 220V da unidade interna à externa |
| Cabo de comunicação | 2 condutores 0,75mm² | Sinal de controle entre as unidades |

### 5.4 Comparativo: Ar-Condicionado Real vs. Protótipo Arduino

| Aspecto | Ar-Condicionado Real | Protótipo Arduino |
|---|---|---|
| Custo | R$ 1.200 – R$ 2.500 | R$ 120 – R$ 226 |
| Resfriamento real | Sim (ciclo de refrigeração) | Não (apenas circulação de ar) |
| Controle por presença | Sim (modelos modernos) | Sim |
| Controle por temperatura | Sim (termistor NTC) | Sim (DHT22) |
| Lógica de decisão | PCB dedicada (STM32) | Arduino Uno R3 |
| Tensão de operação | 220V AC | 5V DC (bateria 9V) |
| Potência consumida | 800–1.000W | ~2W |
| Complexidade de montagem | Requer técnico certificado | Montagem educacional acessível |
| Objetivo do protótipo | — | Demonstrar a lógica de controle em escala acessível |

---

## Conclusão

O protótipo desenvolvido com Arduino reproduz com fidelidade conceitual o **sistema de tomada de decisão** presente em qualquer ar-condicionado moderno. A diferença está na escala: enquanto o aparelho real executa esta lógica sobre um ciclo termodinâmico de alta potência (compressor, refrigerante, serpentinas), o protótipo a executa sobre um ventilador de 5V.

O valor do protótipo para a tese de sustentabilidade está exatamente nessa equivalência lógica: ao demonstrar que **presença + temperatura → decisão de ligar/desligar**, o projeto evidencia que a tecnologia necessária para o uso consciente e automatizado do ar-condicionado é simples, barata e plenamente implementável em ambientes compartilhados como salas de aula, escritórios e espaços de convivência.

A automação elimina o principal vetor de desperdício energético nesses ambientes: o **fator humano** — o esquecimento de desligar o aparelho ao sair do cômodo.
