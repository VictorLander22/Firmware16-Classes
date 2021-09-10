// void teste()
// {
//   String functionName = gRequest->arg("fn");
//   slogln(functionName);
//   //gRequest = request;
//   //asyncExecuteFunction = true;
//   if (functionName == "teste")
//   {
//     gRequest->send(200, "text/html", "OK");
//   }
//   else
//   {
//     gRequest->send(200, "text/html", "NOK");
//   }
// }

void showDateTime()
{
  Serial.printf("Data hora: %u/%u/%u %u:%u:%u\n", Rtc.day, Rtc.month, Rtc.year, Rtc.hour, Rtc.minute, Rtc.second);
  delay(200);
}

void mostarEEProm()
{
  DevSet.getDeviceSettings();
  DevSet.showVariables();
}
