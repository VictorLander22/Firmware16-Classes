/*
 * uMQTTBroker demo for Arduino (C++-style)
 * 
 * The program defines a custom broker class with callbacks, 
 * starts it, subscribes locally to anything, and publishs a topic every second.
 * Try to connect from a remote client and publish something - the console will show this as well.
 */
#include <string>
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
  payload[length] = '\0';

  String strRec = (String)(char *)payload;

  newMqttMsg = true;

  (!DEBUG_ON) ?: Serial.println("mqttcloud: " + String(topic) + "-" + (String)msgMqtt);

  String str = "Recibido MQTT: " + strRec;

  client.publish(mqttTopicoCloudRet, str.c_str());

  msgMqtt = (char *)strRec.c_str();

  api();
}

void MqttCloudReconnect()
{

  (!DEBUG_ON) ?: Serial.println(F("Iniciando MQTT connection..."));

  if (!client.connected())
  {
    //(!DEBUG_ON) ?:
    (!DEBUG_ON) ?: Serial.print(F("Attempting MQTT connection..."));
    // Create a random client ID
    String clientId = "ESP8266Client-";
    clientId += String(random(0xffff), HEX);
    // Attempt to connect
    if (client.connect(clientId.c_str(), mqtt_server_user, mqtt_server_userpw))
    {
      //(!DEBUG_ON) ?:
      (!DEBUG_ON) ?: Serial.println("connected");
      // Once connected, publish an announcement...
      client.publish("outTopic", "hello world");
      // ... and resubscribe
      client.subscribe(mqttTopicoCloud);

      //(!DEBUG_ON) ?:
      (!DEBUG_ON) ?: Serial.println("WiFi connected");
      //(!DEBUG_ON) ?:
      (!DEBUG_ON) ?: Serial.println("IP address: " + WiFi.localIP().toString());

      NtpSetDateTimeNTP();
      UpdatePing();
      hasInternet = true;
      hasMQTT = true;
    }
    else
    {
      //(!DEBUG_ON) ?:
      hasMQTT = false;
      (!DEBUG_ON) ?: Serial.print("failed, rc=");
      //(!DEBUG_ON) ?:
      Serial.print(client.state());
      //(!DEBUG_ON) ?:
      (!DEBUG_ON) ?: Serial.println(" try again in 30 seconds");
    }
    enableConnection = false;
  }
}

void MqttSetup()
{
  //(!DEBUG_ON) ?:
  (!DEBUG_ON) ?: Serial.println();

  str = "keepin/placas/" + gchipId + "/cloud";
  strRet = str + "/ret";
  mqttTopicoCloud = str.c_str();
  mqttTopicoCloudRet = strRet.c_str();

  //(!DEBUG_ON) ?:
  (!DEBUG_ON) ?: Serial.println(mqtt_server);
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
  client.setBufferSize(2048);
  //(!DEBUG_ON) ?:
  (!DEBUG_ON) ?: Serial.println("Id Chip: " + gchipId);

  //(!DEBUG_ON) ?:
  (!DEBUG_ON) ?: Serial.println("Subscribe cloud: " + (String)mqttTopicoCloud);
}

void MqttLoop()
{
  /*
 * Publish the counter value as String
 */

  if (!client.connected() && (tipoWifiAtual != 2) && ((millisAtual >= millisMqttReconnect)))
  {
    (!DEBUG_ON) ?: Serial.println("MQTT disconnected!!!");

    millisMqttReconnect = millisAtual + 60000;

    if (enableConnection)
    {
      MqttCloudReconnect();
    }
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
    pch = pch + strlen(pkey) + 1;
    plen = strlen(pch);
    lch = strchr(pch, '&');
    if (lch == NULL)
    {
      lch = strchr(pch, '\0');
      llen = 0;
    }
    llen = strlen(lch);

    String msgtrata = pch;
    msgtrata.remove(plen - llen);
    (!DEBUG_ON) ?: Serial.print(key);
    (!DEBUG_ON) ?: Serial.println(msgtrata);
    return msgtrata;
  }
  else
  {
    (!DEBUG_ON) ?: Serial.print(key);
    (!DEBUG_ON) ?: Serial.println("");
    return "";
  }
}
