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

struct in6_ifreq
{
	struct in6_addr ifr6_addr;
	uint32_t ifr6_prefixlen;
	unsigned int ifr6_ifindex;
};



int get_ipv6_addr_prefix()
{
    struct ifaddrs *ifaddr = NULL;
	struct ifaddrs *ifa = NULL;
    struct sockaddr_in6 addr6;
    int sockfd6;
    char ip_str[128] = {0};
    char ip_mask[128] = {0};

    memset(&addr6, 0, sizeof(addr6));
    sockfd6 = socket(AF_INET6, SOCK_DGRAM, 0);

    if(getifaddrs(&ifaddr) != 0) {
        close(sockfd6);
		return -1;
	}

    ifa = ifaddr;
    while(ifa) {
        
		if(ifa->ifa_addr->sa_family == AF_INET6) {
            fprintf(stdout, "ifname: %s\n",ifa->ifa_name);

			memcpy(&addr6, ifa->ifa_addr, sizeof(struct sockaddr_in6));
			inet_ntop(AF_INET6, &addr6.sin6_addr, ip_str, sizeof(ip_str));
            fprintf(stdout, "addr: %s\n",ip_str);

            memset(&addr6, 0, sizeof(addr6));
			memcpy(&addr6, ifa->ifa_netmask, sizeof(struct sockaddr_in6));
			inet_ntop(AF_INET6, &addr6.sin6_addr, ip_mask, sizeof(ip_mask));
            fprintf(stdout, "mask: %s\n",ip_mask);
	
		}
		ifa = ifa->ifa_next;
	}

    freeifaddrs(ifaddr);
    close(sockfd6);
    
    return 0;
}

static int set_ipv6_addr_prefix(char *dev, char *ipv6_adddr, int ipv6masklen)
{
    int rv = 0;
	int sockfd6;
	struct ifreq ifr;
	struct in6_ifreq ifreq6;
    memset(&ifr, 0, sizeof(ifr));
    memset(&ifreq6, 0, sizeof(ifreq6));

    if (ipv6masklen < 0 || ipv6masklen > 128){
        fprintf(stderr, "[Error]: Invalid prefixlen\n");
        return -1;
    }

    strcpy(ifr.ifr_name,dev);

    // 找到网卡的index
    sockfd6 = socket(AF_INET6, SOCK_DGRAM, 0);
	rv = ioctl(sockfd6, SIOCGIFINDEX, &ifr);
	if (rv != 0){
        fprintf(stderr, "[Error]:SIOCGIFINDEX: %s\n",strerror(errno));
        close(sockfd6);
        return rv;
    }

    ifreq6.ifr6_ifindex = ifr.ifr_ifindex;

    ifreq6.ifr6_prefixlen = ipv6masklen;
	inet_pton(AF_INET6, ipv6_adddr , &ifreq6.ifr6_addr);
	rv = ioctl(sockfd6, SIOCSIFADDR, &ifreq6);
}


int main()
{ 
    int rv = 0;
    char ip6Addr[64] = "fe80::20c:29ff:fe54:9999";
    char ethname[] = "lo";
    char gateway[] = "fe80::76fe:48ff:fe25:7777";

    char ip[128] = {0};
    char mask[128] = {0};

    // set_ipv6_addr_prefix(ethname, ip6Addr, 128);
    get_ipv6_addr_prefix();
    
    return 0;
}

