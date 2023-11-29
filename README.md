# System Programming Assignments

## Summary

This is a programming test assignment given to me by Prof. Shinjo. I created this private repository for easy documentation and code management.

I chose Assignments 3, 4, 5, and 6. This class is really interesting, and in the future, if I have some free time, I will most likely finish the other assignments as well.

The main development environment was an Ubuntu 22.04.1 system on Github Codespaces. During this time, all code was also tested on my laptop running Ubuntu 22.04.3 LTS (12.0GiB RAM and Celeron N5100).

## Assignment3

### 302


#### Improvements:

- Removed unnecessary test files to clean up the workspace.
- Implemented error checking mechanisms to improve program robustness.
  
#### Basic Usage:

```bash
./FileCopier <source file> <destination file>
```

#### Output:

First we generate a random file of 10MB.

```bash
@BH3GEI ➜ /workspaces/SystemProgrammingAssignments/Assignment3/302 (main) $  dd if=/dev/urandom of=testfile bs=1M count=10
10+0 records in
10+0 records out
10485760 bytes (10 MB, 10 MiB) copied, 0.0464821 s, 226 MB/s
```

Then compile and run the program I wrote.

```bash
@BH3GEI ➜ /workspaces/SystemProgrammingAssignments/Assignment3/302 (main) $  gcc -o FileCopier FileCopier.c 
@BH3GEI ➜ /workspaces/SystemProgrammingAssignments/Assignment3/302 (main) $  ./FileCopier testfile testfile1
Time taken for file copy (BUFSIZ): 0.022660 seconds
Time taken for file copy (char): 65.742096 seconds
```
We can see that there is a considerable difference in performance between the two ways of copying files.

### 303

#### Improvements:

- Replaced the `putchar()` function with `write()` system call for better efficiency and error handling.

#### Basic Usage:

```bash
./HeadViewer [file] [num of lines]
./TailViewer [file] [num of lines]
```

file: The specific document the user wishes to examine.

num of lines: The count of lines to be displayed from the commencement or conclusion of the document.

#### Output:
First we try to generate and read a relatively long file.

```bash
@BH3GEI ➜ /workspaces/SystemProgrammingAssignments/Assignment3/303 (main) $  head -c 75K < /dev/urandom | base64 > testfile.txt
```
```bash
@BH3GEI ➜ /workspaces/SystemProgrammingAssignments/Assignment3/303 (main) $ gcc -o TailViewer TailViewer.c
@BH3GEI ➜ /workspaces/SystemProgrammingAssignments/Assignment3/303 (main) $ gcc -o HeadViewer HeadViewer.c
```
```bash
@BH3GEI ➜ /workspaces/SystemProgrammingAssignments/Assignment3/303 (main) $ ./head testfile.txt 5
AY6+9rpd9gq3RF/IB5HAAq0UpVAc1Sx5v8ioLHRtxHbduBH63CexdvWa/lpyeRFmmd5Un7/ugRSw
OF8IQ2yZLTQNTAJLmn95ECh3YFLOTaeDSACv0c051uK/8N3lpSBsnCeiBkxZglxfyMGZqh/ymnhu
2s+Ag4cIoQl3wtVpWSutfsK7tnBRdu/vXZ3kibgxetKnCd9jud10L3cSVr94LodkStKv8jUN7YKF
q5K7mI/9buoAK9gQaBriXeXWEyiaWS7pBLRkUjYFV7CKMneP+eWXZGl1+ANpZqlSaPklVzUxwzSu
usblKYWNV57zBP1bdgFP4sHV1hzcnFKoPvC+5O0So9lmdFcVGYOmQZ2ImUs/oxGiJnaopDpapstP
```

```bash
@BH3GEI ➜ /workspaces/SystemProgrammingAssignments/Assignment3/303 (main) $ ./tail testfile.txt 5
bRVQgDtY6SpJb7KZXUGmGmbMm78Ip20TNDCM/PC4Lz8TixF6SfAzjBAzD8QcIU6V1b8GiOY8Gflf
5LGXSKnbflh19Z8KYjzzdDIqT6kf4yeccv0poMSJbWmO3awKK1FPDb4Ww1tQCFRQFJuCGcwm4Zya
UJg2l+gCVXeUUfzYdpRE0oeEhzjSEx8feRuDMXWvh4aSHAVV9yOTVYKpNejvlsH2tibBpBTetFI2
0TRVRG4L+nt2lxn2fb05Nhjgfb0y
```

Then a shorter file with only one line.

```bash
@BH3GEI ➜ /workspaces/SystemProgrammingAssignments/Assignment3/303 (main) $ head -c 10 < /dev/urandom | base64 > testfile.txt
```



```bash
@BH3GEI ➜ /workspaces/SystemProgrammingAssignments/Assignment3/303 (main) $./TailViewer testfile.txt 5
r0PN4wlr1uTkNA==
```
```bash
@BH3GEI ➜ /workspaces/SystemProgrammingAssignments/Assignment3/303 (main) $./HeadViewer testfile.txt 5
r0PN4wlr1uTkNA==
```

## Assignment4

### 404

#### Basic Usage:
We just run it as it does not take any arguments.
```bash
./MySystem
```

#### Output:

```bash
@BH3GEI ➜ /workspaces/SystemProgrammingAssignments/Assignment4/404 (main) $ gcc -o MySystem MySystem.c
@BH3GEI ➜ /workspaces/SystemProgrammingAssignments/Assignment4/404 (main) $ ./MySystem
      1       1      11
@BH3GEI ➜ /workspaces/SystemProgrammingAssignments/Assignment4/404 (main) $ ls *.c | wc
      1       1      11
```
We can see the results are the same.

### 405

#### Basic Usage:

```bash
./RedirectModified [input_file] [output_file]
```

input_file: File to process.
output_file: Result out file.

#### Output:

```bash
@BH3GEI ➜ /workspaces/SystemProgrammingAssignments/Assignment4/405 (main) $ head -c 10 < /dev/urandom | base64 > input.txt
@BH3GEI ➜ /workspaces/SystemProgrammingAssignments/Assignment4/405 (main) $ gcc -o RedirectModified RedirectModified.c
@BH3GEI ➜ /workspaces/SystemProgrammingAssignments/Assignment4/405 (main) $ ./RedirectModified input.txt output.txt
```

### 407
#### Improvements:

- Removed `sleep()` calls and explored better synchronization techniques such as Mutexes or Condition Variables.

#### Basic Usage:
```bash
./AlternatingOutput [num of chars]
```
num of chars: How many characters the user wants to output.

#### Output:

```bash
@BH3GEI ➜ /workspaces/SystemProgrammingAssignments/Assignment4/407 (main) $ gcc -o AlternatingOutput AlternatingOutput.c
@BH3GEI ➜ /workspaces/SystemProgrammingAssignments/Assignment4/407 (main) $ ./AlternatingOutput 60
0a1b2c3d4e5f6g7h8i9j0kl12m3n4o5p6q7r8s9t0u1v2w3x4y5z6a7b8c9d
```

## Assignment5

### 503
#### Basic Usage:
```bash
./SignalHandler [argument]
```

#### Output:
```bash
@BH3GEI ➜ /workspaces/SystemProgrammingAssignments/Assignment5/503 (main) $ ./SignalHandler 0
Received SIGFPE, division by zero
@BH3GEI ➜ /workspaces/SystemProgrammingAssignments/Assignment5/503 (main) $ ./SignalHandler 1
Received SIGSEGV, invalid memory access
@BH3GEI ➜ /workspaces/SystemProgrammingAssignments/Assignment5/503 (main) $ ./SignalHandler 2

```

Restricted by Github Codespaces, I can't resize this terminal. so I'll pass a message from another terminal next to trigger the condition which parameter as 2.

```bash
@BH3GEI ➜ /workspaces $ ps aux | grep SignalHandler
codespa+   48453  0.0  0.0   2368   896 pts/1    S+   20:20   0:00 ./SignalHandler 2
codespa+   48913  0.0  0.0   8176  2304 pts/2    S+   20:20   0:00 grep --color=auto SignalHandler
@BH3GEI ➜ /workspaces $ kill -SIGWINCH 48453
```
Then we can see that the condition is triggered.
```bash
@BH3GEI ➜ /workspaces/SystemProgrammingAssignments/Assignment5/503 (main) $ ./SignalHandler 2
Received SIGWINCH, window size changed
```
### 505
#### Improvements:

- Modified termio logic and added functionality to print the current time before calling `mygetchar()`.
- Ensured compatibility with EOF input.
#### Basic Usage:
```bash
./GetcharModified [timeout]
```
#### Output:
```bash
# Ctrl-C
@BH3GEI ➜ /workspaces/SystemProgrammingAssignments/Assignment5/505 (main) $ ./GetcharModified 3
Please input a character within 3 seconds:
^C
Return value: -3, current time: Fri Nov 24 03:13:12 2023

# Normal
@BH3GEI ➜ /workspaces/SystemProgrammingAssignments/Assignment5/505 (main) $ ./GetcharModified 3
Please input a character within 3 seconds:
a
Return value: 97, current time: Fri Nov 24 03:13:00 2023

# Timeout
@BH3GEI ➜ /workspaces/SystemProgrammingAssignments/Assignment5/505 (main) $ ./GetcharModified 3
Please input a character within 3 seconds:

Return value: -2, current time: Fri Nov 24 03:13:19 2023

# EOF
@BH3GEI ➜ /workspaces/SystemProgrammingAssignments/Assignment5/505 (main) $ echo -n | ./GetcharModified 3
Please input a character within 3 seconds:

Return value: -1, current time: Fri Nov 24 03:13:25 2023

# EOF
@BH3GEI ➜ /workspaces/SystemProgrammingAssignments/Assignment5/505 (main) $ ./GetcharModified 3
Please input a character within 3 seconds:
^D
Return value: -1, current time: Fri Nov 29 01:14:20 2023
```

## Assignment6

### 612

I used Jilin University's mail server to replace Tsukuba University's server because I don't have a Tsukuba University e-mail address.
Jilin University's mail server requires submission of email accounts and passwords encoded in base64.
So I implemented a simple base64 encoding and added statements to set the account password in the variable definition part of the code.
In addition, I added some logic related to exception handling.

Before programming, we can try logging in and sending a mail using telnet.

```bash

$ telnet mails.jlu.edu.cn smtp
Trying 202.198.16.89...
Connected to mails.jlu.edu.cn.
Escape character is '^]'.
220 mails.jlu.edu.cn ESMTP Server
EHLO mails.jlu.edu.cn
250-mails.jlu.edu.cn
250-PIPELINING
250-SIZE 1524000000
250-ETRN
250-STARTTLS
250-AUTH LOGIN PLAIN
250-AUTH=LOGIN PLAIN
250-ENHANCEDSTATUSCODES
250 8BITMIME
AUTH LOGIN
334 VXNlcm5hbWU6
dGhpcyBpcyB1c2VybmFtZQo=
334 UGFzc3dvcmQ6
dGhpcyBpcyBwYXNzd29yZA==
235 2.7.0 Authentication successful
MAIL FROM: liyao1119@mails.jlu.edu.cn
250 2.1.0 Ok
RCPT TO: liyao.bh3gei@gmail.com
250 2.1.5 Ok
DATA
354 End data with <CR><LF>.<CR><LF>
From: liyao1119@mails.jlu.edu.cn
To: liyao.bh3gei@gmail.com
Subject: Across the firewall we can reach every corner of the world.
.
250 2.0.0 Ok: queued as 4SHRdM0JbTz1gwbG
quit
221 2.0.0 Bye
Connection closed by foreign host.

```

#### Improvements:

- Added status code checks to `smtp_command()` for better error handling.
- Replaced `strcat()` with `snprintf()` to prevent buffer overflows.
- Fixed SMTP DATA formatting by replacing "." with ".." and ensuring an empty line between headers and body.


#### Usage:

The execution would probably be like this

```bash
./smtpput [host address] [from@coins.tsukuba.ac.jp] [to@coins.tsukuba.ac.jp] < mail-data
```

In addition, the user's account password needs to be configured

```c
// Global variables for the email and password
char *username = "xxx@xxx.xxx";
char *password = "xxxxxx";
```

#### Output:

```bash
@BH3GEI ➜ /workspaces/SystemProgrammingAssignments/Assignment6/612 $ ./smtpput mails.jlu.edu.cn liyao1119@mails.jlu.edu.cn liyao.bh3gei@gmail.com < mail-data
220 mails.jlu.edu.cn ESMTP Server
250-mails.jlu.edu.cn
250-PIPELINING
250-SIZE 1524000000
250-ETRN
250-STARTTLS
250-AUTH LOGIN PLAIN
250-AUTH=LOGIN PLAIN
250-ENHANCEDSTATUSCODES
250 8BITMIME
334 VXNlcm5hbWU6
334 UGFzc3dvcmQ6
235 2.7.0 Authentication successful
250 2.1.0 Ok
250 2.1.5 Ok
354 End data with <CR><LF>.<CR><LF>
250 2.0.0 Ok: queued as 4Sc77x2Yvwz1gwb7
221 2.0.0 Bye
```
After running the program, we open the mail box.

It was successfully sent.

![image](https://github.com/BH3GEI/SystemProgrammingAssignments/assets/58540850/fcb0c853-e631-4a0f-a848-a3ef7b81de23)

