
with open('/proc/cpuinfo') as f:
    for line in f:
        if (line.find("model name") != -1):
            print(line.rstrip('\n'))
            break 

with open('/proc/cpuinfo') as f:
    for line in f:
        if (line.find("cpu cores") != -1):
            print(line.rstrip('\n'))
            break 

with open('/proc/cpuinfo') as f:
    for line in f:
        if (line.find("cpu MHz") != -1):
            print(line.rstrip('\n'))
            break 

with open('/proc/meminfo') as f:
    for line in f:
        if (line.find("MemTotal:") != -1):
            print(line.rstrip('\n'))
            break 

with open('/proc/meminfo') as f:
    for line in f:
        if (line.find("MemFree:") != -1):
            print(line.rstrip('\n'))
            break 

with open('/proc/version') as f:
    for line in f:
            print(line.rstrip('\n'))
            break 