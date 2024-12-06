#include <Arduino.h>
#include <GameManager.h>

GameManager game;

void setup()
{
  Serial.begin(115200);
  game.init();
}
void loop()
{
  game.runningIRRecv();
  game.runEffects();
  game.isPlayerDead();
  game.espnowHandlingPeer();
  game.sendMessageEspNowLoop();
}