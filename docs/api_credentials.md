# Telegram API credentials

LibreGram needs its own Telegram API `api_id` and `api_hash` to connect to Telegram.

Obtain an application ID and hash from the [Telegram API development tools](https://my.telegram.org/apps). Keep the hash private and never commit credentials to Git.

## Runtime environment variables

The preferred environment variable names are:

- `LIBREGRAM_API_ID`
- `LIBREGRAM_API_HASH`

Both must be set together. For example, in a shell:

```bash
export LIBREGRAM_API_ID="YOUR_API_ID"
export LIBREGRAM_API_HASH="YOUR_API_HASH"
./LibreGram
```

The application also accepts the legacy `KTGDESKTOP_API_ID` and `KTGDESKTOP_API_HASH` pair for compatibility with existing local setups. The complete LibreGram pair takes precedence when both pairs are present. Values from different prefixes are never mixed.

The existing `-api-id` and `-api-hash` command-line options remain supported. Environment variables take precedence unless `-no-env-api` is passed.

## Build-time CMake options

The inherited build system continues to use these CMake cache variables:

- `TDESKTOP_API_ID`
- `TDESKTOP_API_HASH`

Their names are retained for compatibility with the existing CMake and Telegram Desktop build infrastructure. Do not put real API credentials into tracked source files, public build logs or commits. The build defines them as compile-time values, so a distributed client binary contains the API hash and it may be recoverable by recipients. Use credentials dedicated to this application for a public binary release; do not reuse credentials from another project.

## Important

Use credentials belonging to your own Telegram application. Test credentials are intended only for limited development and must not be treated as production credentials. See the [official Telegram API documentation](https://core.telegram.org/api/obtaining_api_id).
