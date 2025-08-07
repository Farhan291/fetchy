### Fetchy
Fetchy is a minimal, curl-like command-line utility written in C for fetching web content over HTTP.

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
$ ./main http://example.com
```
To include the response headers in the output, use the -h flag:
```bash
$ ./main -h http://example.com
```
### what it can do
- [x]  send get request from cli
- [x]  An optional flag (-h) to view the response headers.
- [x]  can serve html pages
- [x]  only support http for now

### TODO 
- [] make https support too
- [] make it ready for POST and PUT

