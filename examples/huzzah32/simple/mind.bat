@echo off
docker run --rm ^
  -v %~dp0..\..\..:/workspace ^
  -w /workspace/examples/huzzah32/simple ^
  fotahub/esp-idf:3.2.2-1 ^
  make -j %~1 HOST=Windows
