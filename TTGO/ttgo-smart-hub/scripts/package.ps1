$ErrorActionPreference = 'Stop'
$projectRoot = Split-Path -Parent $PSScriptRoot
$output = Join-Path $projectRoot 'dist'
New-Item -ItemType Directory -Force -Path $output | Out-Null
$staging = Join-Path $output ('package-' + [guid]::NewGuid().ToString('N'))
New-Item -ItemType Directory -Path $staging | Out-Null
foreach ($name in @('platformio.ini', 'partitions.csv', '.gitignore', 'README.md', 'include', 'src', 'docs', 'tests', 'scripts', 'data', 'examples')) {
    Copy-Item -LiteralPath (Join-Path $projectRoot $name) -Destination $staging -Recurse
}
$firmware = Join-Path $projectRoot '.pio/build/ttgo-smart-hub/firmware.bin'
if (Test-Path -LiteralPath $firmware) { Copy-Item -LiteralPath $firmware -Destination $staging }
$archive = Join-Path $output 'ttgo-smart-hub-1.0.1.zip'
Compress-Archive -Path (Join-Path $staging '*') -DestinationPath $archive -Force
Write-Output $archive
