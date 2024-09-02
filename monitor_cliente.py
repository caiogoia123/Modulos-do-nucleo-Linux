

# O script abaixo utiliza a biblioteca watchdog para monitorar um diretório e websockets para enviar eventos de mudança para um servidor.

import asyncio
import websockets
import os
from watchdog.observers import Observer
from watchdog.events import FileSystemEventHandler

# Configurações do servidor WebSocket
SERVER_URI = "ws://localhost:8765"

# Classe para lidar com eventos de mudança no diretório
class DirectoryMonitor(FileSystemEventHandler):
    def __init__(self, websocket):
        self.websocket = websocket

    async def on_modified(self, event):
        await self.send_event(event)

    async def on_created(self, event):
        await self.send_event(event)

    async def on_deleted(self, event):
        await self.send_event(event)

    async def send_event(self, event):
        if event.is_directory:
            return
        message = f"Evento: {event.event_type} - Arquivo: {event.src_path}"
        await self.websocket.send(message)

# Função principal para monitorar o diretório e conectar ao servidor
async def monitor_directory(directory):
    async with websockets.connect(SERVER_URI) as websocket:
        event_handler = DirectoryMonitor(websocket)
        observer = Observer()
        observer.schedule(event_handler, directory, recursive=True)
        observer.start()
        print(f"Monitorando o diretório: {directory}")

        try:
            while True:
                await asyncio.sleep(1)
        except KeyboardInterrupt:
            observer.stop()
        observer.join()

if __name__ == "__main__":
    directory_to_monitor = "/caminho/para/diretorio"
    asyncio.run(monitor_directory(directory_to_monitor))
