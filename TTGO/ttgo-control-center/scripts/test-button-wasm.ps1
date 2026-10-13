param(
    [Parameter(Mandatory = $true)][string]$Clang,
    [string]$Node = 'node'
)
$ErrorActionPreference = 'Stop'
$projectRoot = Split-Path -Parent $PSScriptRoot
Push-Location $projectRoot
try {
    New-Item -ItemType Directory -Force -Path dist | Out-Null
    $env:TEMP = Join-Path $projectRoot 'dist'
    $env:TMP = $env:TEMP
    & $Clang --target=wasm32 -x c++ -std=c++11 -ffreestanding -nostdlib -Iinclude tests/button_test.cpp '-Wl,--no-entry' '-Wl,--export=main' -o dist/button_test.wasm
    if ($LASTEXITCODE -ne 0) { throw 'Falha ao compilar teste' }
    & $Node -e "const fs=require('fs'); WebAssembly.instantiate(fs.readFileSync('dist/button_test.wasm')).then(({instance})=>{const code=instance.exports.main(); if(code) throw Error('Falha linha '+code); console.log('Debounce C++: todos os casos passaram');});"
    if ($LASTEXITCODE -ne 0) { throw 'Teste de debounce falhou' }
} finally { Pop-Location }
