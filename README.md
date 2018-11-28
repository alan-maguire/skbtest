# skbtest - a KTF test suite for the sk_buff structure/functions

struct sk_buff * is the core networking abstraction used to represent packets
as they travel through the networking stack.  Here we demonstrate KTF functionality by writing tests which assert aspects of skb behaviour.  The initial intent of this work is demonstrate how to write tests with KTF; however a longer-term aspiration is to develop a valuable set of unit tests around skb behaviour.

Any contribution or feedback is most welcome!

Test cases are added to kernel/skbuff.c. To run the tests you will need to build googletest from source, build KTF and build this suite also.

Note that these instructions are centered on Oracle Linux but should work for other distros too, possibly with modified package names.

## Building googletest

```
# yum install cmake3
# cd ~
# mkdir -p src build/`uname -r`
# cd src
# git clone https://github.com/google/googletest.git
# cd ~/build/`uname -r`
# mkdir googletest
# cd googletest
# cmake3 ~/src/googletest/ -DBUILD_SHARED_LIBS=ON
# make
# sudo make install
```

## Building KTF

We need kernel-uek-devel and cpp packages to build. Finally once we have built ktf, we insert the kernel module.

```
# sudo yum install kernel-uek-devel cpp libnl3-devel
# cd ~/src
# git clone https://github.com/oracle/ktf
# cd ktf
# autoreconf
# cd ~/build/`uname -r`
# mkdir ktf
# cd ktf
# PKG_CONFIG_PATH=/usr/local/lib64/pkgconfig ~/src/ktf/configure KVER=`uname -r`
# make
# sudo make install
# sudo insmod kernel/ktf.ko

## Building skbtest

```

# cd ~/src
# git clone <path to this git tree>
# cd skbtest
# autoreconf
# cd ~/build/`uname -r`
# mkdir skbtest
# cd skbtest
# ~/src/skbtest/configure  KVER=`uname -r` --prefix=/usr/local --libdir=/usr/local/lib64 --with-ktf=/usr/local
# make
# sudo make install
# sudu insmod kernel/skbtest.ko
```

## Running skbtest

Now the ktf and skbtest modules have been added, we just need to "ktfrun".

# sudo LD_LIBRARY_PATH=/usr/local/lib64 /usr/local/bin/ktfrun
[==========] Running 2 tests from 1 test case.
[----------] Global test environment set-up.
[----------] 2 tests from skb
[ RUN      ] skb./alloc_skb_nomem
[       OK ] skb./alloc_skb_nomem (23 ms)
[ RUN      ] skb./alloc_skb_sizes
[       OK ] skb./alloc_skb_sizes (0 ms)
[----------] 2 tests from skb (23 ms total)

[----------] Global test environment tear-down
[==========] 2 tests from 1 test case ran. (23 ms total)
[  PASSED  ] 2 tests.
```
