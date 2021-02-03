#include <iostream>
#include <cstring>

#include <SDL2/SDL.h>
#include <SDL2/SDL_net.h>
#include <GL/glew.h>

int main(int argc, char *argv[])
{
  SDL_Init(SDL_INIT_EVERYTHING);
  SDLNet_Init();

  IPaddress ip;
  SDLNet_ResolveHost(&ip, NULL, 8080);

  TCPsocket server = SDLNet_TCP_Open(&ip);
  TCPsocket client;

  const char* text =
    "HTTP/1.1 200 OK\r\n"
    "Connection: close\r\n"
    "Content-Type: text/plain\r\n\r\n"
    "Hello client!";
  while(1)
  {
      client = SDLNet_TCP_Accept(server);
      if(client)
      {
	SDLNet_TCP_Send(client, text, strlen(text) + 1);
	SDLNet_TCP_Close(client);
	break;
      }
  }
  SDLNet_TCP_Close(server);

  SDLNet_Quit();
  SDL_Quit();
}
