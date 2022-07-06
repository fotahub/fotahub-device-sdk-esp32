#!/bin/bash
docker run --rm \
  --env HOST=Linux \
  --volume $PWD/../../..:/workspace \
  --workdir /workspace/examples/huzzah32/simple \
  fotahub/esp-who:latest \
  ./idf.sh build