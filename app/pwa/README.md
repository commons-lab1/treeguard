# TreeGuard PWA

Offline-first progressive web app for finding and watering thirsty trees.

## Status

**Stretch goal.** This PWA is a UI skeleton demonstrating the target UX. The device works without any app — the app adds convenience and gamification.

## Features (planned)

- Scan for nearby TreeGuard devices via Web Bluetooth
- View tree status (thirsty / happy / sick)
- Log watering events
- Track personal stats (trees watered, streak, liters)
- QR code landing pages per tree

## Running locally

```bash
# Any static file server works
npx serve .
# or
python3 -m http.server 8000
```

Open `http://localhost:8000` on your phone. Web Bluetooth requires HTTPS in production — use a tunneling tool (ngrok, Cloudflare Tunnel) for BLE testing on a real device.

## Tech stack

- Vanilla HTML/CSS/JS (no build step, no framework)
- Service Worker for offline caching
- Web Bluetooth API for BLE scanning
- localStorage for stats persistence

Intentionally zero dependencies. Any volunteer should be able to open `index.html` and understand the entire app.

## Icons

`icon-192.png` and `icon-512.png` need to be created (the TreeGuard logo). For now the PWA works without them — the manifest just won't show a home screen icon.

## Deployment

Host on GitHub Pages, Netlify, or Cloudflare Pages. Point a subdomain like `app.treeguard.org` at it.
