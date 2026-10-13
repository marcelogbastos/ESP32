"""Send an alert while the TTGO's setup portal is open. Python standard library only."""
import argparse
import json
import os
import urllib.error
import urllib.request

parser = argparse.ArgumentParser()
parser.add_argument("text", help="1 to 95 UTF-8 bytes; ASCII recommended on the TFT")
parser.add_argument("--level", type=int, choices=(0, 1, 2), default=0)
args = parser.parse_args()
if not 1 <= len(args.text.encode("utf-8")) <= 95:
    parser.error("text must contain 1 to 95 UTF-8 bytes")
url = os.environ.get("TTGO_HUB_URL", "").rstrip("/")
key = os.environ.get("TTGO_HUB_KEY", "")
if not url.startswith(("http://", "https://")) or not key:
    parser.error("Set TTGO_HUB_URL and TTGO_HUB_KEY environment variables")
request = urllib.request.Request(
    url + "/api/alert",
    data=json.dumps({"text": args.text, "level": args.level}, ensure_ascii=False).encode(),
    headers={"Content-Type": "application/json", "X-Hub-Key": key},
    method="POST",
)
try:
    with urllib.request.urlopen(request, timeout=10) as response:
        print(response.status, response.read(512).decode())
except (urllib.error.URLError, TimeoutError) as error:
    parser.exit(1, f"Alert request failed: {error}\n")
