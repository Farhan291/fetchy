### Fetchy
Fetchy is a minimal, curl-like command-line utility written in C for fetching web content over HTTP/HTTPS.

### building

Clone the repository and run make in the project's root directory:
```bash
$ git clone https://github.com/Farhan291/fetchy.git
$ cd fetchy
$ make 
```

### usage

Provide a URL as a command-line argument.

To fetch a URL and display only its body:
```bash
$ ./main https://man.archlinux.org/ 
```
To include the response headers in the output, use the -h flag:
```bash
$ ./main -h https://man.archlinux.org/ 
```
### what it can do
- [x]  send get request from cli
- [x]  An optional flag (-h) to view the response headers.
- [x]  can serve html pages
- [x]  support https too

### TODO 

- [] make it ready for POST and PUT
- [] parse the html page 

