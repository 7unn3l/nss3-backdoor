param(
    $dllpath
)
$ffoxpath = "C:/Program Files/Mozilla Firefox/nss3.dll"
$ffoxpath_orig = "C:/Program Files/Mozilla Firefox/nss3.orig"

# Self-elevate the script if required
if (-Not ([Security.Principal.WindowsPrincipal] [Security.Principal.WindowsIdentity]::GetCurrent()).IsInRole([Security.Principal.WindowsBuiltInRole] 'Administrator')) {
    if ([int](Get-CimInstance -Class Win32_OperatingSystem | Select-Object -ExpandProperty BuildNumber) -ge 6000) {
    $CommandLine = " -ExecutionPolicy Bypass -NoProfile -File `"" + $MyInvocation.MyCommand.Path + "`" " + $dllpath
    Write-Host $CommandLine
    Start-Process -WorkingDirectory "C:/" -FilePath PowerShell.exe -Verb Runas -ArgumentList $CommandLine
    Exit
    }
}

Write-Host "killing firefox"
taskkill /IM firefox.exe /F
Write-Host "injecting nss3.dll into firefox path"
Copy-Item $dllpath $ffoxpath -force
Write-Host "checking if nss3.orig exists..."

if (![System.IO.File]::Exists($ffoxpath_orig)){
    Write-Host "nss3.orig does not exist, copying..."
    $oirg_path = $MyInvocation.MyCommand.Path + '/../../ext/nss3_original.dll'
    Copy-Item $oirg_path $ffoxpath_orig
}

Write-Host "done."
pause