// You can comment this out to disable debugging
//#define DEBUGGING
#include "WebSocketClient.h"
#include "sha1.h"
#include "base64.h"

bool WebSocketClient::handshake(Client &client) {

    socket_client = &client;

    // If there is a connected client->
    if (socket_client->connected()) {
        // Check request and look for websocket handshake
#ifdef DEBUGGING
            Serial.println(F("Client connected"));
#endif
        if (analyzeRequest()) {
#ifdef DEBUGGING
                Serial.println(F("Websocket established"));
#endif

                return true;

        } else {
            // Might just need to break until out of socket_client loop.
#ifdef DEBUGGING
            Serial.println(F("Invalid handshake"));
#endif
            disconnectStream();

            return false;
        }
    } else {
        return false;
    }
}

bool WebSocketClient::analyzeRequest() {
    String temp;

    int bite;
    bool foundupgrade = false;
    unsigned long intkey[2];
    String serverKey;
    char keyStart[17];
    char b64Key[25];
    //String key = "------------------------";	              
	String key = "dGhlIHNhbXBsZSBub25jZQ==";

    randomSeed(analogRead(0));

    for (int i=0; i<16; ++i) {
        keyStart[i] = (char)random(1, 256);
    }

    base64_encode(b64Key, keyStart, 16);

    for (int i=0; i<24; ++i) {
        key[i] = b64Key[i];
    }

#ifdef DEBUGGING
    Serial.println(F("Sending websocket upgrade headers"));
#endif    

    socket_client->print(F("GET "));
    socket_client->print(path);
    socket_client->print(F(" HTTP/1.1\r\n"));
    socket_client->print(F("Upgrade: websocket\r\n"));
    socket_client->print(F("Connection: Upgrade\r\n"));
    socket_client->print(F("Host: "));
    socket_client->print(host);
    socket_client->print(CRLF); 
    socket_client->print(F("Sec-WebSocket-Key: "));
    socket_client->print(key);
    socket_client->print(CRLF);
	//socket_client->print(F("Sec-WebSocket-Origin: "));
	//socket_client->print(origin);
	//socket_client->print(CRLF);
    socket_client->print(F("Sec-WebSocket-Version: 13\r\n"));
    socket_client->print(CRLF);

#ifdef DEBUGGING
    Serial.println(F("Analyzing response headers"));
#endif    

    while (socket_client->connected() && !socket_client->available()) {
        delay(100);
#ifdef DEBUGGING
        Serial.println("Waiting for WebSocket connection...");
#endif
    }

    // TODO: More robust string extraction
    while ((bite = socket_client->read()) != -1) {

        temp += (char)bite;

        if ((char)bite == '\n') {
#ifdef DEBUGGING
            Serial.print("Got Header: " + temp);
#endif
            if (!foundupgrade && temp.startsWith("Upgrade: websocket")) {
                foundupgrade = true;
            } else if (temp.startsWith("Sec-WebSocket-Accept: ")) {
                serverKey = temp.substring(22,temp.length() - 2); // Don't save last CR+LF
            }
            temp = "";		
        }

        if (!socket_client->available()) {
          delay(20);
        }
    }

    key += "258EAFA5-E914-47DA-95CA-C5AB0DC85B11";
    uint8_t *hash;
    char result[21];
    char b64Result[30];

    Sha1.init();
    Sha1.print(key);
    hash = Sha1.result();

    for (int i=0; i<20; ++i) {
        result[i] = (char)hash[i];
    }
    result[20] = '\0';

    base64_encode(b64Result, result, 20);

    // if the keys match, good to go
    return serverKey.equals(String(b64Result));
}


String WebSocketClient::handleStream() {
    uint8_t msgtype;
    uint8_t bite;
    unsigned int length;
    uint8_t mask[4];
    uint8_t index;
    unsigned int i;
    bool hasMask = false;

    // Poll for available characters.
    int time_out{0};
    int chars_avail{0};
    do {
        chars_avail = socket_client->available();
        if (chars_avail > 0) break;
        delay(25);
        time_out += 25;
    } while (time_out <= AVAILABLE_TIMEOUT_MS);

    // String to hold bytes sent by server to client
    String socketString;
    if (socket_client->connected() && socket_client->available()) {
        msgtype = timedRead();
        if (!socket_client->connected()) {
            return socketString;
        }
        length = timedRead();

        if (length & WS_MASK) {
            hasMask = true;
            length = length & ~WS_MASK;
        }

        if (!socket_client->connected()) {
            return socketString;
        }
        index = 6;

        if (length == WS_SIZE16) {
            length = timedRead() << 8;
            if (!socket_client->connected()) {
                return socketString;
            }
            length |= timedRead();
            if (!socket_client->connected()) {
                return socketString;
            }
        } else if (length == WS_SIZE64) {
#ifdef DEBUGGING
            Serial.println(F("Message is larger than 16 bit, attempting to read."));
#endif
            length = timedRead() << 14;
			if (!socket_client->connected()) {
				return socketString;
			}
			length |= timedRead();
			if (!socket_client->connected()) {
				return socketString;
			}
			Serial.println(F("Message attempt finalized."));
        }

        if (hasMask) {
            // get the mask
            mask[0] = timedRead();
            if (!socket_client->connected()) {
                return socketString;
            }
            mask[1] = timedRead();
            if (!socket_client->connected()) {

                return socketString;
            }
            mask[2] = timedRead();
            if (!socket_client->connected()) {
                return socketString;
            }
            mask[3] = timedRead();
            if (!socket_client->connected()) {
                return socketString;
            }
        }

        if (hasMask) {
            for (i=0; i<length; ++i) {
                socketString += (char) (timedRead() ^ mask[i % 4]);
                if (!socket_client->connected()) {
                    return socketString;
                }
            }
        } else {
            for (i=0; i<length; ++i) {
                socketString += (char) timedRead();
                if (!socket_client->connected()) {
                    return socketString;
                }
            }            
        }

    }

    return socketString;
}

void WebSocketClient::disconnectStream() {
#ifdef DEBUGGING
    Serial.println(F("Terminating socket"));
#endif
    // Should send 0x8700 to server to tell it I'm quitting here.
    socket_client->write((uint8_t) 0x87);
    socket_client->write((uint8_t) 0x00);
    
    socket_client->flush();
    delay(10);
    socket_client->stop();
}

String WebSocketClient::getData() {
    String data;
#ifdef DEBUGGING
    Serial.println("Getting data.......");
#endif
    data = handleStream();

    return data;
}

void WebSocketClient::sendData(const char *str) {
#ifdef DEBUGGING
    Serial.print(F("Sending data: "));
    Serial.println(str);
#endif
    if (socket_client->connected()) {
        sendEncodedData(str);       
    }
}

void WebSocketClient::sendData(String str) {
#ifdef DEBUGGING
    Serial.print(F("Sending data: "));
    Serial.println(str);
#endif
    if (socket_client->connected()) {
        sendEncodedData(str);
    }
}

void WebSocketClient::sendData(const char *str, uint8_t opcode) {
#ifdef DEBUGGING
    Serial.print(F("Sending data: "));
    Serial.println(str);
#endif
    if (socket_client->connected()) {
        sendEncodedData(str, opcode);       
    }
}

void WebSocketClient::sendData(String str, uint8_t opcode) {
#ifdef DEBUGGING
    Serial.print(F("Sending data: "));
    Serial.println(str);
#endif
    if (socket_client->connected()) {
        sendEncodedData(str, opcode);
    }
}

int WebSocketClient::timedRead() {
  while (!socket_client->available()) {
    delay(20);  
  }

  return socket_client->read();
}

void WebSocketClient::sendEncodedData(char *str) {
    int size = strlen(str);

    // string type
    socket_client->write(0x81);

    // NOTE: no support for > 16-bit sized messages
    if (size > 125) {
        socket_client->write(126);
        socket_client->write((uint8_t) (size >> 8) & 0xFF);
        socket_client->write((uint8_t) (size & 0xFF));
    } else if (size <= 125) {
        socket_client->write((uint8_t) size);
    } else {
		Serial.print(F("Websocket: Message is too big to send."));
		Serial.println(F("Only messages up to 65535 bytes are allowed."));
	}

    for (int i=0; i<size; ++i) {
        socket_client->write(str[i]);
    }
}

void WebSocketClient::sendEncodedData(String str) {
    int size = str.length() + 1;
    char cstr[size];

    str.toCharArray(cstr, size);

    sendEncodedData(cstr);
}

void WebSocketClient::sendEncodedData(char *str, uint8_t opcode) {
    uint8_t mask[4];
    int size = strlen(str);
    // Opcode; final fragment
	socket_client->write(opcode | WS_FIN);
	
    if (size > 125) {
		if(size == 126) {
			socket_client->write(WS_SIZE16 | WS_MASK);
			socket_client->write((uint8_t) (size >> 8));
			socket_client->write((uint8_t) (size & 0xFF));
		} else if(size == 127) {
			socket_client->write(WS_SIZE64 | WS_MASK);
			socket_client->write((uint8_t) (size >> 14));
			socket_client->write((uint8_t) (size & 0xFF));
		}
    } else {
        socket_client->write((uint8_t) size | WS_MASK);
    }
	
    mask[0] = random(0, 256);
    mask[1] = random(0, 256);
    mask[2] = random(0, 256);
    mask[3] = random(0, 256);
    
    socket_client->write(mask[0]);
    socket_client->write(mask[1]);
    socket_client->write(mask[2]);
    socket_client->write(mask[3]);
    for (int i=0; i<size; ++i) {
        socket_client->write(str[i] ^ mask[i % 4]);
    }
}

void WebSocketClient::sendEncodedData(String str, uint8_t opcode) {
    int size = str.length() + 1;
    char cstr[size];

    str.toCharArray(cstr, size);

    sendEncodedData(cstr, opcode);
}
