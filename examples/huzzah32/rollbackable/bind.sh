#!/bin/bash
docker run --rm \
  --env HOST=Linux \
  --volume $PWD/../../..:/workspace \
  --workdir /workspace/examples/huzzah32/rollbackable \
  fotahub/esp-idf:4.4-1 \
  ./idf.sh build