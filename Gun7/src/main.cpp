#include "GameManager.h"

GameManager gameManager;

void setup()
{
  Serial.begin(115200);
  gameManager.init();
}

void loop()
{
  if (WiFi.status() != WL_CONNECTED)
    gameManager.wServerHandlingClient();
  gameManager.setValidToPlay();
  gameManager.espNowHandlingPeer();
  gameManager.activateButtons();
  gameManager.checkReloadStatus();
  gameManager.sendMessageEspNowLoop();
  gameManager.sendBinaryToWSocketLoop();
}