#!/bin/bash

if [ -z "$1" ]; then
  echo "Error: Missing input argument. Please provide testcase id."
  exit 1
fi


python ./scripts/verify.py --output ./tests/$1.out --answer ./tests/$1.ans
