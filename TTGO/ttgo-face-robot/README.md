# TTGO Face Robot
Firmware 1.0.0 para LILYGO TTGO T-Display V1.1, ESP32 clássico, ST7789 240 × 135.
Tela somente com o rosto animado, fundo preto. Funciona sem Wi-Fi, APIs, SD ou PSRAM.

## Instalação
Abra esta pasta no VS Code com PlatformIO. Conecte por USB, feche o monitor serial e execute:
~~~powershell
pio run
pio run --target upload --upload-port COM3
pio device monitor --port COM3 --baud 115200
~~~
Substitua COM3 pela porta real. Se necessário, mantenha BOOT pressionado durante Connecting e solte quando começar a gravação.
Não utilize em T-Display S3. O binário em dist é somente a aplicação; na primeira gravação use PlatformIO para também instalar bootloader e partições.

## Controles
Ao ligar, inicia Normal e troca automaticamente a cada oito segundos.

| Botão | Clique | Segurar 700 ms |
|---|---|---|
| GPIO35 | Próxima expressão e modo manual | Liga/desliga troca automática |
| GPIO0/BOOT | Próxima cor | Liga/desliga pupilas |

Cores: ciano, verde, branco, laranja e violeta. Pupilas se aplicam aos olhos comuns.
Clique ocorre ao soltar; pressão longa não produz clique adicional. Configurações voltam ao padrão ao reiniciar.
O monitor serial mostra expressão, modo, paleta e pupilas; a tela permanece limpa.

## Faces selecionadas
Adaptação das formas CSS fornecidas, com tamanho e movimentos ajustados à tela pequena.

| Face | Referência | Efeito |
|---|---|---|
| Normal | buddy-normal | Respiração, olhar lateral, piscadas |
| Feliz | new-02 | Olhos em arco e sorriso |
| Rindo | new-05 | Olhos em arco e boca aberta |
| Triste | new-27 | Pálpebras inclinadas e lágrimas |
| Bravo | new-15 | Pálpebras inclinadas e boca reta |
| Surpreso | new-16 | Olhos grandes e boca em O |
| Piscando | winking | Piscadela direita periódica |
| Dormindo | new-42 | Olhos fechados e z flutuante |
| Apaixonado | new-13 | Corações pulsantes |
| Estrelado | new-08 | Estrelas giratórias |
| Óculos | new-35 | Óculos e sorriso |
| Erro | new-57 | Olhos em X |

As faces têm movimento vertical suave. Olhos comuns abrem ao entrar e piscam periodicamente.
index.html, style.css, new_faces.css e script.js foram preservados como referência.
O firmware desenha com primitivas TFT; não reproduz áudio ou brilho CSS. As abas PNG/SVG da referência dependem de imagens não presentes nesta pasta.

## Hardware
| Sinal | GPIO |
|---|---:|
| MOSI | 19 |
| SCLK | 18 |
| CS | 5 |
| DC | 16 |
| Reset | 23 |
| Backlight | 4 |
| Navegação | 35 |
| Ação/BOOT | 0 |

GPIO35 usa o pull-up externo da placa. Rotação 1; dimensões nativas 135 × 240.
Dependências fixadas: espressif32 6.9.0, Arduino e TFT_eSPI 2.5.43.
Perfil lilygo-t-display conserva flash de 4 MB mesmo em placas com flash maior.
Aviso TOUCH_CS é esperado: a placa não possui touch.

## Arquitetura
- src/main.cpp: inicialização, entrada e agendamento.
- include/input/Button.h: debounce de 30 ms e pressão longa.
- include/faces/FaceState.h: sequência, modo automático e tempo.
- src/FaceRenderer.cpp: desenho das expressões.
- test/state_test.cpp: sequência, modo manual, rollover de millis, piscadas e botões.

~~~mermaid
flowchart LR
 Buttons[GPIO 35 e 0] --> Input[Button]
 Input --> State[FaceState]
 Clock[millis] --> State
 State --> Renderer[FaceRenderer]
 Renderer --> Sprite[Sprite 8 bits]
 Sprite --> Display[ST7789]
~~~

Frames a cada 33 ms, sem esperas de animação; delay de 1 ms permite atendimento das tarefas ESP32.
Sprite 240 × 135 ocupa aproximadamente 32,4 KB de heap além das estruturas TFT.
Não há alocação dinâmica por frame na aplicação. Falha de sprite exibe mensagem e mantém o loop responsivo.

## Testes
Compilação: pio run. Teste de lógica com Clang para WebAssembly e Node:
~~~powershell
clang --target=wasm32 -std=c++11 -nostdlib -Iinclude test/state_test.cpp "-Wl,--no-entry" "-Wl,--export=runTests" -o dist/state_test.wasm
node scripts/test-state.cjs dist/state_test.wasm
~~~
Crie dist antes de executar o teste caso tenha baixado somente os fontes.
Na placa, confira orientação, todas as expressões, cores, clique/pressão longa e funcionamento automático por alguns minutos.
Compilação e testes de lógica não substituem a inspeção física da animação.

## Personalização e distribuição
Altere desenhos em FaceRenderer.cpp, duração em FaceState.h e cores no array colors.
Uma nova expressão exige atualizar enum, desenho, nomes do log e documentação.
dist contém firmware.bin e o ZIP com fontes, referência e firmware.
