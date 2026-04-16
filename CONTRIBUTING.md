# Contributing to TreeGuard

Thank you for wanting to help trees survive.

## How to contribute

### Report a bug or suggest a feature
Open a GitHub Issue. Use the templates provided.

### Submit a change
1. Fork the repository
2. Create a feature branch: `git checkout -b feature/your-change`
3. Make your changes
4. Sign off your commits (DCO): `git commit -s -m "description"`
5. Push and open a Pull Request

### DCO sign-off
We use the [Developer Certificate of Origin](https://developercertificate.org/). Every commit must include:
```
Signed-off-by: Your Name <your@email.com>
```
Use `git commit -s` to do this automatically.

## What we especially need help with

- **Field testing:** Deploy units, report survival data
- **Translations:** Signage and docs in more languages
- **Alternative frame designs:** Not everyone has a 3D printer
- **Species presets:** Share moisture thresholds for tree species in your region
- **PCB review:** Sanity-check our schematics
- **Documentation:** Photos, diagrams, clearer instructions

## Design principles

Before proposing changes, please read [docs/design-rationale/](docs/design-rationale/). Every decision has a reason. If you disagree with a reason, open a discussion — we change our minds when shown better data.

## Code style

- Firmware: C++, PlatformIO, clang-format with the provided `.clang-format`
- App: standard Prettier config
- Docs: one sentence per line (easier diffs)

## License

By contributing, you agree that your contributions are licensed under the project's respective licenses (MIT for software, CERN-OHL-S v2 for hardware, CC BY-SA 4.0 for docs).
