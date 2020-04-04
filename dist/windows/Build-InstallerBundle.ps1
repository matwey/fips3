param (
    $SourceRoot = "$PSScriptRoot/../..",
    $BundleUpgradeCode = 'b1f6b092-eeec-41c6-bcd2-a0789b1342f4',

    $candle = 'candle',
    $light = 'light'
)

Set-StrictMode -Version Latest
$ErrorActionPreference = 'Stop'

$msiInstaller = @(Get-ChildItem $SourceRoot/*.msi)
if ($msiInstaller.Count -ne 1) {
    throw "Found $($msiInstaller.Count) *.msi files in $SourceRoot, while expected exactly 1"
}

if (-not ($msiInstaller[0].Name -match '^Fips-(.*?)-(.*?).msi$')) {
    throw "Cannot parse installer name: $($msiInstaller[0].Name)"
}
$version = $Matches[1]
$platform = $Matches[2]

& $candle -ext WixBalExtension "-dVERSION=$version" "-dPLATFORM=$platform" "-dUPGRADE_CODE=$BundleUpgradeCode" "$SourceRoot\dist\windows\InstallerBundle.wxs"
if (!$?) {
    throw 'candle execution error while building the installer'
}
$output = "Fips-$version-$platform.bundle.exe"
& $light -ext WixBalExtension -out $output "$SourceRoot\InstallerBundle.wixobj"
if (!$?) {
    throw 'light execution error while building the installer'
}
Write-Output "Built installer: $output"
