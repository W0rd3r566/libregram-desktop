#define AppName "LibreGram Desktop"
#define AppVersion "0.1.0-beta.1"
#ifndef SourcePath
  #error SourcePath must point to the Windows build artifacts
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
ArchitecturesAllowed=x64compatible
ArchitecturesInstallIn64BitMode=x64compatible
OutputBaseFilename=LibreGram-Windows-x86_64
Compression=lzma2
SolidCompression=yes
UninstallDisplayIcon={app}\LibreGram.exe

[Files]
Source: "{#SourcePath}\LibreGram.exe"; DestDir: "{app}"; Flags: ignoreversion
Source: "{#SourcePath}\libregram-ws-proxy.exe"; DestDir: "{app}"; Flags: ignoreversion
Source: "{#SourcePath}\LICENSE"; DestDir: "{app}"
Source: "{#SourcePath}\LEGAL"; DestDir: "{app}"
Source: "{#SourcePath}\tg-ws-proxy-rs.LICENSE"; DestDir: "{app}"

[Icons]
Name: "{group}\{#AppName}"; Filename: "{app}\LibreGram.exe"
