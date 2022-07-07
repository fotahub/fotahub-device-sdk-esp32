@echo off
docker run --rm ^
  --env HOST=Windows ^
  --volume %~dp0..\..\..:/workspace ^
  --workdir /workspace/examples/esp-eye/human-face-detection ^
  fotahub/esp-who:latest ^
  ./idf.sh build
