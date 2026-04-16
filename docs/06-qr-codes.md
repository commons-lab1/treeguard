# QR Code Generation Guide

Every TreeGuard unit should have a unique QR code that links to its tree info page.

## URL format

```
https://treeguard.org/tree/{TREE_ID}
```

Example: `https://treeguard.org/tree/TG-003`

Before the project has a domain, use a GitHub Pages URL:
```
https://commons-lab.github.io/treeguard/tree/{TREE_ID}
```

## Generating QR codes

### Option 1: Command line (recommended for batches)

Using `qrencode` (install via `apt install qrencode` / `brew install qrencode`):

```bash
# Single QR code
qrencode -o TG-001.png -s 10 -m 2 "https://commons-lab.github.io/treeguard/tree/TG-001"

# Batch generate (TG-001 through TG-050)
for i in $(seq -w 1 50); do
  qrencode -o "TG-${i}.png" -s 10 -m 2 \
    "https://commons-lab.github.io/treeguard/tree/TG-0${i}"
done
```

### Option 2: Python script

```python
# pip install qrcode[pil]
import qrcode

def generate_tree_qr(tree_id, output_dir="."):
    url = f"https://commons-lab.github.io/treeguard/tree/{tree_id}"
    img = qrcode.make(url, box_size=10, border=2)
    img.save(f"{output_dir}/{tree_id}.png")

# Generate a batch
for i in range(1, 51):
    generate_tree_qr(f"TG-{i:03d}")
```

### Option 3: Online tools

Any QR code generator works. Ensure:
- Error correction level: **M** (15%) minimum — outdoor dirt/scratches will obscure parts
- Size: at least **25 x 25 mm** printed — must be scannable from 30 cm distance
- Dark modules on light background (standard, not inverted)

## Printing QR codes

**Material options:**
- Weatherproof vinyl labels (best — UV and water resistant)
- Laminated paper (acceptable — will need replacement after 6-12 months)
- Laser-engraved onto frame (permanent but requires equipment)

**Placement:**
- Attach to the frame at eye level (~120-150 cm from ground)
- Face toward the walking path, not away from it
- Position near but not overlapping the signage sticker

## Tree landing page content

Each QR link should show:
1. **Tree species** (common + Latin name)
2. **Planted date**
3. **Current status** (if connected version: live from BLE/backend)
4. **"How to water" instructions** (1 L, plain water, pour at base)
5. **"Report a problem" button** (damage, dead tree, vandalism)
6. **Coordinator contact** (optional, per deployment group)

For MVP (offline devices), this can be a static page generated per tree.
For V1 (connected devices), the page can show live status from the backend.

## Tree ID format

Convention: `TG-{NNN}` where NNN is a zero-padded sequential number.

For larger deployments, prefix with a location code:
- `TG-NYC-001` (New York City deployment)
- `TG-BER-042` (Berlin deployment)

Track IDs in [data/field-logs/](../data/field-logs/) to avoid collisions.
