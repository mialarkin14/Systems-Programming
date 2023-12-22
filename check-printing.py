#!/usr/bin/python3
from collections import deque
import sys, re

def     sys_error(s):
    print ("Error: " + s)
    sys.exit(1)

# regular expressions 
ptn_event = re.compile('^(Printer)\s+(\d+).+job\s+(\d+)');
ptn_summary = re.compile('(Printer)\s+(\d+).+\s(\d+)\s+jobs');
ptn_total = re.compile('^Total.+(d+)');
ptn_config = re.compile('(jobs|printers)=(\d+)');

n_lines = 0
num_jobs = num_printers = 0
printer_total = 0
printers=[]

for line in sys.stdin:
    n_lines += 1

    # config lines
    if n_lines < 3: 
        # check parameters
        m = re.search(ptn_config, line)
        if m:
            key = m.group(1)
            v = int(m.group(2))
            if key == 'printers':
                num_printers = v
            elif key == 'jobs': 
                num_jobs = v
                assert num_printers > 0
                assert num_jobs > 0
                printers = [0] * num_printers
                q = deque(range(num_jobs))
        else:
            sys_error("Not a configuration line.\n"+line)
        continue

    # event line
    m = re.search(ptn_event, line)
    if m:
        # event
        dev_id = int(m.group(2))
        job_id = int(m.group(3))
        job_id_expected = q.popleft()
        if job_id != job_id_expected:
            sys_error("Printer did not fetch the correct job.\nLine {}: {}".
                    format(n_lines, line))
        printers[dev_id] += 1
        continue

    # summary 
    m = re.search(ptn_summary, line)
    if m:
        dev_id = int(m.group(2))
        dev_total = int(m.group(3))
        assert dev_total == printers[dev_id]
        printer_total += dev_total

# sanity check
# print(len(q), num_jobs, printer_total)
assert len(q) == 0
assert printer_total == num_jobs

# printers.append(0)
# num_printers += 1
m = num_jobs / num_printers
lb = m * 0.7
ub = m * 1.3
for x in printers:
    if x < lb or x > ub:
        print("Number of jobs {} is far from average.".format(x))
        exit(1)
exit(0)
