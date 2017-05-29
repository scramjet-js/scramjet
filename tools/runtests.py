#!/usr/bin/env python
""" Application for running test drivers """
# Usage:
#     $ runtest test1 test2 test3

import sys
import subprocess

def run_test(test):
    """ Execute the specified 'test' """
    test_index = 0
    while True:
        test_index = test_index + 1
        result = subprocess.call([test, str(test_index)])
        if 0 > result:
            print "Test " + str(test_index) + " of '" + test + "' failed."
            sys.exit(1)
        elif 200 < result:
            break

# Run each test driver passed in on the command-line

for t in sys.argv[1:]:
    run_test(t)

