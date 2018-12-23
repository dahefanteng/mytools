#encoding=utf8
import sys
import time
import crypt
import itertools
import string

#返回用户所在位置
def location(fn,user):
    for sline in fn.readlines():
        fn.seek(0)
        if sline.strip().split(":")[0] == user:
            wz = fn.readlines().index(sline)
            fn.seek(0)
            return wz
def crack(fn,passwd,wz,text):
    text = text.split(":")[1]
    passlist = text.split("$")    
    if passlist[1] != "6":
        print passlist[1]
        print "该系统未采用sha512加密算法"
    else:
        salt = "$"+passlist[1]+"$"+passlist[2]
        if crypt.crypt(passwd,salt)==text:
            return 1
        else:
            return 0
def test():
    char = string.printable[:10]
    all = []
    for i in xrange(0,7):
        all.append(itertools.product(char,repeat=i))
    return all



def total():
    user = sys.argv[1]
    btime = time.time()
    fn = open("/etc/shadow","r")
    wz = location(fn,user)
    text = fn.readlines()[wz].strip()
    tmp = test()
    for i in tmp:
        for j in i:
            j = "".join(j)
            print j
            if crack(fn,j,wz,text):
                ftime=time.time()
                print ftime-btime
                print "用户%s的密码为%s" % (user,j)
                raise("ok")

if __name__ == "__main__":
    total()





