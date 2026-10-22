#!/bin/bash
# Bonus. Failed requests whose minute ends in zero, then their count.
grep -E '^[^ ]+ 10:[0-5]0 [^ ]+ FAIL ' access.log | awk '{print} END {print "matching failures:", NR}'
