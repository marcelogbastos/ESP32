# Resultados — 12/10/2026

- Build PlatformIO aprovado: ESP32 classico, Espressif32 6.9.0, Arduino 2.0.17 e TFT_eSPI 2.5.43.
- RAM estatica: 22.244 bytes (6,8%); sprite usa 32.400 bytes adicionais de heap.
- Aplicacao: 315.133 bytes (24,0% da particao).
- Testes C++ executados: atributos, moedas, sono, limites, evolucao, recuperacao, remedio e rejeicao de estados invalidos aprovados.
- ZIP conferido com firmware e fontes.
- Validacao fisica de tela, botoes e NVS pendente; firmware nao foi gravado automaticamente.

## Animacoes 1.1.0 — 13/10/2026

Cuidados com animacoes independentes de temporizacao: comer, brincar, banho e remedio. Testes C++ cobrem inicio, termino, reentrada e rollover de millis, alem das regras existentes do pet. Build aprovado com RAM estatica de aproximadamente 22 KB e imagem de aproximadamente 317 KB. Aparencia e fluidez na tela ainda precisam de validacao fisica apos upload; nao houve gravacao automatica.

## Versao 2.0 — 13/10/2026

Build aprovado: imagem 328.993 bytes, RAM estatica 22.428 bytes (6,8%), sprite dinamico 32.400 bytes. Testes C++ aprovados para migracao v1, preservacao de idade/moedas/atributos/sono, humores, evolucao condicionada a cuidados, variantes adultas, objetivos, resgate unico, troca de dia ligado, compras sem cobranca duplicada, personalizacao e duracao de animacoes. Home possui somente personagem e efeitos; controles em telas auxiliares. PWM e aparencia final precisam de validacao na placa. Firmware nao foi gravado automaticamente.

## Versao 2.1 — 13/10/2026

Builds aprovados: `ttgo-virtual-pet` (imagem 334.861 bytes, RAM estatica 22.764 bytes) e `ttgo-virtual-pet-lowpower` (imagem 352.737 bytes, RAM estatica 24.092 bytes). Testes C++ (compilados no host com clang via zig) aprovados, incluindo eventos, recompensas e rodadas do mini-jogo, CRC-32, texto de backup e zeragem de bytes reservados. Nao executado na placa: tela do mini-jogo, console serial, light sleep e NVS com CRC/backup. Firmware nao foi gravado automaticamente.
