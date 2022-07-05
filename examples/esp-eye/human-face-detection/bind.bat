@echo off
docker run --rm ^
  --env HOST=Windows ^
  --volume %~dp0..\..\..:/workspace ^
  --workdir /workspace/examples/esp-eye/human-face-detection ^
  fotahub/esp-who:0.9.3-1 ^
  ./idf.sh build
