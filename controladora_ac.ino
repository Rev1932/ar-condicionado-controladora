/*
 * Controladora de Ar-Condicionado por Deteccao de Presenca
 *
 * Hardware:
 *   - Arduino Uno R3
 *   - Sensor PIR HC-SR501  -> pino digital D7
 *   - Modulo Rele 5V       -> pino digital D8 (active low)
 *   - Micro Ventilador 5V  -> controlado pelo rele (terminal NO)
 *   - Bateria 9V + P4      -> Jack DC do Arduino
 *
 * Comportamento:
 *   Liga o rele (ventilador) ao detectar movimento.
 *   Desliga apos TEMPO_DESLIGAMENTO ms sem movimento.
 */

/* ------------------------------------------------------------------ */
/* Pinos                                                               */
/* ------------------------------------------------------------------ */
static const uint8_t PIR_PIN   = 7;
static const uint8_t RELAY_PIN = 8;

/* ------------------------------------------------------------------ */
/* Configuracao de tempo                                               */
/* ------------------------------------------------------------------ */
/*
 * Altere apenas este valor para mudar o tempo sem movimento antes
 * de desligar o sistema.
 *
 * Exemplos:
 *   30000UL  =  30 segundos (testes e prototipo)
 *  300000UL  =   5 minutos  (ambientes de fluxo continuo)
 *  600000UL  =  10 minutos  (salas de reuniao)
 *  900000UL  =  15 minutos  (dormitorios / salas de estar)
 */
static const unsigned long TEMPO_DESLIGAMENTO = 30000UL;

/* ------------------------------------------------------------------ */
/* Estado global                                                       */
/* ------------------------------------------------------------------ */
static unsigned long ultimoMovimento = 0UL;
static bool          sistemaLigado   = false;

/* ------------------------------------------------------------------ */
/* Prototipos                                                          */
/* ------------------------------------------------------------------ */
static void ligarSistema(void);
static void desligarSistema(void);

/* ------------------------------------------------------------------ */
/* Setup                                                               */
/* ------------------------------------------------------------------ */
void setup(void)
{
    pinMode(PIR_PIN,   INPUT);
    pinMode(RELAY_PIN, OUTPUT);

    /*
     * Rele active-low: HIGH = desativado (ventilador desligado).
     * Garante estado seguro antes de qualquer logica.
     */
    digitalWrite(RELAY_PIN, HIGH);

    Serial.begin(9600);
    Serial.println(F("Sistema iniciado. Aguardando estabilizacao do PIR (30s)..."));

    /*
     * O PIR HC-SR501 necessita de ~30 segundos apos energizado para
     * calibrar sua leitura de temperatura ambiente. Leituras feitas
     * antes desse periodo geram falsos positivos inevitaveis.
     */
    delay(30000UL);

    Serial.println(F("PIR estabilizado. Sistema pronto."));
    Serial.println(F("-------------------------------------------"));
}

/* ------------------------------------------------------------------ */
/* Loop principal                                                      */
/* ------------------------------------------------------------------ */
void loop(void)
{
    int estadoPIR = digitalRead(PIR_PIN);

    if (estadoPIR == HIGH) {
        /*
         * Movimento detectado: atualiza o timestamp.
         * Usar millis() aqui e seguro porque a subtracao de dois
         * valores unsigned long funciona corretamente mesmo apos o
         * overflow (~49 dias de uptime), pois a aritmetica modular
         * de complemento de dois preserva o intervalo.
         */
        ultimoMovimento = millis();

        if (!sistemaLigado) {
            ligarSistema();
        }

    } else {
        if (sistemaLigado) {
            unsigned long agora         = millis();
            unsigned long tempoDecorrido = agora - ultimoMovimento;

            if (tempoDecorrido >= TEMPO_DESLIGAMENTO) {
                desligarSistema();
            }
        }
    }

    /*
     * 500ms entre leituras: elimina ruido de bouncing do sinal PIR
     * sem comprometer a responsividade perceptivel pelo usuario.
     */
    delay(500UL);
}

/* ------------------------------------------------------------------ */
/* Liga o rele e registra o evento                                     */
/* ------------------------------------------------------------------ */
static void ligarSistema(void)
{
    digitalWrite(RELAY_PIN, LOW);  /* LOW ativa o rele (active low) */
    sistemaLigado = true;
    Serial.println(F("[ON]  Presenca detectada -> Ar-condicionado LIGADO"));
}

/* ------------------------------------------------------------------ */
/* Desliga o rele e registra o evento                                  */
/* ------------------------------------------------------------------ */
static void desligarSistema(void)
{
    digitalWrite(RELAY_PIN, HIGH); /* HIGH desativa o rele           */
    sistemaLigado = false;
    Serial.println(F("[OFF] Sem presenca -> Ar-condicionado DESLIGADO"));
}
