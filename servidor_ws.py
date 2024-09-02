# O script a seguir cria um servidor WebSocket simples que escuta as conexões e recebe mensagens.

import asyncio
import websockets

async def server(websocket, path):
    async for message in websocket:
        print(f"Recebido: {message}")

async def main():
    async with websockets.serve(server, "localhost", 8765):
        print("Servidor WebSocket iniciado em ws://localhost:8765")
        await asyncio.Future()  # Mantém o servidor rodando

if __name__ == "__main__":
    asyncio.run(main())
