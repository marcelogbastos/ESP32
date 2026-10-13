# Validação de bancada — Fase 1

Registrar data, revisão impressa na PCB, marcação do ESP32, porta USB e resultado de cada item. O firmware não consegue descobrir a revisão física da PCB.

- [ ] Conferir revisão V1.1 e comparar trilhas/pinos com esquema do fabricante antes de gravar.
- [ ] Build sem erros para ESP32 clássico; registrar versões e consumo de RAM/flash.
- [ ] Log serial informa ESP32, tamanho de flash e display 240 × 135.
- [ ] Borda completa, texto legível e barras vermelho/verde/azul na ordem correta.
- [ ] GPIO35 avança uma única vez por toque; manter pressionado não repete.
- [ ] GPIO0 abre/volta uma única vez por toque.
- [ ] Selecionar Teste de hardware, reiniciar e confirmar seleção restaurada e NVS OK.
- [ ] Sem secrets: menu funciona e Wi-Fi permanece desligado.
- [ ] Com credenciais: IP aparece; desligar AP e religar; reconexão deve ocorrer sem travar botões.
- [ ] Executar por 30 minutos; registrar heap a cada 10 s e verificar ausência de queda contínua/reset.

Em branco significa não validado, nunca aprovação implícita. Não avançar a integrações de controle ou OTA antes desta conferência.
