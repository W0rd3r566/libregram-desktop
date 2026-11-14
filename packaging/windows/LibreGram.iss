#define AppName "LibreGram Desktop"
#define AppVersion "0.1.0-beta.1"
#ifndef LibreGramPayload
  #error LibreGramPayload must point to the Windows build artifacts
#endif
#if LibreGramArch == "x86"
  #define AllowedArch "x86compatible"
#elif LibreGramArch == "x86_64"
  #define AllowedArch "x64compatible"
#elif LibreGramArch == "arm64"
  #define AllowedArch "arm64"
#else
  #error Unsupported LibreGramArch
#endif

[Setup]
AppId=io.github.w0rd3r566.libregram
AppName={#AppName}
AppVersion={#AppVersion}
AppPublisher=LibreGram contributors
AppPublisherURL=https://github.com/W0rd3r566/libregram-desktop
DefaultDirName={localappdata}\Programs\LibreGram
DefaultGroupName={#AppName}
PrivilegesRequired=lowest
ArchitecturesAllowed={#AllowedArch}
#if LibreGramArch != "x86"
ArchitecturesInstallIn64BitMode={#AllowedArch}
#endif
OutputBaseFilename=LibreGram-Windows-{#LibreGramArch}
Compression=lzma2
SolidCompression=yes
UninstallDisplayIcon={app}\LibreGram.exe

[Files]
Source: "{#LibreGramPayload}\LibreGram.exe"; DestDir: "{app}"; Flags: ignoreversion
Source: "{#LibreGramPayload}\libregram-ws-proxy.exe"; DestDir: "{app}"; Flags: ignoreversion
Source: "{#LibreGramPayload}\LICENSE"; DestDir: "{app}"
Source: "{#LibreGramPayload}\LEGAL"; DestDir: "{app}"
Source: "{#LibreGramPayload}\tg-ws-proxy-rs.LICENSE"; DestDir: "{app}"

[Icons]
Name: "{group}\{#AppName}"; Filename: "{app}\LibreGram.exe"
