param([Parameter(Mandatory = $true)][string]$Clang, [string]$Node = 'node')
$ErrorActionPreference = 'Stop'
Push-Location (Split-Path -Parent $PSScriptRoot)
$previousTemp = $env:TEMP; $previousTmp = $env:TMP
try {
    New-Item -ItemType Directory -Path dist -Force | Out-Null
    $env:TEMP = Join-Path (Get-Location) 'dist'; $env:TMP = $env:TEMP
    & $Clang --target=wasm32 -x c++ -std=c++11 -ffreestanding -fno-exceptions -nostdlib -Iinclude tests/pet_test.cpp src/pet/Pet.cpp '-Wl,--no-entry' '-Wl,--export=main' -o dist/pet_test.wasm
    if ($LASTEXITCODE -ne 0) { throw 'Falha ao compilar teste' }
    & $Node -e "const fs=require('fs'); WebAssembly.instantiate(fs.readFileSync('dist/pet_test.wasm')).then(({instance})=>{const code=instance.exports.main(); if(code) throw Error('Falha linha '+code); console.log('Motor do bichinho: testes aprovados');});"
    if ($LASTEXITCODE -ne 0) { throw 'Teste falhou' }
} finally { $env:TEMP = $previousTemp; $env:TMP = $previousTmp; Pop-Location }
