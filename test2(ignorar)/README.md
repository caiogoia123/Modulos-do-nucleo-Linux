# Modulos-do-nucleo-Linux
Escrever um módulo para o núcleo do Linux e descrever passo a passo os métodos e resultados

# Site de referencia

[writing-a-simple-linux-kernel-module](https://blog.sourcerer.io/writing-a-simple-linux-kernel-module-d9dc3762c234)

# Para testar o codigo

Para testar o módulo, execute os seguintes comandos:

    sudo insmod lkm_example.ko

Deve printar "Hello, World!"

E para remover o modulo:

    sudo rmmod lkm_example

Use este comando para verificar as mensagens do núcleo, incluindo "Hello, World!" e "Goodbye, World!":

    sudo dmesg

# Outras funcoes do Makefile

    test

Após executar o make, você pode usar o comando make test para automatizar o processo de teste, evitando a repetição dos comandos acima.