"""Convert a trusted PEM CA list to the Arduino-ESP32 2.x certificate bundle.

Requires cryptography. Input must come from a trusted source (e.g. curl.se/ca).
The format stores sorted DER subjects and SubjectPublicKeyInfo, matching
libraries/WiFiClientSecure/src/esp_crt_bundle.c in Arduino-ESP32 2.0.17.
"""
import argparse
import re
import struct
from pathlib import Path
from cryptography import x509
from cryptography.hazmat.primitives.serialization import Encoding, PublicFormat

parser = argparse.ArgumentParser()
parser.add_argument("input", type=Path)
parser.add_argument("output", type=Path)
args = parser.parse_args()
entries = {}
for pem in re.findall(rb"-----BEGIN CERTIFICATE-----.*?-----END CERTIFICATE-----", args.input.read_bytes(), re.S):
    cert = x509.load_pem_x509_certificate(pem)
    subject = cert.subject.public_bytes()
    key = cert.public_key().public_bytes(Encoding.DER, PublicFormat.SubjectPublicKeyInfo)
    if subject in entries and entries[subject] != key:
        raise ValueError("Duplicate subject with different public keys; inspect input")
    entries[subject] = key
assert entries, "No CA certificates in PEM"
bundle = struct.pack(">H", len(entries))
for subject, key in sorted(entries.items()):
    bundle += struct.pack(">HH", len(subject), len(key)) + subject + key
args.output.parent.mkdir(parents=True, exist_ok=True)
args.output.write_bytes(bundle)
print(f"CA bundle: {len(entries)} certificates, {len(bundle)} bytes")
