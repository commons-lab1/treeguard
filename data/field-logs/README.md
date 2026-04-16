# Field Logs

Store anonymized deployment data here as CSV files.

## Format

See [template.csv](template.csv) for the column definitions.

```csv
tree_id,species_common,species_latin,preset_id,lat,lon,planted_date,deployed_date,removed_date,status,months_active,frame_type,firmware_version,notes
TG-001,English Oak,Quercus robur,2,0.000,0.000,2026-05-01,2026-05-01,,alive,0,3dprint,mini-0.1,"First test unit"
```

Copy `template.csv` as your starting point and add rows for each deployed unit.

## Privacy

- Round GPS coordinates to 3 decimal places (~111m precision) — enough for analysis, not enough to identify specific addresses
- No personal data of deployers or waterers
- No photos with identifiable people

## Contributing data

Submit field logs via PR. Every data point helps improve thresholds, validate designs, and demonstrate impact to partner organizations.
