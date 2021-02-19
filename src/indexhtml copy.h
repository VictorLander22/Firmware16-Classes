
#include <ESP8266WebServer.h>
#include "pagehelper.h"
#include "index.html.h"
#include "settings.html.h"
#include "style.css.h"
#include "smiley.jpg.h"
ESP8266WebServer server(80);

const char *ssid = "yourssid";
const char *password = "yourpassword";
void setup()
{

  // HTML - PAGES
  server.on("/", [] { server.send_P(200, "text/html", index_html::HtmlPage); });                          // Startpage
  server.on(settings_html::PageName(), [] { server.send_P(200, "text/html", settings_html::HtmlPage); }); // Settings

  // CSS
  server.on(css$style_css::PageName(), [] { server.send_P(200, "text/html", css$style_css::HtmlPage); });

  // IMAGES
  server.on(images$smiley_jpg::PageName(), [] { server.sendContent_P(images$smiley_jpg::Image, sizeof(images$smiley_jpg::Image)); });

  server.begin();
}

void loop()
{
  server.handleClient();
}
