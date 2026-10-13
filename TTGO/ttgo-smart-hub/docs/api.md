# APIs e integrações

O painel HTTP/porta 80 só responde enquanto está aberto pelo módulo Configuração. Não há backend obrigatório. Use MQTT para receber alertas continuamente com o painel fechado.

## GET /api/status

Autenticação Digest: usuário `admin`, senha mostrada na tela Configuração.

```sh
curl --digest -u 'admin:SENHA_DA_PLACA' http://IP_DA_PLACA/api/status
```

Resposta 200: `firmware`, `uptime` (segundos), `heap` (bytes), `wifi`, `mqtt`, `alerts`, `weather_valid` e `ha_valid`. Valores meteorológicos válidos podem ser cache do último sucesso; a tela mostra a idade e eventual falha da atualização.

## POST /api/alert

Cabeçalho `X-Hub-Key` com a senha da placa. Content-Type `application/json`. Payload de até 384 bytes, texto de 1 a 95 bytes e nível 0 (informação), 1 (aviso) ou2 (erro). Limite de uma mensagem/segundo.

```sh
curl -X POST http://IP_DA_PLACA/api/alert \
  -H 'X-Hub-Key: SENHA_DA_PLACA' \
  -H 'Content-Type: application/json' \
  -d '{"text":"Pipeline concluido","level":0}'
```

202 aceita; 400 payload inválido; 401 chave inválida; 413 muito grande; 429 limite de frequência. Use texto ASCII para melhor compatibilidade com as fontes embarcadas.

Também há cliente Python sem bibliotecas externas em `examples/send_alert.py`: defina `TTGO_HUB_URL` e `TTGO_HUB_KEY` no ambiente antes de executar. Não salve a chave no Git.

## MQTT

Tópico padrão de assinatura: `ttgo/hub/events`. Mensagem pode ser texto ou JSON:

```json
{"text":"Servidor indisponivel","level":2}
```

```sh
mosquitto_pub -h IP_DO_BROKER -t ttgo/hub/events -m '{"text":"Build OK","level":0}'
```

Configure usuário/senha no painel quando o broker exigir. Para TLS, ative a opção e use a porta TLS do broker. PubSubClient publica/assina QoS 0, buffer 512 bytes; payload de eventos é truncado para 95 bytes na tela. Payloads acima do buffer MQTT não serão recebidos.

Telemetria a cada 30 s em `ttgo/hub/events/telemetry`: `uptime`, `heap`, `rssi`. Se mudar o tópico, o sufixo continua `/telemetry`. Evite assinar um wildcard que inclua a própria telemetria se não quiser transformá-la em alertas. O broker deve permitir leitura do tópico de eventos e publicação da telemetria.

## Clima

Open-Meteo `/v1/forecast`, modulo Clima habilitado e coordenadas configuradas, `current=temperature_2m,weather_code,wind_speed_10m`, `daily=temperature_2m_max,temperature_2m_min`, `forecast_days=1`, `timezone=auto`. Sem chave nesta integração. Dados por HTTPS a cada 10 min ou toque GPIO0 na tela Clima. A previsão diária é do dia da localização; o relógio do dashboard usa o fuso POSIX configurado. Consulte as condições de uso do provedor para sua aplicação.

## Home Assistant

URL base, token de longa duração e uma entidade são configurados pelo painel. Consulta `GET /api/states/ENTITY`, com `Authorization: Bearer TOKEN`. Toggle: `POST /api/services/DOMAIN/toggle` e corpo `{"entity_id":"ENTITY"}`. Somente domínios `switch`, `light`, `input_boolean` permitem comando; demais entidades são monitoradas.

Na tela HA, GPIO0 abre a confirmação. Um segundo toque envia o comando; segurar GPIO0 cancela e volta ao menu. Uma falha do serviço não dispara repetição automática do comando. O estado é atualizado depois de um comando aprovado e a cada 30 s.

## OTA

Com Wi-Fi conectado, abra Configuração e ative o painel. Com o IP e senha da placa:

```sh
python CAMINHO_FRAMEWORK/tools/espota.py -i IP_DA_PLACA -p 3232 --auth SENHA_DA_PLACA -f .pio/build/ttgo-smart-hub/firmware.bin
```

O mesmo ambiente PlatformIO precisa ser usado para gerar a imagem. Feche o painel para encerrar a janela OTA. Para recuperação, use USB. Não configurar OTA como método do primeiro upload: bootloader e partições precisam existir.
