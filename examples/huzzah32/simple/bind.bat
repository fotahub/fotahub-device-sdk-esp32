@echo off
docker run --rm ^
  --env HOST=Windows ^
  --volume %~dp0..\..\..:/workspace ^
  --workdir /workspace/examples/huzzah32/simple ^
  fotahub/esp-idf:4.4.1-1 ^
  ./idf.sh build
