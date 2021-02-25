/*
 * uMQTTBroker demo for Arduino (C++-style)
 * 
 * The program defines a custom broker class with callbacks, 
 * starts it, subscribes locally to anything, and publishs a topic every second.
 * Try to connect from a remote client and publish something - the console will show this as well.
 */
#include <string>
//#include <ESP8266WiFi.h>
#include <PubSubClient.h>

bool WiFiAP = false; // Do yo want the ESP as AP?
//const char *mqtt_server = "168.138.146.38";
const char *mqtt_server = "cloudmqtt.keepin.com.br";
const char *mqtt_server_user = "keepinadm";
const char *mqtt_server_userpw = "Keepin@2020@Cloud";

//long lastMsg = 0;
//char msg[50];
//int value = 0;

String idChip = "";
const char *mqttTopicoCloud;
const char *mqttTopicoCloudRet;
String str = "";
String strRet = "";
String *clientestr;

WiFiClient espClient;
PubSubClient client(espClient);

// recebido mqtt pela cloud
void callback(char *topic, byte *payload, unsigned int length)
{
  String strRec = "";
  char data_str[length + 1];
  os_memcpy(data_str, payload, length);
  data_str[length] = '\0';

  strRec = (String)data_str;

  msgMqtt = data_str;
  newMqttMsg = true;

  Serial.println("mqttcloud: " + String(topic) + "-" + (String)data_str);

  String str = "Recibido Cloud: " + strRec;
  const char *cloudStr = str.c_str();

  client.publish(mqttTopicoCloudRet, cloudStr);

  api();
}
/*
 * WiFi init stuff
 */

void MqttCloudReconnect()
{
  // Loop until we're reconnected
  Serial.println("Iniciando MQTT connection...");

  if (!client.connected())
  {
    Serial.print("Attempting MQTT connection...");
    // Create a random client ID
    String clientId = "ESP8266Client-";
    clientId += String(random(0xffff), HEX);
    // Attempt to connect
    if (client.connect(clientId.c_str(), mqtt_server_user, mqtt_server_userpw))
    {
      Serial.println("connected");
      // Once connected, publish an announcement...
      client.publish("outTopic", "hello world");
      // ... and resubscribe
      client.subscribe(mqttTopicoCloud);

      Serial.println("WiFi connected");
      Serial.println("IP address: " + WiFi.localIP().toString());

      NtpSetDateTimeNTP();
    }
    else
    {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      //delay(5000);
    }
  }
}

void MqttSetup()
{
  Serial.println();

  str = "keepin/placas/" + gchipId + "/cloud";
  strRet = str + "/ret";
  mqttTopicoCloud = str.c_str();
  mqttTopicoCloudRet = strRet.c_str();

  Serial.println(mqtt_server);
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
  client.setBufferSize(2048);
  Serial.println("Id Chip: " + gchipId);

  Serial.println("Subscribe cloud: " + (String)mqttTopicoCloud);
}

void MqttLoop()
{
  /*
 * Publish the counter value as String
 */

  if (!client.connected() && (tipoWifiAtual != 2) && ((millisAtual >= millisMqttReconnect)))
  {
    millisMqttReconnect = millisAtual + 30000;
    MqttCloudReconnect();
  }
  client.loop();
}

String MqttArg(char *msg, char *pkey)
{
  char *pch;
  char *lch;

  char key[] = "";
  int plen, llen;

  strcat(key, pkey);
  strcat(key, "=");

  pch = strstr(msg, key);

  if (pch != NULL)
  {

    pch = strstr(msg, key) + strlen(key);

    plen = strlen(pch);

    lch = strchr(pch, '&');

    if (lch == NULL)
    {
      lch = strchr(pch, '\0');
      llen = 0;
    }
    llen = strlen(lch);
  }
  else
    plen = 0;

  char strcp[(plen - llen)];

  if (pch != NULL)
  {
    String msgtrata = pch;
    msgtrata.remove(plen - llen);

    return msgtrata;
  }
  else
    return "";
}