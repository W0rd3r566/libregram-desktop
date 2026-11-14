// LibreGram process adapter for tg-ws-proxy-rs (MIT).
use std::io::{self, Read, Write};
use tg_ws_proxy_rs::{config::Config, server};

#[tokio::main]
async fn main() {
    let config = Config::from_args();
    let (closed, shutdown) = tokio::sync::oneshot::channel();
    std::thread::spawn(move || {
        let mut input = io::stdin().lock();
        let _ = input.read(&mut [0]);
        let _ = closed.send(());
    });
    let result = server::run_with_listen(
        config,
        async { let _ = shutdown.await; },
        |listener| {
            let mut output = io::stdout().lock();
            let _ = writeln!(output, "{}", listener.tg_link);
            let _ = output.flush();
        },
    ).await;
    if result.is_err() {
        std::process::exit(1);
    }
}
