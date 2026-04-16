# 3D Printed Frame

## Files

- `treeguard-frame.scad` — Parametric OpenSCAD source (single file, all parts)
- Export STLs per part using the `part` parameter

## Print settings

| Setting | Value |
|---------|-------|
| Material | **PETG** (required — PLA fails outdoors) |
| Layer height | 0.2-0.3 mm |
| Infill | 30-40% gyroid |
| Walls | 3 perimeters minimum |
| Supports | Not needed for most parts |
| Color | Green or brown (blends with environment) |

## Exporting STLs

```bash
# Individual parts
openscad -o spine.stl -D 'part="spine"' treeguard-frame.scad
openscad -o ring.stl -D 'part="ring"' treeguard-frame.scad
openscad -o enclosure.stl -D 'part="enclosure"' treeguard-frame.scad
openscad -o stake.stl -D 'part="stake"' treeguard-frame.scad
openscad -o solar_mount.stl -D 'part="solar_mount"' treeguard-frame.scad
```

## Customization

Edit parameters at the top of the `.scad` file:

| Parameter | Default | Range | What it controls |
|-----------|---------|-------|------------------|
| `inner_diameter` | 200 mm | 150-300 | Clearance around trunk |
| `frame_height` | 700 mm | 400-1000 | Protective cage height |
| `ring_count` | 3 | 2-5 | Number of horizontal ring levels |
| `wall` | 4 mm | 3-6 | Structural thickness |

## Print list per unit

| Part | Quantity | Est. time | Est. filament |
|------|----------|-----------|---------------|
| Spine | 3 | ~45 min each | ~30g each |
| Ring segment | 9 (3 per level × 3 levels) | ~20 min each | ~10g each |
| Electronics enclosure | 1 | ~40 min | ~25g |
| Ground stake | 3 | ~15 min each | ~8g each |
| Solar mount (V1 only) | 1 | ~15 min | ~10g |
| **Total** | | **~6-7 hours** | **~230g** |
