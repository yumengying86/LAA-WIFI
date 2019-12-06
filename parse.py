#!/usr/bin/python3
import sys
import re

with open('res.csv', 'w') as fout:
    with open(sys.argv[1], 'r') as fin:
        line = fin.readline()
        while line:
            if re.match('--------monitorA----------', line):
                line = fin.readline()
                if re.match('^Flow', line):
                    for i in range(8):
                        line = fin.readline()
                        fout.write(re.search("\d+(.\d+)?", line).group() + ',')
                    line = fin.readline()
                else:
                    fout.write(',,,,,,,,')
                    continue
            elif re.match('--------monitorB----------', line):
                line = fin.readline()
                if not line:
                    break
                elif re.match('^Flow', line):
                    for i in range(8):
                        line = fin.readline()
                        fout.write(re.search("\d+(.\d+)?", line).group() + ',')
                    fout.write('\n')
                    line = fin.readline()
                else:
                    fout.write(',,,,,,,,\n')
                    line = fin.readline()
            else:
                line = fin.readline()
                