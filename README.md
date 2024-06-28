# Classbench for Packet Classification

https://github.com/classbench-ng/classbench-ng

https://www.arl.wustl.edu/classbench/

## Environment

Develop on Visual Studio Code and cmake with WSL2 Ubuntu-22.04

Git commit message template with .stCommitMsg

## How to Use

#### Notice your file path

Old version:

make all

sh run.sh acl1 1000

---

New version of classbench, installed environment (Ruby):

make

sh run.sh acl1 1000

(The new version of classbench is too slow, and it takes too long to generate a 100K data set, so I used 20K to check it can run.)


## License

                    GNU GENERAL PUBLIC LICENSE
                       Version 3, 29 June 2007
