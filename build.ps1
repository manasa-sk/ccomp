$sourceFile = "main.c"
$headerFile1 = "lexer.c"
$headerFile2 = "parser.c"
$outputFile = "main.exe"
gcc $sourceFile $headerFile1 $headerFile2 -o $outputFile
Start-Process -NoNewWindow -Wait -FilePath ".\$outputFile"