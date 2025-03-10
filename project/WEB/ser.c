/*************************************************************************
	> File Name: ser.c
	> Author: yas
	> Mail: rage_yas@hotmail.com
	> Created Time: 2024年08月05日 星期一 09时07分35秒
 ************************************************************************/

#include"head.h"
#include"sqlite.h"
#include "logic.h"

char username[32] = {0};
char password[32] = {0};
char goodsname[32] = {0};
char cli_ip[128] = {0};

char *pgood_head = "<!DOCTYPE html><html><head><meta charset=\"utf-8\"><title>MY HTML</title></head><body><a href=\"find.html\"><img border=\"0\" src=\"./source/images/200905/thumb_img/shouye.png\" alt=\"HTML 教程\" width=\"200\" height=\"200\"></a><style>body{background-image:url('1.gif');background-size:cover;}</style><center>  <h1>查询结果</h1><br><br><br><form action=\"find.html\" methon=\"POST\" align=center>";

char *detail_head = "<!DOCTYPE html><html><head><meta charset=\"utf-8\"><title>MY HTML</title></head><body><a href=\"goods_name.html\"><img border=\"0\" src=\"./source/images/200905/thumb_img/shouye.png\" alt=\"HTML 教程\" width=\"200\" height=\"200\"></a><style>body{background-image:url('1.gif');background-size:cover;}</style><center>  <h1>详细信息</h1><br><br><br><form action=\"find.html\" methon=\"POST\" align=center>";

char *pfail = "</form></center></body></html>";

unsigned char FromHex(unsigned char x)
{
	if(0 == x ) return -1;
	unsigned char y;
	if(x>='A' &&x<='Z') y = x-'A'+10;
	else if(x>='a' &&x <='z') y = x-'a'+10;
	else if(x>='0' && x<='9') y = x-'0';

	return y;
}

int  urlDecode(  char* dest, const char* src)
{
	if(NULL ==src || NULL == dest)
	{
		return -1;

	}
	int len = strlen(src);
	int i =0 ;
	for(i = 0 ;i<len;i++)
	{

		if('+' == src[i]) strcat(dest,"");
		else if('%'==src[i])
		{
			//if(i+2len)return -1;
			unsigned char high = FromHex((unsigned char)src[++i]);
			unsigned char low = FromHex((unsigned char)src[++i]);
			unsigned char temp = high*16 +low;
			char temp2[5]={0};
			sprintf(temp2,"%c",temp);
			strcat(dest , temp2);
		}
	}
	return 0;
}

int init_tcp_ser(const char *ip, int port)
{
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if(sockfd == -1)
    {
        perror("fail socked\n");
        return -1;
    }
    
    int val = 1;
    setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &val, sizeof(val));

    struct sockaddr_in ser;

    ser.sin_family = AF_INET;
    ser.sin_port = htons(port);
    ser.sin_addr.s_addr = inet_addr(ip);

    int ret = bind(sockfd, (struct sockaddr *)&ser, sizeof(ser));
    if(ret == -1)
    {
        perror("fail bind\n");
        return -1;
    }

    listen(sockfd, 100);

    return sockfd;
}

int send_http_head(int connfd)
{
    char *phead = "HTTP/1.1 200 OK\r\n"
        "Content-Type: text/html;charset=UTF-8\r\n"
        "Connection: close\r\n"
        "Server: WEB_SERVER\r\n\r\n";

  //  printf("%s",phead);
    ssize_t size = send(connfd, phead, strlen(phead), 0);
    if(size == -1)
    {
        perror("fail send\n");
        return -1;
    }

    return 0;
}
 int respond_send_content(const char *pathname, int connfd)
{
    char buff[1024] = {0};

    printf("send filename = %s\n", pathname);
    write_logic(pathname, cli_ip);
    int fd = open(pathname, O_RDONLY);
    if(-1 == fd)
    {
        perror("fail open\n");
        return -1;
    }
    
    while(1)
    {
        ssize_t size = read(fd, buff, sizeof(buff));
        if(0 == size)
        {
            break;
        }
        
        int ret = send(connfd, buff, size, 0);
        if(-1 == ret)
        {
            perror("fail send\n");
            return -1;
        }
    }

    return 0;
}

int content_prase(HTTP_REQ_T preq)
{
    int i = 0;
    //printf("%s\n",preq.content);
    char *pUse = strstr(preq.content, "username=");
    char *pPase =  strstr(preq.content, "passname=");
    char *pgoods = strstr(preq.content, "goodsname=");

    if(pUse != NULL && pPase != NULL)
    {
        pUse += 9;
        pPase += 9;
        while(*pUse != '&')
        {
            username[i] = *pUse;
            i++;
            pUse++;
        }
        strcpy(password, pPase);

        printf("username:%s\n",username);
        printf("passname:%s\n",password);

    }
    else if(pgoods)
    {
        pgoods += strlen("goodsname=");
        if(*pgoods == '%')
        {
            urlDecode(goodsname,pgoods);
        }
        else
        {
            strcpy(goodsname, pgoods);
        }
    }

    return 0;
}


int send_respond(int connfd, HTTP_REQ_T preq)
{
    char buff[256] = {0};
    char num[12] = {0};
    int i = 0;

    write_logic(preq.url, cli_ip);
    write_logic(preq.method, cli_ip);

    send_http_head(connfd);
    
    if(strcmp("GET", preq.url) == 0)
    {
        printf("%s\n",preq.url);
        printf("%s\n",preq.method);
        printf("%s\n",preq.content);
        
        char *pid = strstr(preq.method, "/data[");
        if(strcmp("/", preq.method) == 0)
        {
            respond_send_content("./html/demo.html", connfd);
        }
        else if(strstr(preq.method, ".png") != NULL || NULL != strstr(preq.method, ".gif") || strstr(preq.method, ".jpg") != NULL)
        {
            sprintf(buff, ".%s", preq.method);
            respond_send_content(buff, connfd);
        }
        else if(strstr(preq.method, ".html") != NULL)
        {
             if(strcmp(preq.method,"/goods_name.html") == 0)
            {
                content_prase(preq);
                send(connfd, pgood_head, strlen(pgood_head), 0);
                find_goods_sqlite(connfd);
                send(connfd, pfail, strlen(pfail), 0);
            }
            else if(strcmp(preq.method,"/find.html") == 0)
            {
                sprintf(buff, "./html/%s",preq.method);
                respond_send_content(buff, connfd);
            }
            else
            {
                sprintf(buff, "./html/%s",preq.method);
                respond_send_content(buff, connfd);
            }
        }
        else if(pid != NULL)      //查找用户查看商品详细信息时其对应的编号
        {
            pid += 6;
            while(*pid != ']')
            {
                num[i] = *pid;
                pid++;
                i++;
            }
            printf("id = %s\n",num);
            send(connfd, detail_head, strlen(detail_head), 0);
            find_detailed_data(num, connfd); 
            send(connfd, pfail, strlen(pfail), 0);
        }
    }
    else if(strcmp("POST", preq.url) == 0)
    {
        if(strstr(preq.method, ".html") != NULL)
        {
            printf("preq.method: %s\n",preq.method);
            if(strcmp(preq.method,"/demo.html") == 0)
            {
                content_prase(preq);
                update_userdata(username,password);   
                memset(username, 0, sizeof(username));
                memset(password, 0, sizeof(password));
                sprintf(buff, "./html/%s",preq.method);
                respond_send_content(buff, connfd);
            }
            else if(strcmp(preq.method,"/goods_name.html") == 0)
            {
                content_prase(preq);
                send(connfd, pgood_head, strlen(pgood_head), 0);
                find_goods_sqlite(connfd);
                send(connfd, pfail, strlen(pfail), 0);
            }
            else if(strcmp(preq.method,"/find.html") == 0)  //登录页面进入时的跳转及用户密码确认界面
            {
                content_prase(preq);
                if(find_user_data(username, password) == 1)
                {
                    memset(username, 0, sizeof(username));
                    memset(password, 0, sizeof(password));
                    sprintf(buff, "./html/%s",preq.method);
                    respond_send_content(buff, connfd);   
                }
                else
                {
                    respond_send_content("./html/demo.html", connfd);
                }
            }
            else if(strcmp(preq.method, "/register.html") == 0)
            {
                sprintf(buff, "./html/%s",preq.method);
                respond_send_content(buff, connfd);
            }
            /*
            else
            {
                printf("1flag: %d\n",flag);
                sprintf(buff, "./html/%s",preq.method);
                respond_send_content(buff, connfd);
            }*/
        }
        else if(strstr(preq.method, ".png") != NULL || NULL != strstr(preq.method, ".gif") || strstr(preq.method, ".jpg") != NULL)
        {
            //sprintf(buff, ".%s", preq.method);
            respond_send_content(preq.method, connfd);
        }
    }
    return 0;
}


int request_prase(char *preq_buff, HTTP_REQ_T *preq)
{
    char buff[1024] = {0};

    strcpy(buff, preq_buff);
    char *req_url = strtok(buff, " ");
    if(req_url == NULL)
    {
        perror("fail strtok\n");
        return -1;
    }
    strcpy(preq->url, req_url);

    char *req_method = strtok(NULL, " ");
    if(req_method == NULL)
    {
        perror("fail strtok\n");
        return -1;
    }
    strcpy(preq->method, req_method);

    char *req_content = strstr(preq_buff, "\r\n\r\n");
    if(req_content != NULL)
    {
        req_content += 4;
    }
    else
    {
        perror("fail strstr\n");
        return -1;
    }
    strcpy(preq->content, req_content);

    return 0;
}

int main()
{
    char recv_buff[2048] = {0};
    HTTP_REQ_T req;

    create_table_sqlite();
    init_logic();


    int sockfd = init_tcp_ser("192.168.1,181", 8080);
    if(-1 == sockfd)
    {
        perror("fail ser_init\n");
        return -1;
    }

    struct sockaddr_in cli;
    socklen_t socklen = sizeof(cli);
    

    while(1)
    {
        create_logic();
        memset(recv_buff, 0, sizeof(recv_buff));
        int connfd = accept(sockfd, (struct sockaddr *)&cli, &socklen);
        if(-1 == connfd)
        {
            perror("fail accept\n");
            return -1;    
        }
       
        sprintf(cli_ip,"[%s]",inet_ntoa(cli.sin_addr));

        
        int pid = fork();
        if(pid > 0)
        {
        
        }
        else
        {
            ssize_t size = recv(connfd, recv_buff, sizeof(recv_buff), 0);
            if(-1 == size)
            {
                perror("fai recv\n");
                break;
            }
            if(0 == size)
            {
                continue;
            }
            //printf("%s\n",recv_buff);
    
            request_prase(recv_buff, &req);
            send_respond(connfd, req);
        }

        close(connfd);
    }
    close(sockfd);

    return 0;
}
