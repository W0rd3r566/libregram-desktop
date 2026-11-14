{ lib, stdenv, stdenvNoCC, autoPatchelfHook, makeWrapper, glib, cairo, pango
, fontconfig, freetype, libglvnd, libpulseaudio, alsa-lib, pipewire, dbus
, libx11, libxcb, libxext, libxrender, libxfixes, libxrandr, libxi
, libxcomposite, libxdamage, libxtst, libxcb-keysyms, libdrm, mesa, udev
, vulkan-loader, wayland
, archive, version
}:

stdenvNoCC.mkDerivation {
  pname = "libregram-desktop";
  inherit version;
  src = archive;
  nativeBuildInputs = [ autoPatchelfHook makeWrapper ];
  buildInputs = [
    stdenv.cc.cc.lib glib cairo pango fontconfig freetype libglvnd
    libpulseaudio alsa-lib pipewire dbus libx11 libxcb libxext libxrender libxfixes
    libxrandr libxi libxcomposite libxdamage libxtst vulkan-loader wayland
    libxcb-keysyms libdrm udev
  ];
  dontBuild = true;
  dontConfigure = true;
  dontStrip = true;
  installPhase = ''
    runHook preInstall
    id=io.github.w0rd3r566.libregram
    install -Dm755 LibreGram "$out/libexec/libregram/LibreGram"
    install -Dm755 libregram-ws-proxy "$out/libexec/libregram/libregram-ws-proxy"
    install -Dm644 "$id.png" "$out/share/icons/hicolor/256x256/apps/$id.png"
    install -Dm644 "$id.desktop" "$out/share/applications/$id.desktop"
    sed -i 's/^DBusActivatable=true$/DBusActivatable=false/' "$out/share/applications/$id.desktop"
    install -Dm644 LICENSE "$out/share/licenses/libregram-desktop/LICENSE"
    install -m644 LEGAL tg-ws-proxy-rs.LICENSE "$out/share/licenses/libregram-desktop/"
    makeWrapper "$out/libexec/libregram/LibreGram" "$out/bin/LibreGram" \
      --prefix LD_LIBRARY_PATH : "/run/opengl-driver/lib:${lib.makeLibraryPath [ libglvnd libpulseaudio alsa-lib pipewire dbus vulkan-loader wayland libx11 libxcb libxcb-keysyms libdrm mesa udev ]}"
    runHook postInstall
  '';
  meta = {
    description = "LibreGram Desktop messenger";
    homepage = "https://github.com/W0rd3r566/libregram-desktop";
    license = lib.licenses.gpl3Plus;
    platforms = [ "x86_64-linux" ];
    mainProgram = "LibreGram";
  };
}
