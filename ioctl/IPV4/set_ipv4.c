#include <stdio.h>
#include <net/if.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <fcntl.h> 
#include <unistd.h>

#define SET_DEV_UP 1
#define SET_DEV_DOWN 0

static int set_iface_flag(char * dev, int action)
{
    int rv = 0;
    int sockfd;
    int flag;
    struct ifreq ifr;

    memset(&ifr, 0, sizeof(ifr));
    if((sockfd = socket(PF_INET,SOCK_STREAM,0)) < 0){
        return -1;
    }
    strcpy(ifr.ifr_name,dev);
    rv = ioctl(sockfd,SIOCGIFFLAGS,&ifr);
    if(action == SET_DEV_DOWN){
        flag = ~IFF_UP;
        ifr.ifr_flags &= flag;
    }else if(action = SET_DEV_UP){
        flag = IFF_UP | IFF_RUNNING;
        ifr.ifr_flags |= flag;
    }
    rv = ioctl(sockfd,SIOCSIFFLAGS,&ifr);
    if(rv != 0){
        printf("ioctl error \n");
    }
    return rv;
}

static int set_ipv4_addr(char *dev, char *addr, int action)
{
    int sfd = 0;
    int rv = 0;
    struct ifreq ifr;
    struct sockaddr_in sin;

    memset(&ifr, 0, sizeof(ifr));
    memset(&sin, 0, sizeof(sin));
    if(addr[0] == '\0'){
        return -1;
    }
    sfd = socket(AF_INET, SOCK_DGRAM, 0);
    strcpy(ifr.ifr_name, dev);
    sin.sin_family = AF_INET;
    inet_pton(AF_INET, addr, &(sin.sin_addr));
    memcpy(&ifr.ifr_addr, &sin, sizeof(struct sockaddr));
    rv = ioctl(sfd, action, &ifr);
    close(sfd);
    return rv;
}

static int set_ipv4_mask(char *dev, char *addr)
{
    int sockfd = 0;
    int rv = 0;
    struct ifreq ifr;
    struct sockaddr_in sin;

    memset(&ifr, 0, sizeof(ifr));
    memset(&sin, 0, sizeof(sin));
    strcpy(ifr.ifr_name, dev);

    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    sin.sin_family = AF_INET;
    rv = inet_pton(AF_INET, addr, &(sin.sin_addr));
    if (rv != 1){
        printf("Invalid address\n");
        return -1;
    }
    memcpy((char *)&ifr.ifr_addr, (char *)&sin, sizeof(struct sockaddr));
    rv = ioctl(sockfd, SIOCSIFNETMASK, &ifr);
    close(sockfd);
    return rv;
}

int main()
{
    int rv = 0;
    char ethname[] = "docker0";
    char ipv4_addr[] = "10.70.70.111";
    char ipv4_mask[] = "255.255.2556.0";

    //set_iface_flag(ethname, SET_DEV_UP);
    set_ipv4_mask(ethname, ipv4_mask);

    //set_ipaddr(ethname, ipv4_addr, SIOCSIFADDR);
    //set_ipaddr(ethname, ipv4_mask, SIOCSIFNETMASK); 

    //get_addr(ethname, ipv4_addr, SIOCGIFADDR);
    //get_addr(ethname, ipv4addr_mask, SIOCGIFNETMASK);

}