
# G729 Codec Service

This is a microservice that exposes a WebSocket interface to transcode audio between G.729A with Annex B and PCMU (G.711 µ-law).

## Table of Contents
+ [Building](#building)
+ [Building a Docker Container](#building-a-docker-container)
+ [WebSocket API docs](docs/websocket-api.md)
+ [Configuration](#configuration)
+ [License](#Licensing)

## Building

### Requirements

+ [Meson](https://mesonbuild.com/Quick-guide.html)
+ OpenSSL v1.1+

### Build Procedure

1. Clone the repository and change working directory
   ```
   git clone https://github.com/IBM/g729-codec-service
   cd g729-codec-service
   ```
2. Run `meson` to generate the build files
   ```
   meson build
   ```
3. Build the source and create an executable using `ninja`, an executable `g729-codec-service` will be generated.
   ```
   cd build/
   ninja
   ```
4. Run the executable:
   ```
   ./g729-codec-service
   [2019-09-27 10:24:26.892] [g729-codec-service] [info] CWSCS0001I: g729-codec-service is listening for requests on address 0.0.0.0 and port 9001.
   ```


## Building a Docker Container

To build as a docker container, you can follow the steps:

1. Clone the repository and change working directory
    ```
    git clone https://github.com/IBM/g729-codec-service
    cd g729-codec-service
    ```
2. Run the `docker build` command:
    ```
    docker build -t g729-codec-service .
    ```
3. Startup the container:
    ```
    docker run g729-codec-service
    [2019-09-27 10:24:26.892] [g729-codec-service] [info] CWSCS0001I: g729-codec-service is listening for requests on address 0.0.0.0 and port 9001.
    ```


## Configuration

Configuration is done through specification of environment variables.

| Configuration Name | Default | Description |
| --- | --- | --- |
| `WEBSOCKET_SEVER_PORT` | `9001` | Specifies the port to listen to for WebSocket requests. |
| `WEBSOCKET_SERVER_HOST` | `0.0.0.0` | Specifies the host interface to listen on for WebSocket requests. |
| `WEBSOCKET_SERVER_CERT_FILE_NAME` | `N/A` | Path to certificate file that will be used for secure websocket requests. |
| `WEBSOCKET_SERVER_KEY_FILE_NAME` | `N/A` | Path to key that will be used for secure websoket server requests. |
| `WEBSOCKET_SERVER_IDLE_TIMEOUT` | `600` | Specifies the amount of time in seconds to wait between WebSocket connection messages before closing down the connection.|
| `LOG_LEVEL` | `INFO` | Specifies the log level. Valid values are `TRACE`, `DEBUG`, `INFO`, `WARN` and `OFF`. |
| `ANNEXB_ENABLED` | `YES` | Specifies if annexb should be used for the codec. |


## Licensing

This projects is licensed under [GNU GPLv2](LICENSE)
