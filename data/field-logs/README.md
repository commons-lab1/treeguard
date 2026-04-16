# Field Logs

Store anonymized deployment data here as CSV files.

## Format

```csv
tree_id,species,preset_id,lat,lon,planted_date,deployed_date,removed_date,status,months_active,notes
TG-001,Quercus robur,2,0.000,0.000,2026-05-01,2026-05-01,,alive,0,"First test unit"
```

## Privacy

- Round GPS coordinates to 3 decimal places (~111m precision) — enough for analysis, not enough to identify specific addresses
- No personal data of deployers or waterers
- No photos with identifiable people

## Contributing data

Submit field logs via PR. Every data point helps improve thresholds, validate designs, and demonstrate impact to partner organizations.
