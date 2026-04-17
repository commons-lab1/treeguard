# Security Policy

## Scope

TreeGuard is an embedded device deployed in public spaces. Security concerns include:

- **Firmware vulnerabilities** that could allow malicious reflashing via BLE/OTA (V1+)
- **Data privacy** in field logs or app telemetry
- **Physical safety** (battery, electrical, structural)

## Reporting a vulnerability

If you discover a security issue, please report it responsibly:

1. **Do NOT open a public GitHub issue**
2. Email the maintainers directly (address in GitHub org profile)
3. Include: description, affected component, potential impact, steps to reproduce
4. We will acknowledge within 48 hours and provide a fix timeline

## Supported versions

| Version | Supported |
|---------|-----------|
| MVP (treeguard-mini) | Yes |
| V1 (treeguard-net) | Yes (when released) |

## Security considerations by design

- **Offline-first:** MVP has no radio, no attack surface beyond physical access
- **BLE (V1):** configuration writes require physical proximity (~10m)
- **No personal data:** the device stores no user information
- **OTA (stretch goal):** will require signed firmware images before deployment
