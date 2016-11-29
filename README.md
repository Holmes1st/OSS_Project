# ORIGIN Project

[<img src="https://avatars1.githubusercontent.com/u/13138166?v=3&s=400" width="20"> AishaNair/Key-Logger](https://github.com/AishaNair/Key-Logger)



# Team 성재와 노예들

> [<img src="https://avatars2.githubusercontent.com/u/22092657?v=3&s=460" width='20'> 민성재 (주인)](https://github.com/WoWsj) wowtjdwo@gmail.com

> [<img src="https://avatars2.githubusercontent.com/u/23147086?v=3&s=460" width="20"> 이영우 (노예)](https://github.com/youngwoo013) dbsgh013@gmail.com

> [<img src="https://avatars3.githubusercontent.com/u/19491469?v=3&s=460" width="20"> 장세영 (노예)](https://github.com/seyoung0916) seyoung0916@naver.com

> [<img src="https://avatars1.githubusercontent.com/u/11886612?v=3&s=460" width="20"> 방일섭 (노예)](https://github.com/Holmes1st) forresti3979@gmail.com

> [<img src="https://avatars0.githubusercontent.com/u/22047213?v=3&s=460" width="20"> 박준영 (노예)](https://github.com/awrsole) pjy9605@gmail.com

## How to build
```bash
$ git clone https://github.com/Holmes1st/OSS_Project.git YOUR_REPO
$ cd YOUR_REPO
$ sudo make             # you need root permission
```

## How to run

```bash
$ # Keylogger enable
$ sudo ./klg_load.sh    # you need root permission
```

```bash
$ # Keylogger disable
$ sudo ./klg_unload.sh  # you need root permission
```

### How to see Keyloger file
```sh
$ cat /dev/klg
       or
$ ./test/example/client.py localhost 31337
```

### How to change accept_ip & port
edit key-logger/klogger.h
```c
#define PEER_IP   "127.0.0.1"
#define PORT      "31337"
```
to
```c
#define PEER_IP  "IPNUMBER"
#define PORT     "PORTNUMBER"
```

### How to see Kernel Log file
```sh
$ sudo cat /proc/kmsg
```
