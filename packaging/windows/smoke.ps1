Add-Type -TypeDefinition @'
using System;
using System.Runtime.InteropServices;
public static class LibreGramWindowProbe {
    [StructLayout(LayoutKind.Sequential)]
    public struct Rect {
        public int Left, Top, Right, Bottom;
    }
    [DllImport("user32.dll")]
    public static extern bool GetWindowRect(IntPtr window, out Rect rectangle);
    [DllImport("user32.dll")]
    public static extern bool IsWindowVisible(IntPtr window);
}
'@

function Test-LibreGram($binary, $argsList) {
    $app = Start-Process -FilePath $binary -ArgumentList $argsList -PassThru
    try {
        $ready = $false
        for ($i = 0; $i -lt 120; $i++) {
            $app.Refresh()
            if ($app.HasExited) { throw "$binary exited before opening a window" }
            $rectangle = [LibreGramWindowProbe+Rect]::new()
            $window = $app.MainWindowHandle
            if ($window -ne 0 -and
                [LibreGramWindowProbe]::IsWindowVisible($window) -and
                [LibreGramWindowProbe]::GetWindowRect($window, [ref]$rectangle) -and
                ($rectangle.Right - $rectangle.Left) -ge 200 -and
                ($rectangle.Bottom - $rectangle.Top) -ge 200) {
                $ready = $true
                break
            }
            Start-Sleep -Milliseconds 250
        }
        if (!$ready) { throw "$binary did not open its main window" }
        Start-Sleep -Seconds 1
        $app.Refresh()
        if ($app.HasExited) { throw "$binary exited after opening its window" }
        $quit = Start-Process -FilePath $binary -ArgumentList "$argsList -quit" -PassThru
        if (!$quit.WaitForExit(30000) -or $quit.ExitCode -ne 0 -or
            !$app.WaitForExit(30000) -or $app.ExitCode -ne 0) {
            throw "$binary did not exit cleanly"
        }
        $quit.Dispose()
    } finally {
        if (!$app.HasExited) {
            $app.Kill()
            $app.WaitForExit()
        }
        $app.Dispose()
    }
}
