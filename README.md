# Modulos-do-nucleo-Linux
Escrever um módulo para o núcleo do Linux e descrever passo a passo os métodos e resultados

# Site de referencia

[writing-a-simple-linux-kernel-module](https://blog.sourcerer.io/writing-a-simple-linux-kernel-module-d9dc3762c234)

# Para testar o codigo:

    sudo insmod lkm_example.ko

Deve printar "Hello, World!"

E para remover o modulo:

    sudo rmmod lkm_example

Comando para ver a mensagem no console:

    sudo dmesg
