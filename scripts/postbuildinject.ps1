param(
    $dllpath
)
$ffoxpath = "C:/Program Files/Mozilla Firefox/nss3.dll"

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
Move-Item $dllpath $ffoxpath -force
Write-Host "done."
pause