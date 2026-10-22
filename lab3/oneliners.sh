#!/bin/bash
# Run with bash oneliners.sh in the directory containing access.log.
# Q1. How many requests failed?
awk '$4 == "FAIL" {n++} END {print n+0}' access.log
# Q2. How many different pages were requested, and which?
cut -d' ' -f5 access.log | sort -u | awk '{print} END {print "pages:", NR}'
# Q3. How many requests did each page get?
cut -d' ' -f5 access.log | sort | uniq -c
# Q4. Which user(s) have the most failures, and how many? Keep every tie.
awk '$4 == "FAIL" {print $3}' access.log | sort | uniq -c | sort -k1,1nr -k2,2 | awk 'NR == 1 {max=$1} $1 == max'
# Q5. How many requests did user3 make, and how many failed?
awk '$3 == "user3" {n++; if ($4 == "FAIL") f++} END {print "user3:", n+0, "requests,", f+0, "failed"}' access.log
# Q6. Last three failures, showing only time and user.
awk '$4 == "FAIL" {print $2, $3}' access.log | tail -3
# Q7. Login shells and account counts (configured, not necessarily interactive).
cut -d: -f7 /etc/passwd | sort | uniq -c
# Q8. Predict: the long listing has one additional total line.
ls /etc | wc -l; ls -l /etc | wc -l
