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
#include <errno.h>

#define SET_DEV_UP 1
#define SET_DEV_DOWN 0
/*
static int get_ipv4_ip_mask(char* dev, char *addr, char *mask)
{
    int rv = 0;
    int sockfd = 0;
    struct sockaddr_in *sin;
    char *ip = NULL;
    struct ifreq ifr;
    memset(&ifr, 0, sizeof(ifr));
    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0){
        return -1;
    }
 
    strcpy(ifr.ifr_name,dev);
    rv = ioctl(sockfd, action, &ifr);
    if(rv == 0){
        sin = (struct sockaddr_in *)&ifr.ifr_addr;
        ip = inet_ntoa(sin->sin_addr);
        strcpy(addr, ip);
    }

    close(sockfd);
    return rv;
}
*/
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

static int set_ipv4_addr_mask(char *dev, char *addr, char *mask)
{
    int sockfd = 0;
    int rv = 0;
    struct ifreq ifr;
    struct sockaddr_in sin;

    if(addr[0] == '\0' || mask[0] == '\0'){
        fprintf(stdout, "[error]: Invalid parameter\n");
        return -1;
    }
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);

    // set ipv4 addr
    memset(&ifr, 0, sizeof(ifr));
    memset(&sin, 0, sizeof(sin));
    strcpy(ifr.ifr_name, dev);
    sin.sin_family = AF_INET;
    inet_pton(AF_INET, addr, &(sin.sin_addr));
    memcpy(&ifr.ifr_addr, &sin, sizeof(struct sockaddr));
    rv = ioctl(sockfd, SIOCSIFADDR, &ifr);
    printf("SIOCSIFADDR rv:%d\n",rv);
    /*
    if((ioctl(sockfd, SIOCSIFADDR, &ifr)) != 0){
        fprintf(stderr, "[error]:SIOCSIFADDR: %s\n",strerror(errno));
        return 1;
    }
    */

    // set ipv4 mask
    memset(&ifr, 0, sizeof(ifr));
    memset(&sin, 0, sizeof(sin));
    strcpy(ifr.ifr_name, dev);
    sin.sin_family = AF_INET;
    inet_pton(AF_INET, mask, &(sin.sin_addr));
    memcpy(&ifr.ifr_netmask, &sin, sizeof(struct sockaddr));
    rv = ioctl(sockfd, SIOCSIFNETMASK, &ifr);
    printf("SIOCSIFNETMASK rv:%d\n",rv);
    close(sockfd);
    return 0;
}



int main()
{
    int rv = 0;
    char ethname[] = "docker0";
    char ipv4_addr[] = {0};
    char ipv4_mask[] = "255.255.0.0";
    char ip[125] = {0};
    char mask[125] = {0};

    //set_iface_flag(ethname, SET_DEV_UP);
    set_ipv4_addr_mask(ethname, "10.70.74.11", "255.255.24a0.0");


    //get_ipv4_ip_mask(ethname, ip, mask)
    //get_addr(ethname, ipv4_addr, SIOCGIFADDR);
    //get_addr(ethname, ipv4addr_mask, SIOCGIFNETMASK);

}