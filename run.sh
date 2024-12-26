#!/bin/bash

if [ -z "$1" ]; then
  echo "Error: Missing input argument. Please provide testcase id."
  exit 1
fi

python ./scripts/auto_gen_transformer.py --input ./tests/$1_spec.json --output transformer.cpp

make clean && make -j 8

./main $2 ./tests/$1.in ./tests/$1.out

./verify.sh $1
