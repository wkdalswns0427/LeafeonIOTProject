#ifndef mywifi_H
#define mywifi_H

void setup_wifi();
void callback(char* topic, byte* payload, unsigned int length);
void reconnect();

#endif