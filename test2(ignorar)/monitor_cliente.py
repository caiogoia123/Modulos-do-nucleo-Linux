import asyncio
import websockets
import os

# Configurações do servidor WebSocket
SERVER_URI = "ws://localhost:8765"
DEVICE_PATH = "/dev/file_monitor"

async def send_events_to_server(websocket):
    try:
        with open(DEVICE_PATH, 'r') as device:
            while True:
                # Lê o evento do dispositivo de caracteres
                event = device.read(256)
                if event:
                    # Envia o evento para o servidor WebSocket
                    await websocket.send(event)
                else:
                    # Se não há eventos, aguarda um pouco antes de tentar ler novamente
                    await asyncio.sleep(1)
    except Exception as e:
        print(f"Erro ao ler do dispositivo de caracteres: {e}")

async def monitor_directory():
    try:
        async with websockets.connect(SERVER_URI) as websocket:
            await send_events_to_server(websocket)
    except websockets.ConnectionClosed as e:
        print(f"Conexão WebSocket fechada: {e}")
    except Exception as e:
        print(f"Erro na conexão WebSocket: {e}")

if __name__ == "__main__":
    asyncio.run(monitor_directory())
