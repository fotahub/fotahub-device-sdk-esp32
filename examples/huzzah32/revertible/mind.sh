#!/bin/bash
docker run --rm \
  -v $PWD/../../..:/workspace \
  -w /workspace/examples/huzzah32/revertible \
  fotahub/esp-idf:3.2.2-1 \
  make -j $@ HOST=Linux
