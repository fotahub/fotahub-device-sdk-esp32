@echo off
docker run --rm ^
  -v %~dp0..\..\..:/workspace ^
  -w /workspace/examples/huzzah32/thingshadow ^
  fotahub/esp-idf:3.2.2-1 ^
  make -j %* HOST=Windows
