$ErrorActionPreference = 'Stop'
$root = Split-Path -Parent $PSScriptRoot
$output = Join-Path $root 'dist'
New-Item -ItemType Directory -Path $output -Force | Out-Null
$staging = Join-Path $output ('package-' + [guid]::NewGuid().ToString('N'))
New-Item -ItemType Directory -Path $staging | Out-Null
foreach ($name in @('platformio.ini','.gitignore','README.md','docs','include','src','tests','scripts')) {
    Copy-Item -LiteralPath (Join-Path $root $name) -Destination $staging -Recurse
}
$firmware = Join-Path $root '.pio/build/ttgo-virtual-pet/firmware.bin'
if (Test-Path -LiteralPath $firmware) { Copy-Item -LiteralPath $firmware -Destination $staging }
$archive = Join-Path $output 'ttgo-virtual-pet-2.0.0.zip'
Compress-Archive -Path (Join-Path $staging '*') -DestinationPath $archive -Force
Write-Output $archive
