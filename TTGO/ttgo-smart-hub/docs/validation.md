# Checklist de bancada

Registrar placa/revisão física, data e versão. Itens pendentes não são aprovação implícita.

- [ ] Confirmar PCB da T-Display clássica e pinagem ST7789.
- [ ] Após upload, serial mostra `TTGO Smart Hub 1.0.0`, display 240×135 e NVS OK.
- [ ] Menu legível e cores corretas; navegação pelos dois botões.
- [ ] Primeiro boot: AP protegido e senha disponível no serial/tela.
- [ ] Login inválido não abre painel; login válido abre.
- [ ] Salvar Wi-Fi e reiniciar: conectar, obter IP, sincronizar data/hora e preservar ajustes.
- [ ] Fechar painel, buscar redes e navegar entre resultados com RSSI/canais corretos.
- [ ] Configurar latitude/longitude e clima: dados atualizam, erro de rede preserva último resultado com aviso.
- [ ] Configurar broker: evento recebido, alerta criado e telemetria publicada.
- [ ] MQTT TLS com NTP e CA válida; broker com CA não confiável falha.
- [ ] HA: entidade consultada, confirmação necessária antes de toggle, erro 401/403 não repete comando.
- [ ] API de alertas: chave inválida 401; payload inválido 400; texto válido 202; chamadas rápidas 429.
- [ ] OTA só disponível com Wi-Fi/painel ativos; senha incorreta falha; imagem correta inicia depois do reset.
- [ ] AP/painel fecha após 10 min quando SSID está configurado, e permanece no primeiro provisionamento.
- [ ] Desligar AP doméstico: menu continua; reconexão quando AP retorna.
- [ ] Teste contínuo por 60 min com integrações: minheap suficiente, sem resets ou queda contínua de heap.

Log periódico inclui uptime, heap atual e mínimo. Falhas HTTP, NTP ou MQTT precisam de validação com os seus serviços. Atualização OTA pode pausar o uso normal da interface e reinicia o firmware ao completar.

## Radio

- [ ] USB grava novo layout de partições e preserva SSID/credenciais e opção do clima.
- [ ] Wi-Fi Analyzer lista até 24 redes ordenadas, páginas, dBm, porcentagem e canal.
- [ ] Bluetooth aparece no menu após migração; painel permite habilitar/desabilitar.
- [ ] BLE detecta um anunciante conhecido; clássico detecta dispositivo em modo visível.
- [ ] Nome ausente mostra (sem nome); resultados mostram endereço, BLE/BT e sinal quando disponível.
- [ ] Durante busca, interface responde; ao acabar, MQTT/HA/clima retomam.
- [ ] Repetir 10 buscas, inclusive sem dispositivos: sem watchdog/reset ou perda contínua de heap.
