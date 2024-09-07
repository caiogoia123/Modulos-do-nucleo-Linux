#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <linux/netlink.h>
#include <sys/socket.h>

#define NETLINK_USER 31

struct sockaddr_nl src_addr, dest_addr;
struct nlmsghdr nlh = NULL;
struct iovec iov;
int sock_fd;
struct msghdr msg;

int main()
{
  char input[1024];

  sock_fd = socket(PF_NETLINK, SOCK_RAW, NETLINK_USER);
  if (sock_fd < 0)
  {
    perror("Erro ao criar socket");
    return -1;
  }

  memset(&src_addr, 0, sizeof(src_addr));
  src_addr.nl_family = AF_NETLINK;
  src_addr.nl_pid = getpid();

  bind(sock_fd, (struct sockaddr)&src_addr, sizeof(src_addr));

  memset(&dest_addr, 0, sizeof(dest_addr));
  dest_addr.nl_family = AF_NETLINK;
  dest_addr.nl_pid = 0;
  dest_addr.nl_groups = 0;

  nlh = (struct nlmsghdr )malloc(NLMSG_SPACE(1024));
  memset(nlh, 0, NLMSG_SPACE(1024));
  nlh->nlmsg_len = NLMSG_SPACE(1024);
  nlh->nlmsg_pid = getpid();
  nlh->nlmsg_flags = 0;

  iov.iov_base = (void)nlh;
  iov.iov_len = nlh->nlmsg_len;
  msg.msg_name = (void *)&dest_addr;
  msg.msg_namelen = sizeof(dest_addr);
  msg.msg_iov = &iov;
  msg.msg_iovlen = 1;

  while (1)
  {
    printf("Digite alguma coisa (ou 'sair' para encerrar): ");
    fgets(input, sizeof(input), stdin);
    input[strcspn(input, "\n")] = '\0'; // Remove o caractere de nova linha

    if (strcmp(input, "sair") == 0)
    {
      break;
    }

    strcpy(NLMSG_DATA(nlh), input);
    sendmsg(sock_fd, &msg, 0);
  }

  close(sock_fd);
  return 0;
}
