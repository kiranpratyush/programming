# Tiny HTTP server built directly on a TCP socket.
# Run:   powershell -ExecutionPolicy Bypass -File .\server.ps1
# Test:  curl.exe http://127.0.0.1:9999      (Wireshark: loopback adapter, filter tcp.port == 9999)
# Stop:  Ctrl+C

$port = 9999
$listener = [System.Net.Sockets.TcpListener]::new([System.Net.IPAddress]::Loopback, $port)
$listener.Start()   # from now on the OS answers SYN with SYN-ACK instead of RST
Write-Host "Listening on 127.0.0.1:$port ..."

try {
    while ($true) {
        # Blocks until a client finishes the 3-way handshake
        $client = $listener.AcceptTcpClient()
        $remote = $client.Client.RemoteEndPoint
        Write-Host "`n=== Connection from $remote ==="

        $stream = $client.GetStream()
        $buffer = New-Object byte[] 4096
        $n = $stream.Read($buffer, 0, $buffer.Length)
        $request = [System.Text.Encoding]::ASCII.GetString($buffer, 0, $n)
        Write-Host "Received $n bytes:"
        Write-Host $request

        $body = "Hello from my own server!`n"
        $response = "HTTP/1.1 200 OK`r`n" +
                    "Content-Type: text/plain`r`n" +
                    "Content-Length: $($body.Length)`r`n" +
                    "Connection: close`r`n" +
                    "`r`n" +
                    $body
        $bytes = [System.Text.Encoding]::ASCII.GetBytes($response)
        $stream.Write($bytes, 0, $bytes.Length)
        Write-Host "Sent $($bytes.Length) bytes."

        # The server closes first here -> watch who sends FIN first in Wireshark
        $client.Close()
        Write-Host "=== Connection closed ==="
    }
}
finally {
    $listener.Stop()
}
