# ESP32
Projetos para utilizando o ESP32
Datasheet: https://www.espressif.com/sites/default/files/documentation/esp32_datasheet_en.pdf

Referencias:
  https://www.fernandok.com/2018/03/esp32-detalhes-internos-e-pinagem.html
  https://www.curtocircuito.com.br/blog/conhecendo-esp32/

## Organização dos projetos

- `TTGO/`: projetos para placas LILYGO TTGO; projeto atual em `TTGO/ttgo-control-center/`.
- `Outros/`: projetos para outras placas ESP32; projeto atual em `Outros/TemperaturaBlynk/`.

Cada novo projeto deve ter sua própria subpasta na categoria correspondente. Para PlatformIO, abra a pasta do projeto que contém `platformio.ini`.

- `TTGO/ttgo-smart-hub/`: hub com clima, Wi-Fi Analyzer, MQTT, alertas, Home Assistant e Bluetooth.

- `TTGO/ttgo-virtual-pet/`: bichinho virtual Pip, cuidados, evolucao e progresso salvo.
