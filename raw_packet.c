#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/ioctl.h>
#include <sys/stat.h>
#include <net/ethernet.h>
#include <netinet/in.h>
#include <linux/if_arp.h>

int main(int argc,char** argv) {
  int packet_socket;
  struct ifreq etherreq;
  struct sockaddr_ll ll;
  unsigned char dst[6];
  unsigned char buf[ETH_FRAME_LEN];
  unsigned flength;
  int fd;

  if (argc != 5) {
    printf("usage: RAW_Packet <iface> <destination> <payloadproto> <payloadfile>\n");
    return EXIT_FAILURE;
  }

  strncpy(etherreq.ifr_name,argv[1],IFNAMSIZ);
  char *tok;
  if((tok = strtok(argv[2],":")) != NULL) {
    dst[0] = (char) strtol(tok,NULL,16);
    int i = 1;
    while((tok = strtok(NULL,":")) != NULL) {
      dst[i] = (char) strtol(tok,NULL,16);
      i++;
    }
  }
  int proto = strtol(argv[3],NULL,16);

  if((fd = open(argv[4],O_RDONLY)) < 0) {
    perror("open error");
    return EXIT_FAILURE;
  }
  flength = (unsigned) lseek(fd,0,SEEK_END);
  lseek(fd,0,SEEK_SET);
  read(fd,buf,flength);
  close(fd);

  packet_socket = socket(PF_PACKET,SOCK_DGRAM,htons(ETH_P_ALL));
  ll.sll_family = PF_PACKET;
  ll.sll_protocol = htons(proto);
  ioctl(packet_socket,SIOCGIFINDEX, &etherreq);
  ll.sll_ifindex = etherreq.ifr_ifindex;
  ll.sll_halen = 0x06;
  memcpy(&ll.sll_addr,&dst,6);

  sendto(packet_socket,&buf,flength,0,(struct sockaddr*) &ll,sizeof(ll));
  close(packet_socket);
  return EXIT_SUCCESS;
}
