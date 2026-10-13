# TTGO Face Robot
Firmware 1.1.1 para LILYGO TTGO T-Display V1.1, ESP32 clássico, ST7789 240 × 135.
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
Ao ligar, inicia Neutro e troca automaticamente a cada oito segundos.

| Botão | Clique | Segurar 700 ms |
|---|---|---|
| GPIO35 | Próxima expressão e modo manual | Liga/desliga troca automática |
| GPIO0/BOOT | Próxima cor | Liga/desliga pupilas |

Cores: ciano, verde, branco, laranja e violeta. Pupilas se aplicam aos olhos comuns.
Clique ocorre ao soltar; pressão longa não produz clique adicional. Configurações voltam ao padrão ao reiniciar.
O monitor serial mostra expressão, modo, paleta e pupilas; a tela permanece limpa.

## Faces padronizadas
As 24 expressões compartilham olhos arredondados de 52 pixels de largura, centros fixos em X=72 e X=168, mesma cor e traço de boca. A emoção varia por altura dos olhos, inclinação das pálpebras, olhar e boca.

Sequência: Neutro, Feliz, Muito feliz, Rindo, Bravo, Furioso, Triste, Chorando, Surpreso, Assustado, Sonolento, Dormindo, Piscando, Curioso, Pensando, Desconfiado, Confuso, Tímido, Carinhoso, Animado, Entediado, Concentrado, Preocupado e Travesso.

Os nomes Bravo/Triste e Furioso/Chorando seguem as correções aprovadas na prévia.
Somente Chorando tem lágrimas. A lágrima direita foi deslocada 12 pixels para a esquerda, ficando 10 pixels à direita do centro do olho.

As faces respiram com movimento vertical suave e piscam periodicamente; Dormindo mantém os olhos fechados, e Piscando faz uma piscadela direita.
Pupilas começam ativadas, com movimento suave horizontal/vertical e pequeno reflexo branco. São recortadas pelas pálpebras e diminuem nos olhos estreitos; desaparecem em olhos fechados. Segure GPIO0 por 700 ms para desativá-las ou reativá-las.
O firmware desenha diretamente em C++; não depende de HTML, CSS ou JavaScript.
A prévia usa curvas do navegador e o firmware usa pixels da TFT, portanto pequenas diferenças de contorno são esperadas.
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
- include/faces/FaceStyle.h: modelo comum e parâmetros das 24 faces.
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
dist contém firmware.bin e o ZIP com fontes, testes, documentação e firmware.




