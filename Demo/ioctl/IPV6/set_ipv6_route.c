#include <net/route.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <sys/ioctl.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <unistd.h>
#include <net/if.h>      
#include <netdb.h>

#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <stdlib.h>
#include <string.h>
#include <sys/prctl.h>
#include <sys/param.h>
#include <sys/wait.h>
#include <errno.h>
#include <paths.h>
#include <sys/syscall.h>
#include <sys/resource.h>
#include <net/if.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <ifaddrs.h>
#include <sys/ioctl.h>
#include <sys/types.h>    
#include <sys/socket.h>      
#include <net/route.h>


#define RTACTION_ADD 1   /* add action */
#define RTACTION_DEL 2   /* del action */

#define PROCNET_ROUTE_IPV6_PATH   "/proc/net/ipv6_route"
static int get_ipv6_gateway(const char *ifname, char *addr, int length)
{
    FILE *fp;
    char addr6[46] = {0};
    char tmpAdd6[8][5];
    char tmpOther[9][64];
    struct sockaddr_in6 sap = {0};
    
    if (!ifname || !addr || length <= 0)
        return -1;
 
    fp = fopen(PROCNET_ROUTE_IPV6_PATH, "r");
    if (!fp){
        return -1;
    }

    lockf(fileno(fp), F_LOCK, 0);
    while (fscanf(fp, "%s %02s %s %s %4s%4s%4s%4s%4s%4s%4s%4s %s %s %s %s %20s\n",
        tmpOther[0], tmpOther[1], tmpOther[2], tmpOther[3],
        tmpAdd6[0], tmpAdd6[1], tmpAdd6[2], tmpAdd6[3],
        tmpAdd6[4], tmpAdd6[5], tmpAdd6[6], tmpAdd6[7],
        tmpOther[4], tmpOther[5], tmpOther[6], tmpOther[7], tmpOther[8]) != EOF) { 
        if (!strcmp(tmpOther[8], ifname) && atoi(tmpOther[1]) == 0) {
            sprintf(addr6, "%s:%s:%s:%s:%s:%s:%s:%s",
                tmpAdd6[0], tmpAdd6[1], tmpAdd6[2], tmpAdd6[3],
                tmpAdd6[4], tmpAdd6[5], tmpAdd6[6], tmpAdd6[7]);        
                //fe80 Scope:Link
                inet_pton(AF_INET6, addr6, &sap.sin6_addr);
                inet_ntop(AF_INET6, &sap.sin6_addr, addr, length);
                lockf(fileno(fp), F_ULOCK, 0);
                fclose(fp);
                return 0;
        }
    }
    lockf(fileno(fp), F_ULOCK, 0);
    fclose(fp);
    return -1;
}



static int INET6_getsock(char *bufp, struct sockaddr *sap)
{
    struct sockaddr_in6 *sin6 = NULL;
    sin6 = (struct sockaddr_in6 *) sap;
    sin6->sin6_family = AF_INET6;
    sin6->sin6_port = 0;
    if (inet_pton(AF_INET6, bufp, sin6->sin6_addr.s6_addr) <= 0){
        return -1;
    }
    return 0;         
}

static int INET6_resolve(char *name, struct sockaddr_in6 *sin6)
{
    struct addrinfo req;
    struct addrinfo *ai = NULL;
    int s;
    memset (&req, 0, sizeof(req));
    req.ai_family = AF_INET6;
    if ((s = getaddrinfo(name, NULL, &req, &ai))){
        return -1;
    }
    memcpy(sin6, ai->ai_addr, sizeof(struct sockaddr_in6));
    freeaddrinfo(ai);
    return 0;
}

static int INET6_input(int type, char *bufp, struct sockaddr *sap)
{
    switch (type) 
    {
    case 1:
        return (INET6_getsock(bufp, sap));
    default:
        return (INET6_resolve(bufp, (struct sockaddr_in6 *) sap));
    }
}

static int ipv6_route_set(int action, char *ipv6_gateway, char* devname)
{
    int rv = 0;
    struct in6_rtmsg rt;          /* ipv6 route struct */
    struct ifreq ifr;             /* interface request struct */
    struct sockaddr_in6 sa6;      /* ipv6 socket address */
    int prefix_len=128;           /* network prefix length */
    char *cp = NULL;
    int skfd = -1;
    prefix_len = 0;
    memset(&sa6, 0, sizeof(sa6));
  
    /* Clean out the RTREQ structure. */
    memset((char *) &rt, 0, sizeof(struct in6_rtmsg));
    memcpy(&rt.rtmsg_dst, sa6.sin6_addr.s6_addr, sizeof(struct in6_addr));

    /* Fill in the other fields. */
    rt.rtmsg_flags = RTF_UP;
    if (prefix_len == 128){
        rt.rtmsg_flags |= RTF_HOST;
    }      
    rt.rtmsg_metric = 0;
    rt.rtmsg_dst_len = prefix_len;

    if (rt.rtmsg_flags & RTF_GATEWAY){
        return -1;
    }

    // Fill in gateway
    sa6.sin6_family = AF_INET6;
    sa6.sin6_port = 0;
    rv = inet_pton(AF_INET6, ipv6_gateway, &sa6.sin6_addr.s6_addr);
    if (rv != 1){
        fprintf(stderr, "gateway error\n");
        return -1;
    }

    memcpy(&rt.rtmsg_gateway, sa6.sin6_addr.s6_addr,sizeof(struct in6_addr));
    rt.rtmsg_flags |= RTF_GATEWAY;
         
    /* Create a socket to the INET6 kernel. */
    if ((skfd = socket(AF_INET6, SOCK_DGRAM, 0)) < 0){
        return -1;
    }
    memset(&ifr, 0, sizeof(ifr));
    // find ifname index
    strcpy(ifr.ifr_name, devname);
    if (ioctl(skfd, SIOGIFINDEX, &ifr) < 0){
        fprintf(stderr, "SIOGIFINDEX: %s\n",strerror(errno));
        return -1;
    }
    rt.rtmsg_ifindex = ifr.ifr_ifindex;
    
    /* Tell the kernel to accept this route. */
    // delete ipv6 route
    if (action == RTACTION_DEL){
        if ((rv = ioctl(skfd, SIOCDELRT, &rt)) < 0){
            printf("rv:%d\n",rv);
            fprintf(stderr, "SIOCSIFNETMASK: %s\n",strerror(errno));
            close(skfd);
            return -1;
        }
    // add ipv6 route
    }else if(action == RTACTION_ADD){
        if ((ioctl(skfd, SIOCADDRT, &rt)) < 0){
            fprintf(stderr, "SIOCSIFNETMASK: %s\n",strerror(errno));
            close(skfd);
            return -1;
        }
    }
    close(skfd);
    return (0);
}


int main()
{ 
    int rv = 0;
    char ip6Addr[64] = {0};
    char ethname[] = "docker0";
    char gateway[] = "fe80::76fe:48ff:fe25:5566";

    rv = ipv6_route_set(RTACTION_ADD, gateway,ethname);
    printf("rv :%d\n", rv);
    return 0;
}

