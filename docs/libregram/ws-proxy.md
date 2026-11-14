# LibreGram WS Proxy

Use the **WS Proxy** switch in the main menu for quick access. **LibreGram Settings → WS Proxy** and the proxy list on the sign-in screen open the detailed controls.

The Linux helper uses the same MIT-licensed tg-ws-proxy-rs revision as the LibreGram Android WS settings branch. The helper listens on 127.0.0.1 with a dynamically allocated port and an ephemeral MTProto secret. LibreGram switches Telegram sessions to that listener only after it reports readiness. The saved proxy configuration remains intact. Stopping the helper, changing the selected proxy, or an unexpected helper exit removes the temporary route.

Enable **Start WS Proxy when LibreGram opens** to start it automatically on subsequent launches. Turning the proxy off affects the current run. Use **Restart** to reconnect the local helper. Direct WSS is the default, and the dialog can save an explicit preference to allow raw TCP fallback. The status in the dialog describes the local helper, while the main window shows Telegram connectivity.

Build the helper with Cargo using the pinned source and lockfile:

```sh
Telegram/build/ws-proxy/build.sh /absolute/path/to/desktop-binary-directory
```

Place `libregram-ws-proxy` next to the Linux `LibreGram` executable. Distributions should include `tg-ws-proxy-rs.LICENSE`. Use the Linux build instructions for the Qt client. The helper does not need Android JNI or an NDK.
