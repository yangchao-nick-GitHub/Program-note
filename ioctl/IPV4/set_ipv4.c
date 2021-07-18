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
#include <net/route.h>


#define SET_DEV_UP 1
#define SET_DEV_DOWN 0

static int set_ipv4_gateway(char *dev, char *gateway)
{
    int sockfd = 0;;
    int rv = 0;
    char iface[128] = {0};
    struct rtentry rt;
    struct sockaddr_in sockaddr;
    
    memset(&rt, 0, sizeof(rt));
    memset(&sockaddr, 0, sizeof(sockaddr));
    strcpy(iface, dev);
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0){
        return -1;
    }
 
    sockaddr.sin_family = AF_INET;
    rv = inet_pton(AF_INET, gateway, &(sockaddr.sin_addr));
    if (rv != 1){
        fprintf(stderr, "[Error]:Invalid gateway: %s\n", gateway);
        close(sockfd);
        return  -1;
    }

    memcpy (&rt.rt_gateway, &sockaddr, sizeof(struct sockaddr_in));
    rt.rt_dst.sa_family = AF_INET;
    rt.rt_genmask.sa_family = AF_INET;
    rt.rt_flags = RTF_GATEWAY;
    rt.rt_dev = iface;

    rv = ioctl(sockfd, SIOCADDRT, &rt);
    if (rv != 0){
        fprintf(stderr, "[Error]:SIOCADDRT: %s\n",strerror(errno));
        close(sockfd);
        return rv;
    }

   close(sockfd);
   return rv;
}

static int get_ipv4_ip_mask(char* dev, char *addr, char *mask)
{
    int rv = 0;
    int sockfd = 0;
    char ip[128] = {0};
    char ip_mask[128] = {0};
    struct sockaddr_in *sin;
    struct ifreq ifr;

    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0){
        return -1;
    }
    
    // get ipv4 ip
    memset(&ifr, 0, sizeof(ifr));
    strcpy(ifr.ifr_name,dev);
    rv = ioctl(sockfd, SIOCGIFADDR, &ifr);
    if (rv != 0){
        fprintf(stderr, "[Error]:SIOCGIFADDR: %s\n",strerror(errno));
        close(sockfd);
        return -1;
    }
    sin = (struct sockaddr_in *)&(ifr.ifr_netmask);
    inet_ntop(AF_INET, &(sin->sin_addr), ip, sizeof(ip));
    if (ip[0] == '\0'){
        fprintf(stderr, "inet_ntop converted ip error\n");
        close(sockfd);
        return  -1;
    }
    strcpy(addr, ip);
    

    // get ipv4 mask
    memset(&ifr, 0, sizeof(ifr));
    strcpy(ifr.ifr_name,dev);
    rv = ioctl(sockfd, SIOCGIFNETMASK, &ifr);
    if (rv != 0){
        fprintf(stderr, "[Error]:SIOCGIFNETMASK: %s\n",strerror(errno));
        close(sockfd);
        return -1;
    }
    sin = (struct sockaddr_in *)&(ifr.ifr_addr);
    inet_ntop(AF_INET, &(sin->sin_addr), ip_mask, sizeof(ip_mask));
    if (ip_mask[0] == '\0'){
        fprintf(stderr, "inet_ntop converted ip error\n");
        close(sockfd);
        return -1;
    }
    strcpy(mask, ip_mask);

    close(sockfd);
    return rv;
}

static int set_iface_flag(char * dev, int action)
{
    int rv = 0;
    int sockfd = 0;
    int flag = 0;
    struct ifreq ifr;

    memset(&ifr, 0, sizeof(ifr));
    if((sockfd = socket(AF_INET,SOCK_STREAM,0)) < 0){
        return -1;
    }
    strcpy(ifr.ifr_name,dev);
    rv = ioctl(sockfd,SIOCGIFFLAGS,&ifr);
    if (rv != 0){
        fprintf(stderr, "[Error]:SIOCGIFFLAGS: %s\n",strerror(errno));
        close(sockfd);
        return 1;
    }
    if(action == SET_DEV_DOWN){
        flag = ~IFF_UP;
        ifr.ifr_flags &= flag;
    }else if(action = SET_DEV_UP){
        flag = IFF_UP | IFF_RUNNING;
        ifr.ifr_flags |= flag;
    }
    rv = ioctl(sockfd,SIOCSIFFLAGS,&ifr);
    if (rv != 0){
        fprintf(stderr, "[Error]:SIOCSIFFLAGS: %s\n",strerror(errno));
        close(sockfd);
        return 1;
    }
    close(sockfd);
    return rv;
}

static int set_ipv4_addr_mask(char *dev, char *addr, char *mask)
{
    int sockfd = 0;
    int rv = 0;
    struct ifreq ifr;
    struct sockaddr_in sin;

    if(addr[0] == '\0' || mask[0] == '\0'){
        fprintf(stderr, "[Error]: Invalid add or mask\n");
        return -1;
    }
    if((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0){
        return -1;
    }

    // set ipv4 addr
    memset(&ifr, 0, sizeof(ifr));
    memset(&sin, 0, sizeof(sin));
    strcpy(ifr.ifr_name, dev);
    sin.sin_family = AF_INET;
    rv = inet_pton(AF_INET, addr, &(sin.sin_addr));
    if (rv != 1){
        fprintf(stderr, "Invalid addr: %s\n",addr);
        close(sockfd);
        return -1;
    }
    memcpy(&ifr.ifr_addr, &sin, sizeof(struct sockaddr));
    rv = ioctl(sockfd, SIOCSIFADDR, &ifr);
    if (rv != 0){
        fprintf(stderr, "[Error]:SIOCSIFADDR: %s\n",strerror(errno));
        close(sockfd);
        return 1;
    }

    // set ipv4 mask
    memset(&ifr, 0, sizeof(ifr));
    memset(&sin, 0, sizeof(sin));
    strcpy(ifr.ifr_name, dev);
    sin.sin_family = AF_INET;
    rv = inet_pton(AF_INET, mask, &(sin.sin_addr));
    if (rv != 1){
        fprintf(stderr, "Invalid mask: %s\n",mask);
        close(sockfd);
        return -1;
    }
    memcpy(&ifr.ifr_netmask, &sin, sizeof(struct sockaddr));
    rv = ioctl(sockfd, SIOCSIFNETMASK, &ifr);
    if (rv != 0){
        fprintf(stderr, "[Error]:SIOCSIFADDR: %s\n",strerror(errno));
        close(sockfd);
        return 1;
    }
    close(sockfd);
    return 0;
}



int main()
{
    char ethname[] = "docker0";
    char ipv4_addr[] = "172.21.24.13";
    char ipv4_mask[] = "255.255.0.0";
    char gateway[] = "172.17.1.6";
    char ip[128] = {0};
    char mask[128] = {0};

    //set_iface_flag(ethname, SET_DEV_UP);

    //set_ipv4_addr_mask(ethname, ipv4_addr, ipv4_mask);

    //get_ipv4_ip_mask(ethname, ip, mask);

    set_ipv4_gateway(ethname, gateway);

    return 0;

}