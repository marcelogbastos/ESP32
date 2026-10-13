param([Parameter(Mandatory = $true)][string]$Clang, [string]$Node = 'node')
$ErrorActionPreference = 'Stop'
Push-Location (Split-Path -Parent $PSScriptRoot)
$previousTemp = $env:TEMP
$previousTmp = $env:TMP
try {
    New-Item -ItemType Directory -Force -Path dist | Out-Null
    $env:TEMP = Join-Path (Get-Location) 'dist'
    $env:TMP = $env:TEMP
    & $Clang --target=wasm32 -x c++ -std=c++11 -ffreestanding -fno-exceptions -nostdlib -Iinclude tests/core_test.cpp '-Wl,--no-entry' '-Wl,--export=main' -o dist/core_test.wasm
    if ($LASTEXITCODE -ne 0) { throw 'Falha ao compilar testes C++' }
    & $Node -e "const fs=require('fs'); WebAssembly.instantiate(fs.readFileSync('dist/core_test.wasm')).then(({instance})=>{const line=instance.exports.main(); if(line) throw Error('Falha em tests/core_test.cpp:'+line); console.log('C++: botoes e parser Bluetooth aprovados');});"
    if ($LASTEXITCODE -ne 0) { throw 'Testes falharam' }
} finally { $env:TEMP = $previousTemp; $env:TMP = $previousTmp; Pop-Location }
