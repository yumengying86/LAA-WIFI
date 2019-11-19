#!/usr/bin/python3
# import re
# 读取文件里面每一行
with open('res.csv', 'w') as fout:
    with open('tmp.txt', 'r') as fin:
        while fin.readline():
            for i in range(10):
                line = fin.readline()
            for i in range(8):
                line = fin.readline()
                fout.write((((line.split(':'))[1].strip()).split(' '))[0] + ',') 
            for i in range(2):
                line = fin.readline()
            for i in range(8):
                line = fin.readline()
                fout.write((((line.split(':'))[1].strip()).split(' '))[0] + ',') 
            fout.write('\n')
