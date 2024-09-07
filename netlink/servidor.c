#include <linux/module.h>  // Módulo do kernel
#include <linux/kernel.h>  // Kernel do Linux
#include <linux/netlink.h> // Netlink
#include <net/sock.h>      // Socket

MODULE_LICENSE("GPL");                                       // Licença do módulo
MODULE_AUTHOR("Nome");                                       // Autor do módulo
MODULE_DESCRIPTION("Monitoramento de diretório por kernel"); // Descrição do módulo

#define NETLINK_USER 31 // Número do protocolo Netlink

struct sock nl_sk = NULL;                   // Socket Netlink
static char msg[1024] = "Hello from kernel"; // Mensagem a ser enviada

static void nl_recv_msg(struct sk_buffskb) // Função para receber mensagens do Netlink
{
   struct nlmsghdr nlh = (struct nlmsghdr)skb->data; // Cabeçalho da mensagem
   strncpy(msg, (char *)nlmsg_data(nlh), 1024);         // Copia o IP recebido para a variável blocked_ip
   printk(KERN_INFO "Mensagem recebida: %s\n", msg);    // Exibe o IP bloqueado
}

static int init dir_monitor_module_init(void) // Função de inicialização do módulo
{
   // Configuração do socket Netlink
   struct netlink_kernel_cfg cfg = {
       .input = nl_recv_msg, // Função de recebimento de mensagens
   };

   nl_sk = netlink_kernel_create(&init_net, NETLINK_USER, &cfg); // Cria o socket Netlink
   if (!nl_sk)                                                   // Verifica se houve erro na criação do socket
   {
      printk(KERN_ALERT "Erro ao criar socket Netlink\n"); // Exibe mensagem de erro
      return -ENOMEM;                                      // Retorna erro de memória
   }

   printk(KERN_INFO "Monitoramento de diretório iniciado\n"); // Exibe mensagem de inicialização
   return 0;
}

static void exit dir_monitor_module_exit(void) // Função de encerramento do módulo
{
   netlink_kernel_release(nl_sk);                     // Libera o socket Netlink
   printk(KERN_INFO "Firewall Dinâmico Encerrado\n"); // Exibe mensagem de encerramento
}

module_init(dir_monitor_module_init); // Define a função de inicialização
module_exit(dir_monitor_module_exit); // Define a função de encerramento