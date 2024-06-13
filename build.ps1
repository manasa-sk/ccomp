$sourceFile = "main.c"
$headerFile = "lexer.c"
$outputFile = "main.exe"
gcc $sourceFile $headerFile -o $outputFile
Start-Process -NoNewWindow -Wait -FilePath ".\$outputFile"