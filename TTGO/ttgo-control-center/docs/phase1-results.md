# Resultados — 12/10/2026

## Hardware consultado (sem gravar aplicação)

Interface CH9102 na COM3. Consultas `esptool chip_id` e `flash_id`:

- ESP32-D0WDQ6-V3, revisão de silício 3.0, dual core, 240 MHz.
- Cristal normalizado em 40 MHz. A primeira consulta estimou 41,01 MHz e emitiu aviso; a segunda reconheceu 40 MHz sem aviso.
- Flash detectada: **16 MB**, fabricante `ef`, dispositivo `4018`.

Isso identifica o microcontrolador conectado, **não comprova que a placa seja T-Display V1.1**. A flash detectada difere dos 4 MB do perfil padrão PlatformIO. Mantido o perfil conservador de 4 MB e suas partições, sem usar a capacidade adicional. Confirmar marcações da PCB, display e esquema antes do upload. As consultas carregam um stub em RAM e reiniciam a placa ao terminar; não escrevem a aplicação na flash.

## Testes de software

- Perfil configurado para ESP32 clássico/ST7789: aprovado.
- Credenciais reais ausentes: aprovado.
- Classe C++ Button compilada com LLVM para WebAssembly e executada no Node: todos os casos aprovados (bounce, pressão mantida, liberação, partida pressionada e rollover).
- Build PlatformIO: **aprovado**, Espressif32 6.9.0, Arduino ESP32 2.0.17 (pacote 3.20017.241212), TFT_eSPI 2.5.43.
- RAM estática: 43.836 / 327.680 bytes (13,4%). Flash da aplicação: 752.473 / 1.310.720 bytes (57,4% da partição). RAM dinâmica durante Wi-Fi precisa ser medida na bancada.
- Aviso TFT_eSPI sobre TOUCH_CS ausente: esperado, pois o perfil não tem touchscreen.
- Binário gerado em `.pio/build/ttgo-t-display/firmware.bin`.

## Pendências físicas

Revisão V1.1, pinagem desta unidade, cores e orientação do display, botões físicos, persistência NVS, conexão/reconexão Wi-Fi e estabilidade por 30 minutos ainda não validados. Não houve upload da aplicação.
