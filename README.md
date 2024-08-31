# Modulos-do-nucleo-Linux
Escrever um módulo para o núcleo do Linux e descrever passo a passo os métodos e resultados

# Funcionamento do Código do Módulo de Kernel
O código que desenvolvemos é um módulo de kernel do Linux que implementa um dispositivo de caractere. Este dispositivo simula um driver simples que permite que o kernel interaja com operações de entrada e saída (I/O), como leitura e escrita, registrando essas operações no log do sistema.

# Testando o Módulo de Kernel
Para testar o módulo de kernel, siga os passos abaixo:

1. Preparando o Ambiente
Antes de começar, certifique-se de que você possui um ambiente de desenvolvimento adequado para compilar e testar módulos do kernel. Você precisará:

Um sistema operacional Linux (preferencialmente uma distribuição como Ubuntu, Fedora ou CentOS).
Cabeçalhos do kernel instalados (pacote linux-headers ou equivalente para sua distribuição).
Ferramentas de compilação como gcc e make.
Instale os pacotes necessários executando os seguintes comandos (para Ubuntu/Debian):

    sudo apt-get update
    sudo apt-get install build-essential linux-headers-$(uname -r)

2. Escrevendo o Código do Módulo
Crie um arquivo chamado my_device.c e copie o código fornecido para dentro deste arquivo.

3. Criando o Makefile
Crie um arquivo chamado Makefile no mesmo diretório e adicione o seguinte conteúdo:

    obj-m += my_device.o

    all:
        make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules

    clean:
        make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean

Este Makefile informa ao sistema de compilação do kernel como compilar o módulo.

4. Compilando o Módulo
No terminal, navegue até o diretório onde estão os arquivos my_device.c e Makefile e execute:

    make

5. Inserindo o Módulo no Kernel
Para inserir o módulo no kernel, use o comando insmod:

    sudo insmod my_device.ko

Após a execução desse comando, o módulo será carregado no kernel. Para verificar se o módulo foi carregado corretamente, 
você pode usar o comando lsmod:

    lsmod | grep my_device

Para ver as mensagens de log registradas pelo módulo, use o comando dmesg:

    dmesg | tail

6. Interagindo com o Dispositivo
Uma vez que o módulo esteja carregado, ele criará um dispositivo de caractere. Use o comando dmesg para verificar o número maior atribuído ao dispositivo (por exemplo, "Registrado corretamente com o número maior 240").

Crie um arquivo de dispositivo com o comando:

    sudo mknod /dev/my_device c <major_number> 0

Substitua <major_number> pelo número maior exibido no log (dmesg).

Agora você pode interagir com o dispositivo usando comandos como echo e cat:

Escrever para o dispositivo:

    echo "Olá, Kernel!" > /dev/my_device

Ler do dispositivo:

    cat /dev/my_device

7. Removendo o Módulo do Kernel
Para remover o módulo, use o comando rmmod:

    sudo rmmod my_device

Verifique novamente o log para ver as mensagens de descarregamento:

    dmesg | tail

8. Limpeza
Para limpar os arquivos gerados durante a compilação, execute:

    make clean


# Conclusão
Ao seguir esses passos, você testará o módulo de kernel no Linux e entenderá como ele interage com o kernel e o espaço do usuário. O módulo implementa operações básicas de leitura e escrita de um dispositivo de caractere, demonstrando como é possível criar um driver simples para dispositivos no Linux.

Este é um bom ponto de partida para explorar funcionalidades mais complexas em módulos de kernel, como manipulação de hardware específico ou otimização de desempenho de I/O.
