$error_ = 0

$error_++
MSBuild /property:Configuration=Release /property:Platform=x86 -t:Clean || Exit $error_

$error_++
MSBuild /property:Configuration=Release /property:Platform=x86 -t:Build || Exit $error_

$error_++
Test-Path ".\Release\fctocf.exe" -PathType Leaf || Exit $error_

$error_++
Move-Item -Path ".\Release\fctocf.exe" -Destination ".\Release\fctocf-x86.exe" -Force || Exit $error_

$error_++
MSBuild /property:Configuration=Release /property:Platform=x64 -t:Clean || Exit $error_

$error_++
MSBuild /property:Configuration=Release /property:Platform=x64 -t:Build || Exit $error_


$error_++
Test-Path ".\x64\Release\fctocf.exe" -PathType Leaf || Exit $error_

$error_++
Move-Item -Path ".\x64\Release\fctocf.exe" -Destination ".\x64\Release\fctocf-x64.exe" -Force || Exit $error_