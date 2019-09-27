

# WebSocket API

## Table of Contents

+ [Overview](#overview)
+ [Querying Capabilities](#querying-capabilities)
+ [Encoding Sessions](#encoding-sessions)
+ [Decoding Sessions](#decoding-sessions)
+ [Speech Detection Events](#speech-detection-events)


## Overview
The service exposes a WebSocket server endpoint at the root path. It supports two modes of operation `decoding` and `encoding` as well as a mechanism to query the audio capabilities supported. Messages and events are sent as JSON formatted text messages, while audio is transferred as binary data.

Generally, the client would start by establishing a WebSocket connection with the service in order to discover what capabilities the service supports. This can be done by sending a [queryCapabilities](#querying-capabilities) message and using the respective response for establishing `decoding` or `encoding` sessions.

Aftewards the client would initiate encoding or decoding session, one websoket session can only be used for either `decoding` OR `encoding` audio . When the client opens the WebSocket connection it must specify the audio it will send in the `inbound` section and the audio it expects to receive in the `outbound` section. In general, these `inbound` and `outbound` sections can be derived from the `capabilities` message response from the server.

To get started, see how to create [Encoding](#encoding-sessions) or [Decoding](#decoding-sessions) sessions.


## Querying Capabilities

A client can send the `queryCapabilities` message to retrieve the capabilities supported by the service. After the client establishes a connection to the server, it can send a message to ask the server to send the capabilities it supports, for example:

Client -> Server:

```json
{
  "method": "queryCapabilities"
}
```

The server will respond with a message defining the `inbound` and `outbound` encodings it supports as JSON formatted text message.

Server->Client

```json
{
  "type": "Codec",
  "inbound": {
    "encodingName": "G729",
    "channels": 1,
    "sampleRate": 8000,
    "payloadType": 18,
    "config": "annexb=yes"
  },
  "outbound": {
    "encodingName": "PCMU",
    "channels": "1",
    "sampleRate": 8000
  }
}
```

After the message has been sent the server will close the connection.

  Server->Client: `WebSocket Close Code: 1000`



## Encoding Sessions

Before creating an encoding session, the client must know what capabilites are supported by the service. This can be done by sending a [queryCapabilities](#querying-capabilities) message, querying capabilities does not need to be done everytime before establishing an **encoding** session.

Assuming these are the capabilities supported by the service:
  ```json
  {
    "type": "Codec",
    "inbound": {
      "encodingName": "G729",
      "channels": 1,
      "sampleRate": 8000,
      "payloadType": 18,
      "config": "annexb=yes"
    },
    "outbound": {
      "encodingName": "PCMU",
      "channels": "1",
      "sampleRate": 8000
    }
  }
  ```

1. Client opens a WebSocket connection the service:
    ```
    ws://{host_name}:9001
    ```
2. Client then sends a message which indicates the `inbound` audio it will send and the `outbound` audio it expects to receive, it is *RECOMMENDED* the client uses the respective sections from the queried capabilities message sent by the service:

    ```json
    {
        "inbound": {
          "encodingName": "PCMU",
          "channels": "1",
          "sampleRate": 8000
        },
        "outbound": {
          "encodingName": "G729",
          "channels": 1,
          "sampleRate": 8000,
          "config": "annexb=yes"
        }
    }
    ```
    This notifies the server that the audio sent will be `PCMU` at a sample rate of `8000`, and that the client expects to receive audio encoding as `G729` at a sample rate of `8000`.

3. The server will respond with a `listening` message:
    ```
    {
      "state": "listening"
    }
    ```
4. Afterwards, the client will send audio frames to encode. The audio frames must be sent as binary data and the server will return the respective frames in the specified encoding as binary data.


## Decoding Sessions

Before creating an encoding session, the client must know what capabilites are supported by the service. This can be done by sending a [queryCapabilities](#querying-capabilities) message, querying capabilities does not need to be done everytime before establishing an **decoding** session.

Assuming these are the capabilities supported by the service:
  ```json
  {
    "type": "Codec",
    "inbound": {
      "encodingName": "G729",
      "channels": 1,
      "sampleRate": 8000,
      "payloadType": 18,
      "config": "annexb=yes"
    },
    "outbound": {
      "encodingName": "PCMU",
      "channels": "1",
      "sampleRate": 8000
    }
  }
  ```

1. Client opens a WebSocket connection the service:
    ```
    ws://{host_name}:9001
    ```
2. Client then sends a message which indicates the `inbound` audio it will send and the `outbound` audio it expects to receive, it is *RECOMMENDED* the client uses the respective sections from the queried capabilities message sent by the service:

    ```json
    {
        "inbound": {
          "encodingName": "G729",
          "channels": 1,
          "sampleRate": 8000,
          "config": "annexb=yes"
        },
        "outbound": {
          "encodingName": "PCMU",
          "channels": "1",
          "sampleRate": 8000
        }
    }
    ```
    This notifies the server that the audio sent will be `G729` with Annex B frames, at a sample rate of `8000`, and that the client expects to receive audio decoded as `PCMU` at a sample rate of `8000`.

3. The server will respond with a `listening` message:
    ```
    {
      "state": "listening"
    }
    ```
4. Afterwards, the client will send audio frames to decode. The audio frames must be sent as binary data and the server will return the respective frames in the specified encoding as binary data.

5. Additionally, since the `config: annexb=yes` was specificed for `inbound` audio, the decoder will periodically send events to notify the client if it detects frames that signal the start of speech or end of speech. See [Speech Detection Events](#speech-detection-events)


## Speech Detection Events

During [Encoding](#encoding-sessions) or [Decoding](#decoding-sessions) sessions, if the `annexb=yes` configuration is specified the service may send events regarding if it detects frames that signal the start of speech or end of speech. These events are JSON formatted text messages sent from the server to the client:

#### START_OF_SPEECH Event

When the start of speech is detected the following event will be sent, this will only be sent once until an end of speech event is detected.

```json
{
  "event": "START_OF_SPEECH"
}
```

#### END_OF_SPEECH Event

When the end of speech is detected the following event will be sent, this will only be sent if prior speech has been detected.

```json
{
  "event": "END_OF_SPEECH"
}
```
