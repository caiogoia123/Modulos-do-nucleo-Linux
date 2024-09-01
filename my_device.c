#include <linux/init.h>    // Necessário para as macros de inicialização e limpeza de módulos
#include <linux/module.h>  // Necessário para a criação de módulos do kernel
#include <linux/kernel.h>  // Necessário para usar o printk e outros utilitários do kernel
#include <linux/fs.h>      // Necessário para usar as estruturas de arquivo do kernel
#include <linux/uaccess.h> // Necessário para copiar dados do espaço do usuário para o espaço do kernel

#define DEVICE_NAME "my_device" // Nome do dispositivo no sistema de arquivos
#define BUFFER_SIZE 1024        // Tamanho máximo do buffer para armazenar mensagens


// Variáveis globais para o dispositivo
static int majorNumber;                  // Número maior do dispositivo (identificador único do driver)
static char message[BUFFER_SIZE] = {0};  // Buffer para armazenar mensagens do usuário
static short messageSize;                // Tamanho da mensagem atual no buffer
static int numberOpens = 0;              // Contador de quantas vezes o dispositivo foi aberto
static struct class* myDeviceClass = NULL;  // Ponteiro para a classe do dispositivo
static struct device* myDeviceDevice = NULL; // Ponteiro para o dispositivo do sistema de arquivos

// Função chamada quando o dispositivo é aberto
static int dev_open(struct inode *inodep, struct file *filep) {
    numberOpens++;  // Incrementa o contador de vezes que o dispositivo foi aberto
    printk(KERN_INFO "MyDevice: Dispositivo aberto %d vezes.\n", numberOpens);
    return 0; // Retorna 0 para indicar sucesso
}

// Função chamada quando o dispositivo é fechado/liberado
static int dev_release(struct inode *inodep, struct file *filep) {
    printk(KERN_INFO "MyDevice: Dispositivo fechado.\n");
    return 0; // Retorna 0 para indicar sucesso
}

// Função chamada quando o dispositivo é lido
static ssize_t dev_read(struct file *filep, char *buffer, size_t len, loff_t *offset) {
    int errorCount = 0;
    // Copia dados do buffer do kernel (message) para o espaço do usuário (buffer)
    errorCount = copy_to_user(buffer, message, messageSize);

    if (errorCount == 0) { // Se a cópia foi bem-sucedida
        printk(KERN_INFO "MyDevice: Enviou %d caracteres para o usuário.\n", messageSize);
        return (messageSize = 0); // Reseta o tamanho da mensagem e retorna 0
    } else {
        printk(KERN_INFO "MyDevice: Falha ao enviar %d caracteres para o usuário.\n", errorCount);
        return -EFAULT; // Retorna um erro se houve falha na cópia
    }
}

// Função chamada quando o dispositivo é escrito
static ssize_t dev_write(struct file *filep, const char *buffer, size_t len, loff_t *offset) {
    // Copia dados do espaço do usuário (buffer) para o buffer do kernel (message)
    snprintf(message, BUFFER_SIZE, "%s(%zu letras)", buffer, len); 
    messageSize = strlen(message); // Atualiza o tamanho da mensagem armazenada
    printk(KERN_INFO "MyDevice: Recebeu %zu caracteres do usuário.\n", len);
    return len; // Retorna o número de bytes recebidos
}

// Estrutura que define as operações suportadas pelo dispositivo
static struct file_operations fops = {
    .open = dev_open,      // Função chamada quando o dispositivo é aberto
    .read = dev_read,      // Função chamada quando o dispositivo é lido
    .write = dev_write,    // Função chamada quando o dispositivo é escrito
    .release = dev_release // Função chamada quando o dispositivo é fechado
};

// Função de inicialização do módulo (chamada quando o módulo é carregado)
static int __init myDevice_init(void) {
    printk(KERN_INFO "MyDevice: Inicializando o módulo MyDevice\n");
    
    // Registra o dispositivo de caractere e obtém o número maior
    majorNumber = register_chrdev(0, DEVICE_NAME, &fops);
    if (majorNumber < 0) { // Verifica se houve erro no registro
        printk(KERN_ALERT "MyDevice: Falha ao registrar um número de dispositivo\n");
        return majorNumber; // Retorna o erro
    }
    printk(KERN_INFO "MyDevice: Registrado corretamente com o número maior %d\n", majorNumber);
    return 0; // Retorna 0 para indicar sucesso
}

// Função de limpeza do módulo (chamada quando o módulo é descarregado)
static void __exit myDevice_exit(void) {
    unregister_chrdev(majorNumber, DEVICE_NAME); // Remove o registro do dispositivo
    printk(KERN_INFO "MyDevice: Módulo descarregado.\n");
}

// Macros para definir as funções de inicialização e limpeza do módulo
module_init(myDevice_init);
module_exit(myDevice_exit);
