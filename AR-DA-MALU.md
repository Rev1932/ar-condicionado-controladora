# Controladora de Ar-Condicionado por Detecção de Presença

**Projeto de Sustentabilidade — Uso Consciente de Ar-Condicionado em Ambientes Compartilhados**

Sistema baseado em sensor de movimento PIR que liga o ar-condicionado (simulado por um ventilador) automaticamente ao detectar presença humana e desliga após um período configurável sem movimento.

---

## Componentes

### Componentes Principais

| Componente | Quantidade | Função |
|---|---|---|
| Arduino Uno R3 | 1 | Microcontrolador principal — lê o sensor e controla o relé |
| Sensor de Movimento PIR HC-SR501 | 1 | Detecta presença humana pelo calor infravermelho |
| Módulo Relé 5V | 1 | Interruptor eletrônico que liga/desliga o ventilador |
| Micro Ventilador (Cooler) 5V | 1 | Simula o ar-condicionado no protótipo |
| Bateria 9V com adaptador P4 | 1 | Alimentação autônoma do sistema |

### Materiais Adicionais Necessários

| Material | Quantidade |
|---|---|
| Protoboard (breadboard) | 1 |
| Jumpers macho-macho | 10 |
| Jumpers macho-fêmea | 5 |
| Cabo USB tipo A/B | 1 |
| Computador com Arduino IDE | 1 |

> **Arduino IDE:** download gratuito em arduino.cc/en/software (versão 2.x recomendada)

---

## Observação Importante — Linguagem de Programação

O **Arduino Uno R3** é programado em **C/C++** via Arduino IDE. Ele **não executa Python nativamente**. Para programação em Python seria necessário um Raspberry Pi (custo ~R$200-400), que é superdimensionado para esta aplicação. O Arduino Uno (~R$40-60) é a escolha técnica correta para este projeto de automação simples.

---

## Passo a Passo de Montagem

---

### Fase 1 — Preparação e Entendimento dos Componentes

#### Passo 1.1 — Inventário de Componentes

Antes de qualquer montagem, organize todos os componentes sobre uma superfície limpa e seca (de preferência uma bancada antiestática ou uma folha de papel sobre uma mesa de madeira). Confirme a presença de cada item da lista acima.

**Justificativa:** Verificar os componentes antes de iniciar evita interrupções no meio da montagem por item faltante, além de permitir identificar componentes fisicamente danificados antes de energizar o circuito.

---

#### Passo 1.2 — Identificação dos Pinos do PIR HC-SR501

Segure o sensor com a cúpula branca voltada para cima e os pinos voltados para você:

```
    [ Cúpula Branca Hemisférica ]
              PIR
    +----------+----------+----------+
    |   VCC    |   OUT    |   GND    |
    | (5V)     | (sinal)  | (terra)  |
    +----------+----------+----------+
      esquerda   centro    direita
```

Na parte traseira do sensor há dois potenciômetros ajustáveis com chave de fenda:

- **TX (Sensibilidade):** ajusta o alcance de detecção — de ~3m (anti-horário) a ~7m (horário)
- **TC (Tempo de disparo):** ajusta por quanto tempo OUT fica HIGH após detectar movimento — de ~5s (anti-horário) a ~300s (horário)

Também pode haver um jumper de dois pinos:
- Posição **H (Repeating):** OUT permanece HIGH enquanto há movimento — **use este modo**
- Posição **L (Single):** apenas um pulso HIGH por detecção

**Justificativa:** O PIR HC-SR501 é o componente mais sensível a conexões incorretas. Identificar os pinos antes da montagem elimina o risco de queimar o sensor por inversão de polaridade (VCC no lugar de GND).

---

#### Passo 1.3 — Identificação dos Pinos do Módulo Relé 5V

O módulo possui dois grupos de terminais:

```
LADO DE CONTROLE (conecta ao Arduino):
+-------+-------+-------+
|  GND  |  VCC  |  IN   |
|       |  (5V) |(sinal)|
+-------+-------+-------+

LADO DE CARGA (conecta ao ventilador):
+-------+-------+-------+
|  NC   |  COM  |  NO   |
|(N.Fec)|(comum)|(N.Ab) |
+-------+-------+-------+
```

- **NC (Normally Closed):** circuito fechado quando relé desativado
- **COM (Common):** terminal comum — entra a alimentação da carga
- **NO (Normally Open):** circuito aberto quando relé desativado, fecha quando ativado

> A maioria dos módulos relé é **ativa em nível baixo (active low)**: enviar LOW (0V) no pino IN **ativa** o relé; enviar HIGH (5V) **desativa** o relé.

**Justificativa:** Entender a lógica active-low antes de escrever o código evita que o ventilador fique permanentemente ligado por lógica invertida no firmware.

---

### Fase 2 — Montagem do Circuito

#### Passo 2.1 — Configuração da Alimentação

**Conecte a bateria ao Arduino:**

```
Bateria 9V (+) ──[Adaptador P4]──→ Jack DC do Arduino (polo central positivo)
```

**Monte as trilhas de alimentação na protoboard:**

```
Pino 5V  do Arduino ──→ Trilha (+) vermelha da protoboard
Pino GND do Arduino ──→ Trilha (-) preta da protoboard
```

O regulador interno do Arduino Uno (CI 7805) converte os 9V da bateria para 5V estável, fornecendo até 800mA — suficiente para todos os componentes:

| Componente | Consumo |
|---|---|
| PIR HC-SR501 | ~50mA |
| Módulo Relé 5V | ~70mA |
| Ventilador 5V | ~200mA |
| **Total** | **~320mA** |

**Justificativa:** Centralizar a alimentação pelo Arduino simplifica o circuito e garante que todos os componentes compartilhem o mesmo potencial de referência (terra comum), essencial para o correto funcionamento dos sinais digitais.

> **Nunca conecte a bateria 9V diretamente aos pinos 5V ou 3.3V do Arduino** — isso danificará permanentemente o regulador de tensão.

---

#### Passo 2.2 — Conexão do Sensor PIR HC-SR501

```
PIR VCC ──→ Protoboard trilha (+) [5V]
PIR GND ──→ Protoboard trilha (-) [GND]
PIR OUT ──→ Pino Digital 7 do Arduino
```

Usando jumpers macho-fêmea (a fêmea vai no pino do PIR, o macho vai na protoboard ou no Arduino):

```
Arduino D7  ───────────────────────────────────→ PIR OUT
Protoboard(+) ──→ fileira A da protoboard ──→ PIR VCC
Protoboard(-) ──→ fileira B da protoboard ──→ PIR GND
```

**Justificativa:** O pino digital 7 não possui funções especiais (PWM, SPI, I2C) que possam causar conflito. O PIR fornece saída digital limpa de 0V ou 5V, diretamente compatível com os pinos do Arduino sem divisor de tensão.

---

#### Passo 2.3 — Conexão do Módulo Relé 5V

**Lado de controle (Arduino → Relé):**

```
Relé GND ──→ Protoboard trilha (-) [GND]
Relé VCC ──→ Protoboard trilha (+) [5V]
Relé IN  ──→ Pino Digital 8 do Arduino
```

**Lado de carga (Relé → Ventilador):**

```
Protoboard trilha (+) [5V] ──→ Relé COM
Relé NO                    ──→ Ventilador fio (+) [vermelho]
Ventilador fio (-) [preto] ──→ Protoboard trilha (-) [GND]
```

Diagrama do circuito de carga:

```
[5V] ──→ [COM]──[NO] ──→ [Ventilador +] ──→ [Ventilador -] ──→ [GND]
                 ↑
        (contato fecha aqui quando relé é ativado)
```

**Justificativa:** O terminal NO (Normally Open) garante que o ventilador fique **desligado por padrão** em caso de falha do Arduino ou perda de energia — comportamento seguro e alinhado com o objetivo de economia de energia do projeto.

---

#### Passo 2.4 — Diagrama Completo do Circuito

```
                    ┌──────────────────────────────┐
                    │         ARDUINO UNO R3        │
                    │                              │
Bateria 9V ─[P4]──→│ Jack DC                  5V  │──→ Protoboard (+)
                    │                         GND  │──→ Protoboard (-)
                    │                              │
PIR OUT   ─────────→│ D7                       D8  │──→ Relé IN
                    │                              │
                    └──────────────────────────────┘

Protoboard (+) ──→ PIR VCC          Protoboard (+) ──→ Relé VCC
Protoboard (-) ──→ PIR GND          Protoboard (-) ──→ Relé GND
                                    Protoboard (+) ──→ Relé COM
                                    Relé NO        ──→ Ventilador (+)
                                    Ventilador (-) ──→ Protoboard (-)
```

**Fluxo de sinal:**

```
Presença humana
     │
     ▼
PIR HC-SR501 detecta calor infravermelho
     │
     ▼
PIR OUT = HIGH (5V)
     │
     ▼
Arduino lê HIGH no pino D7
     │
     ▼
Arduino envia LOW no pino D8 (ativa relé active-low)
     │
     ▼
Relé fecha contato NO
     │
     ▼
Circuito do ventilador é fechado → Ventilador LIGA
```

---

### Fase 3 — Calibração Física do Sensor PIR

#### Passo 3.1 — Ajuste dos Potenciômetros

Com o circuito **desligado**, use uma chave de fenda de precisão (philips ou fenda pequena):

**Potenciômetro TX (Sensibilidade):**
- Posição inicial recomendada: **centro** (~5m de alcance)
- Ajuste depois dos testes conforme o tamanho do ambiente

**Potenciômetro TC (Tempo de disparo):**
- Gire ao máximo no sentido **anti-horário** (posição mínima ~5 segundos)
- O controle do tempo de desligamento será feito por software, não por hardware

**Modo de disparo:**
- Configure o jumper para posição **H (Repeating/Repetição)**

**Justificativa:** Controlar o tempo via software é muito mais flexível — permite alterar o comportamento sem abrir o hardware. O potenciômetro TC no mínimo evita delays entre detecções consecutivas de movimento.

---

#### Passo 3.2 — Posicionamento do Sensor no Ambiente

Para cobertura ideal em um cômodo:

- Monte o PIR a **2,0m a 2,5m de altura**
- Aponte para a área de maior circulação (entrada, centro da sala)
- Evite apontar para janelas (luz solar direta causa falsos positivos)
- Evite posicionar próximo a ar-condicionados, aquecedores ou ventiladores (variações de temperatura causam falsos positivos)
- O ângulo de detecção do HC-SR501 é de **120°** horizontalmente

**Justificativa:** O PIR detecta variação de calor infravermelho. Fontes de calor em movimento que não sejam pessoas (correntes de ar quente, luz solar, outros equipamentos) podem causar ativações indesejadas se o sensor estiver mal posicionado.

---

### Fase 4 — Programação do Arduino

#### Passo 4.1 — Instalação do Arduino IDE

1. Acesse **arduino.cc/en/software** e baixe o Arduino IDE 2.x
2. Instale normalmente no computador
3. Conecte o Arduino ao computador via cabo USB tipo A/B
4. Abra o Arduino IDE
5. Em `Ferramentas > Placa > Arduino AVR Boards`: selecione **Arduino Uno**
6. Em `Ferramentas > Porta`: selecione a porta COM correspondente
   - Windows: `COM3`, `COM4`, etc.
   - Linux: `/dev/ttyACM0` ou `/dev/ttyUSB0`
   - macOS: `/dev/cu.usbmodemXXXX`

---

#### Passo 4.2 — Carregando o Código

1. Abra o arquivo `controladora_ac.ino` no Arduino IDE
2. Clique em **Verificar** (ícone de check ✓) para compilar — deve retornar zero erros
3. Clique em **Upload** (ícone de seta →) para enviar ao Arduino
4. Abra o **Monitor Serial** com `Ctrl+Shift+M`, selecione baud rate **9600**
5. Aguarde a mensagem: `PIR estabilizado. Sistema pronto.`

> O sistema aguarda **30 segundos** após ligar antes de iniciar o monitoramento. Este é o tempo de estabilização necessário para o sensor PIR calibrar sua leitura de temperatura ambiente. Durante este período, o LED TX do Arduino piscará conforme as mensagens são enviadas pela serial.

---

#### Passo 4.3 — Lógica do Firmware

```
SETUP:
  Configura pinos → Desliga relé (HIGH) → Aguarda 30s (PIR estabiliza)

LOOP (executa a cada 500ms):
  │
  ├─ PIR = HIGH (detectou movimento)
  │   ├─ Atualiza timestamp: ultimoMovimento = millis()
  │   └─ Se sistema estava desligado → chama ligarSistema()
  │                                     └─ RELAY_PIN = LOW (relé ativa)
  │                                     └─ sistemaLigado = true
  │
  └─ PIR = LOW (sem movimento detectado)
      └─ Se sistema está ligado:
          ├─ (millis() - ultimoMovimento) < TEMPO_DESLIGAMENTO → aguarda
          └─ (millis() - ultimoMovimento) >= TEMPO_DESLIGAMENTO → chama desligarSistema()
                                                                    └─ RELAY_PIN = HIGH (relé desativa)
                                                                    └─ sistemaLigado = false
```

---

#### Passo 4.4 — Ajuste do Tempo de Desligamento

Para alterar o tempo sem movimento antes de desligar, modifique apenas esta constante no arquivo `.ino`:

```c
// 30 segundos — protótipo e testes
const unsigned long TEMPO_DESLIGAMENTO = 30000UL;

// 5 minutos — ambientes com fluxo contínuo
const unsigned long TEMPO_DESLIGAMENTO = 300000UL;

// 10 minutos — salas de reunião ou escritórios
const unsigned long TEMPO_DESLIGAMENTO = 600000UL;

// 15 minutos — dormitórios ou salas de estar
const unsigned long TEMPO_DESLIGAMENTO = 900000UL;
```

---

### Fase 5 — Testes e Validação

#### Passo 5.1 — Teste de Funcionamento Básico

Sequência de teste após upload do código:

1. Conecte a bateria 9V (ou mantenha o USB conectado)
2. Aguarde os **30 segundos** de estabilização — monitor serial exibirá a contagem
3. Mantenha-se parado a ~1m do PIR por 10 segundos para confirmar ausência de falsos positivos
4. Mova o braço lentamente na direção do PIR
5. **Resultado esperado:** ventilador liga e monitor serial exibe `[ON] Presenca detectada -> Ar-condicionado LIGADO`
6. Afaste-se e aguarde 30 segundos sem movimento
7. **Resultado esperado:** ventilador desliga e monitor serial exibe `[OFF] Sem presenca -> Ar-condicionado DESLIGADO`

---

#### Passo 5.2 — Diagnóstico de Problemas

| Sintoma | Causa Provável | Solução |
|---|---|---|
| Ventilador não liga nunca | PIR não detecta / pino errado | Verificar conexão do pino OUT do PIR no D7 |
| Ventilador fica sempre ligado | Lógica de relé invertida / NC no lugar de NO | Verificar uso do terminal NO do relé |
| Falsos positivos frequentes | PIR muito sensível / fonte de calor próxima | Reduzir TX; reposicionar sensor |
| Ventilador não desliga | TEMPO_DESLIGAMENTO muito alto | Reduzir valor da constante |
| Nada funciona | Terra (GND) não compartilhado | Verificar se todos os GNDs estão na mesma trilha |
| Monitor serial mostra lixo | Baud rate incorreto | Selecionar 9600 no Monitor Serial |

---

#### Passo 5.3 — Teste de Resistência (30 minutos)

Após o teste básico, deixe o sistema funcionando por 30 minutos observando:

- Temperatura do Arduino (não deve esquentar excessivamente)
- Estabilidade do ventilador (não deve ligar/desligar rapidamente — "flickering")
- Comportamento com diferentes tipos de movimento (lento, rápido, de longe)

---

### Fase 6 — Considerações para Implementação Real

Este protótipo simula o ar-condicionado com um ventilador 5V. Para controlar um ar-condicionado real:

1. **Substitua o relé 5V** por um módulo relé de **10A / 250V AC** (adequado para rede elétrica)
2. **As conexões de 220V devem ser feitas por um eletricista habilitado**
3. **Considere usar um relé de estado sólido (SSR)** para maior durabilidade em acionamentos frequentes
4. **Para ambientes grandes**, adicione múltiplos PIRs conectados ao mesmo pino D7 em paralelo — qualquer detecção ativará o sistema
5. **Para maior precisão**, considere adicionar um segundo sensor de temperatura/umidade (DHT22) para ligar o AC apenas quando a temperatura ultrapassar um threshold

---

## Resumo de Conexões

### Tabela de Pinagem

| Pino Arduino | Conectado a | Descrição |
|---|---|---|
| 5V | Protoboard (+) | Alimentação dos módulos |
| GND | Protoboard (-) | Terra comum |
| D7 | PIR HC-SR501 OUT | Leitura do sensor de presença |
| D8 | Módulo Relé IN | Controle do relé (active low) |
| Jack DC | Bateria 9V (P4) | Alimentação principal |

### Tabela de Conexões do Relé

| Terminal Relé | Conectado a | Descrição |
|---|---|---|
| VCC | Protoboard (+) 5V | Alimentação do módulo relé |
| GND | Protoboard (-) GND | Terra do módulo relé |
| IN | Arduino D8 | Sinal de controle |
| COM | Protoboard (+) 5V | Alimentação da carga (entrada) |
| NO | Ventilador (+) | Saída para a carga |
| Ventilador (-) | Protoboard (-) GND | Terra da carga |

---

## Estimativa de Tempo de Montagem

| Fase | Atividade | Tempo Estimado |
|---|---|---|
| 1 | Inventário e identificação dos componentes | 20 min |
| 2 | Montagem do circuito na protoboard | 30 min |
| 3 | Calibração física do PIR | 10 min |
| 4 | Instalação do IDE e upload do código | 20 min |
| 5 | Testes e ajustes | 30 min |
| **Total** | | **~110 minutos** |

sessão do claude: claude --resume 1fdc40de-c699-46f4-962c-865ba85476ff
