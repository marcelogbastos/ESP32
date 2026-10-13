# Resultados de construção — 12/10/2026

- Testes C++ executados em WebAssembly/Node: debounce, clique no release, hold sem clique posterior, rollover de millis, comida e virada do Snake, colisão com parede, entrada na cauda que sai, limites e colisões do Pong: aprovados.
- Hardware de referência consultado na sessão anterior: ESP32-D0WDQ6-V3, flash 16 MB. Não houve identificação física de PCB nem upload do Smart Hub.
- Bundle CA obtido via HTTPS de curl.se/ca/cacert.pem, dados Mozilla de 25/09/2026: 121 certificados, 55.587 bytes. PEM e bundle acompanham o projeto; atualizar periodicamente e recompilar.
- Compilação PlatformIO: aprovada, Espressif32 6.9.0 / Arduino ESP32 2.0.17. Aviso TOUCH_CS esperado: esta placa não possui touch. RAM estática: 56.012 bytes (17,1%); sprite de 32.400 bytes, tarefa e TLS usam RAM dinâmica adicional. Aplicação: aproximadamente 1,15 MB, dentro da partição OTA de 1.310.720 bytes.
- Ambiente de build: instalação global PlatformIO. Cache com caminho longo falhou ao iniciar ferramenta interna do GCC; caminho global resolveu.
- Scripts Python: sintaxe aprovada.
- Testes físicos e integração com a rede do usuário: pendentes conforme checklist.

## Correcao do clima

Mensagens de estado especificas, primeira consulta condicionada a NTP e retentativa inicial de 15 segundos. Campos de coordenadas ampliados para 24 caracteres. API status inclui weather_enabled, weather_status e weather_http. Validacao fisica com as coordenadas do usuario ainda pendente.

## Atualizacao de radio

Wi-Fi ampliado para top 24 com dBm e escala relativa. Bluetooth BLE/classico com resultado limitado a 24, deduplicacao por endereco/tipo, busca assincrona e liberacao do controlador. Tarefa de integracoes pausa e retoma automaticamente. Testes do parser de nomes incluem payload truncado, campo vazio, nome curto/completo, limite de 32 bytes e terminador NUL. Descobertas reais e consumo durante scan ainda requerem bancada. Novo layout 4MB com duas particoes OTA de 1.966.080 bytes requer primeiro upload por USB.

Build com radio aprovado: aproximadamente 1,88 MB, RAM estatica 70.644 bytes (21,6%). Testes C++ de parser Bluetooth e nucleo aprovados. Reserva de memoria BT preservada no boot pelo HAL Arduino; medicao dinamica do scan ainda pendente.

## Remocao dos jogos

Snake e Pong removidos das fontes, do painel e do menu; testes atuais verificam botoes e parser Bluetooth. Resultados anteriores acima pertencem as versoes anteriores. IDs e dados de configuracao dos demais modulos preservados.

## Clima 1.0.1

Eliminada a duplicidade de habilitacao: flag do modulo Clima determina as consultas. Configuracoes anteriores conciliadas ao carregar NVS; coordenadas e credenciais preservadas. Serial inclui ativo, NTP, HTTP e mensagem a cada 10 segundos. Validacao de consulta na placa permanece pendente do upload.
