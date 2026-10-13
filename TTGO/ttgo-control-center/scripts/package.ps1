$ErrorActionPreference = 'Stop'
$projectRoot = Split-Path -Parent $PSScriptRoot
$outputDir = Join-Path $projectRoot 'dist'
New-Item -ItemType Directory -Force -Path $outputDir | Out-Null
$archivePath = Join-Path $outputDir 'ttgo-control-center-phase1.zip'
$items = @('platformio.ini', '.gitignore', 'README.md', 'docs', 'tests', 'scripts', 'src') |
    ForEach-Object { Join-Path $projectRoot $_ }
$staging = Join-Path $outputDir 'package'
New-Item -ItemType Directory -Force -Path $staging | Out-Null
# Explicit file list keeps credentials and PlatformIO caches out of the archive.
foreach ($item in $items) { Copy-Item -LiteralPath $item -Destination $staging -Recurse -Force }
$includeDir = Join-Path $staging 'include'
New-Item -ItemType Directory -Force -Path $includeDir | Out-Null
foreach ($name in @('board.h', 'secrets.example.h', 'core', 'input', 'network')) {
    Copy-Item -LiteralPath (Join-Path $projectRoot "include/$name") -Destination $includeDir -Recurse -Force
}
$firmware = Join-Path $projectRoot '.pio/build/ttgo-t-display/firmware.bin'
if (Test-Path -LiteralPath $firmware) { Copy-Item -LiteralPath $firmware -Destination $staging -Force }
Compress-Archive -Path (Join-Path $staging '*') -DestinationPath $archivePath -Force
Write-Output $archivePath
