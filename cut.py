#!/usr/bin/python3
# import re
# 读取文件里面每一行

i = 0
with open('res', 'w') as fout:
    with open('aaa', 'r') as fin:
        line = fin.readline()
        i = i + 1
        while line:
            if '214285000' in line:
                line1 = fin.readline()
                line2 = fin.readline()
                line3 = fin.readline()
                line4 = fin.readline()
                line5 = fin.readline()
                i = i + 5
                if not(('214285000' in line1) and ('214285000' in line2)):
                    print("error")
                    print(i)
                if not(('785713000' in line3) and ('785713000' in line4)):
                    print("error")  
                    print(i)
                if not('785713000' in line5):
                    print("error")
                    print(i)
            else:
                fout.write(line)
            line = fin.readline()
            i = i + 1
