#include "common.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "st.h"

#define REQUEST_TIMEOUT 30

static void* handle_request(void* arg)
{
    ssize_t len = 0;
    st_netfd_t clientFd = (st_netfd_t)arg;
    char req[512] = {0};
    char resp[] = "hello\r\n";

    len = st_read(clientFd, req, sizeof(req), SEC2USEC(REQUEST_TIMEOUT));
    std::cout << "req: " << req << std::endl;
    if(len > 0)
    {
        st_write(clientFd, resp, sizeof(resp), ST_UTIME_NO_TIMEOUT);
    }

    return NULL;
}

int main(int argc, char *argv[])
{
    int ret = 0;
    int sock = 0;
    st_netfd_t serverFd, clientFd;
    struct sockaddr_in localAddr, remoteAddr;

    if(st_init() < 0) {
        return -1;
    }

    sock = socket(AF_INET, SOCK_STREAM, 0);
    assert(sock > 0);

    memset(&localAddr, 0, sizeof(localAddr));
    localAddr.sin_family = AF_INET;
	localAddr.sin_addr.s_addr = inet_addr("0.0.0.0");
	localAddr.sin_port = htons(1234);

    ret = bind(sock, (struct sockaddr *)&localAddr, sizeof(localAddr));
    assert(ret == 0);

    listen(sock, 256);

    serverFd = st_netfd_open_socket(sock);
    assert(serverFd);

    int addrlen = sizeof(remoteAddr);
    while (true)
    {
        clientFd = st_accept(serverFd, (struct sockaddr *)&remoteAddr, &addrlen, ST_UTIME_NO_TIMEOUT);
        assert(clientFd);

        if(st_thread_create(handle_request, clientFd, 0, 0) == NULL) {
            assert(0);
        }
    }

    return 0;
}