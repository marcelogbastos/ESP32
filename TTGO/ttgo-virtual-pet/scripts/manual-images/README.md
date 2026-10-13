# Gerar imagens do manual

O mock TFT captura as chamadas do `src/display/Screen.cpp` real em WebAssembly. O Node grava as operações e `render.py` desenha as ilustrações em PNG com Pillow. Fontes e rasterização do host aproximam a tela; não são fotografias/capturas físicas.

Gerar a partir da raiz do projeto com Clang que inclua wasm-ld, Node e Python com Pillow:

```powershell
New-Item -ItemType Directory -Force dist
clang --target=wasm32 -x c++ -std=c++11 -ffreestanding -fno-exceptions -nostdlib -Iscripts/manual-images -Iinclude scripts/manual-images/driver.cpp src/display/Screen.cpp src/pet/Pet.cpp -Wl,--no-entry -Wl,--export=render -Wl,--export-memory -Wl,--allow-undefined -o dist/manual-render.wasm
node scripts/manual-images/capture.cjs
python scripts/manual-images/render.py
```

O renderer usa Consolas/Segoe UI de `C:/Windows/Fonts`. Adapte os caminhos de fontes em outro sistema. O mock é exclusivo das ilustrações e não entra no build do firmware.
