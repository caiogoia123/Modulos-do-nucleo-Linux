import asyncio
import websockets
import os
from watchdog.observers import Observer
from watchdog.events import FileSystemEventHandler

# Configurações do servidor WebSocket
# Coloque o seu IPv4 no lugar do localhost para funcionar !
SERVER_URI = "ws://localhost:8765"

# Classe para lidar com eventos de mudança no diretório
class DirectoryMonitor(FileSystemEventHandler):
    def __init__(self, websocket):
        self.websocket = websocket
        self.loop = asyncio.get_event_loop()

    async def send_event(self, event):
        if event.is_directory:
            return
        message = f"Evento: {event.event_type} - Arquivo: {event.src_path}"
        await self.websocket.send(message)

    def on_modified(self, event):
        self.loop.create_task(self.send_event(event))

    def on_created(self, event):
        self.loop.create_task(self.send_event(event))

    def on_deleted(self, event):
        self.loop.create_task(self.send_event(event))

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
    directory_to_monitor = os.path.expanduser("~/Desktop/FACULDADE/BCC-5-SEMESTRE/SO/Modulos-do-nucleo-Linux")
    asyncio.run(monitor_directory(directory_to_monitor))



