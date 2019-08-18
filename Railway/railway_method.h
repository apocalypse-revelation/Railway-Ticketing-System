//
// Created by Chane William on 2019-08-15.
//
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <iostream>
#include <time.h>
#include <sys/timeb.h>
#include "other_method.h"
#ifndef RAILWAY_RAILWAY_METHOD_H
#define RAILWAY_RAILWAY_METHOD_H

#endif //RAILWAY_RAILWAY_METHOD_H

//struct date { //日订票
//    int amount; //订票数量
//    char date[20];//订票日期
//    struct node *next;
//};

//车次信息
struct trainInfo{
    char trainNo[10];//车次序号
    char start[11];//始发站
    char terminus[11];//终点站
    int total;//总票
    int stationsNum;//该车次共有几站
    struct stationInfo *SI;//途经站
//    struct date *dt;
};
//车次信息中间值缓存
struct train{
    char trainNo[10];//车次序号
    char start[11];//始发站
    char terminus[11];//终点站
    int total;//余票
    int stationsNum;//该车次共有几站
};

struct stationInfo{ //站点信息链表结构体
    int num;//站序号
    char name[11];//站名
    char arrivetime[10];//到达时间
    char leavetime[10];//离开时间
    char interval[10];//停站时间
    double price;//此站到下一站的价钱
    struct stationInfo *next;//指向下一站
};

struct passengersInfo{
    char name[10];//乘客姓名
    char date[10];//乘车日期
    char trainNo[10];//乘车的车次编号
    char id[20];//乘客身份证号
    char startstation[20];//乘车站
    char terminalstation[20];//离车站
//    struct passengersInfo *next;//下一个乘客信息
};

//乘客信息中间值缓存
//struct passengers{
//    char name[10];//乘客姓名
//    char date[10];//乘车日期
//    char trainNo[10];//乘车的车次编号
//    char id[20];//乘客身份证号
//    char startstation[20];//乘车站
//    char terminalstation[20];//离车站
//};

void order();

void query();

void refund();

void deleteLine(int i);


void reservation();

//菜单
void menu()
{
    int enter = 1;
    int servicechoice;

    do
    {
        enter = 0;
        printf("\n");
        printf("\n");
        printf("************************************\n");
        printf("****欢迎使用铁路客运售票模拟系统********\n");
        printf("************************************\n");
        printf("\n");
        printf("——————————————功能表—————————————————————\n");
        printf("|——1.查询———2.订票———3.退票———other.退出——|\n");
        printf("————————————————————————————————————————\n");
        printf("请输入服务序号:");
        scanf("%d",&servicechoice);
    }while(enter == 1);
    if(servicechoice == 1)
    {
        query();
    }
    else if(servicechoice == 2)
    {
        printf("\n↬正在开始订票⟲⟲⟲⟲⟲⟲⟲⟲\n\n");
        order();
    }
    else if(servicechoice == 3)
    {
        printf("\n↬正在开始退票⟲⟲⟲⟲⟲⟲⟲⟲\n\n");
        refund();
    }
}

//订票
void order()
{
    char pname[10], pid[20], ptrainno[10], startstation[20], terminalstation[20],orderdate[10],sysdate[16],urdate[16],st[10],tn[10];;
    FILE *fp, *drivingrecords;
    struct train a;
    struct trainInfo A[10];
    struct stationInfo *front,*rear,*p,*p1;
    struct passengersInfo pp[9999];//姑且假设有9999个用户
    int Cnt=0, pgcnt = 0, soldticket = 0, orderticketamount, urstartstationnumber, urterminalstationnum;
    double startprice, terminalprice, sumprice;
    //取当前系统时间再过7天-月日
    NowDate time8 = getTime(8);
    NowDate timenow = getTime(0);

    strcpy(sysdate,timenow.tmp0);//取0823的字符串赋值给sysdate变量
    strcpy(orderdate,time8.tmp0);//取0823的字符串赋值给sysdate变量
    //读取文件初始化车次信息
    fp = fopen("train.txt","r");
    if (fp== NULL)
    {
        printf("File Open Error(train.txt)\n");
        exit(0);
    }
    while(!feof(fp))
    {
        //先取总车次信息
        fscanf(fp,"%s %s %s %d %d",a.trainNo,a.start,a.terminus,&a.total,&a.stationsNum);
        strcpy(A[Cnt].trainNo ,a.trainNo);
        strcpy(A[Cnt].start,a.start);
        strcpy(A[Cnt].terminus,a.terminus);
        A[Cnt].total=a.total;
        A[Cnt].stationsNum = a.stationsNum;
        A[Cnt].SI = NULL;
        front=rear = NULL;
        //再取该车次的各个站点信息
        for(int i=0;i<a.stationsNum ;i++)
        {
            p = (struct stationInfo *)malloc(sizeof(struct stationInfo));
            fscanf(fp,"%d %s %s %s %s %lf",&p->num,p->name,st,tn,p->interval,&p->price);
            strcpy(p->arrivetime,st);
            strcpy(p->leavetime,tn);
            if (front ==NULL)
                front = p;
            else
                rear->next = p;
            rear = p;
        }
        rear->next = NULL;
        A[Cnt].SI = front;
        Cnt++;
    }
    fclose(fp);
    //取乘车记录
    drivingrecords = fopen("passengers.txt","r");
    if(drivingrecords == NULL)
    {
        printf("File Open Error(passengers.txt)\n");
        exit(0);
    }
    while(!feof(drivingrecords))
    {
        char date[10];
        char trainNo[10];
        char name[10];
        char startstat[20];
        char terminalstat[20];
        char id[20];
        fscanf(drivingrecords, "%s %s %s %s %s %s", date, trainNo, startstat, terminalstat, name, id);
        strcpy(pp[pgcnt].date,date);
        strcpy(pp[pgcnt].trainNo,trainNo);
        strcpy(pp[pgcnt].startstation,startstat);
        strcpy(pp[pgcnt].terminalstation, terminalstat);
        strcpy(pp[pgcnt].name,name);
        strcpy(pp[pgcnt].id,id);
        pgcnt++;
    }
    fclose(drivingrecords);
    printf("↬现在是北京时间--%s（提示:任何过程都可以输入0退至菜单）\n请输入出发日期（如0818）:",sysdate);
    scanf("%s", urdate);
    if(strcmp("0",urdate) == 0)
        menu();
    if(isAlldigit(urdate) == 0)
    {
        printf("!!!!!!!!!!存在非法字符！应都是数字\n");
        order();
    }
    else if(strlen(urdate) != 4)
    {
        printf("!!!!!!!!!!输入的数字日期共为4个！\n");
        order();
    }
    else if(strcmp(urdate,sysdate) < 0)
    {
        printf("!!!!!!!!!!不能选今天之前的日期！\n");
        order();
    }
    if(day_diff(sysdate,urdate) > 7)
    {
        printf("!!!!!!!!!!不能买7天后的票！请重新输入！\n");
        order();
    }
    printf("请输入车次号:");
    scanf("%s",ptrainno);
    if(strcmp("0",ptrainno) == 0)
        menu();
    for(int i = 0;i <= Cnt;i++)
    {
        if(i == Cnt)
        {
            printf("找不到该车次号,请重新输入:");
            scanf("%s",ptrainno);
            i = -1;
            continue;
        }
        if(strcmp(ptrainno,A[i].trainNo) != 0)
            continue;
        p = A[i].SI;
        p1 =  A[i].SI;
        //显示余票
        for(int i = 0; i < pgcnt; i++)
        {
//            printf("%s\n",surplusticket);
            if((strcmp(urdate,pp[i].date) == 0)&&(strcmp(ptrainno,pp[i].trainNo) == 0))
            {
                soldticket++;
            }
        }
        //购票
        int surplusticket = A[i].total-soldticket;
        char charticketamount[20],reserve[20];
        printf("↬温馨提示:该时间的车次还有余票为:%d\n", surplusticket);
        printf("请输入购买的票数:");
        scanf("%s", charticketamount);
        while(1)
        {
            if(strcmp("0", charticketamount) == 0)
                menu();
            if(isAlldigit(charticketamount) == 0)
            {
                printf("!!!!!!!!!!不能输入非数字,请重新输入:");
                scanf("%s", charticketamount);
                continue;
            }
            if(atoi(charticketamount) > surplusticket) //如果购买的票数大于余票
            {
                printf("您购买的票数不能超过余票!您还可以选择抢票预约（输入1预约，输入2重新输入票数）:");
                scanf("%s", reserve);
                if(strcmp(reserve,"1") == 0)
                {
                    //将乘客信息写入 passengers.txt中
                    FILE * fwritetorv;
                    char rname[20],rtrainno[20], rdate[20], rid[20];
                    fwritetorv = fopen ("reservation.txt", "a+");
                    printf("请输入您的姓名:");
                    scanf("%s",rname);
                    while(1)
                    {
                        if(strcmp("0",rname) == 0)
                            menu();
                        if(strcmp("",rname) == 0)
                        {
                            printf("您的姓名不能为空，请重新输入:");
                            scanf("%s",rname);
                            continue;
                        }
                        break;
                    }

                    printf("请输入您的身份证号:");
                    scanf("%s",rid);
                    while(1)
                    {
                        if(strcmp("0",rid) == 0)
                            menu();
                        if(isAlldigit(rid) == 0)
                        {
                            printf("!!!!!!!!!!存在非法字符！应都是数字,请重新输入:");
                            scanf("%s",rid);
                            continue;
                        }
                        if(strlen(rid) != 18)
                        {
                            printf("!!!!!!!!!!身份证应都为18位,请重新输入:");
                            scanf("%s",rid);
                            continue;
                        }
                        break;
                    }
                    fprintf(fwritetorv, "\n%s %s %s %d %s", ptrainno, urdate, rname, atoi(charticketamount), rid);
                    fclose(fwritetorv);
                    printf("预约成功！\n");
                    menu();
                }
                else if(strcmp(reserve,"2") == 0)
                {
                    printf("请重新输入您的票数:");
                    scanf("%s", charticketamount);
                    continue;
                }
                else
                {
                    menu();
                }

            }
            orderticketamount = atoi(charticketamount);//这里防止控制台输入字符串造成混乱
            break;
        }
        printf("请输入出发站:");
        scanf("%s",startstation);
        while (1)
        {
            if(strcmp("0",startstation) == 0)
                menu();
            if(!p)
            {
                printf("该车次没有该出发站,请重新选择:");
                scanf("%s",startstation);
                p = p1;
                continue;
            }
            if(strcmp(startstation,p->name) != 0)
            {
                p = p->next;
                continue;
            }
            else
            {
                urstartstationnumber = p->num;//始发站序号
                if(urstartstationnumber == A[i].stationsNum)
                {
                    printf("您的出发站不能选终点站!请重新输入:");
                    scanf("%s",startstation);
                    p = p1;
                    continue;
                }
                startprice = p->price; //钱
                break;
            }
        }
        //乘客到达站
        p = p1;
        printf("请输入到达站:");
        scanf("%s",terminalstation);
        while (1)
        {
            if(strcmp("0",terminalstation) == 0)
                menu();
            if(!p)
            {
                printf("该车次找不到该到达站,请重新选择:");
                scanf("%s",terminalstation);
                p = p1;
                continue;
            }
            if(strcmp(terminalstation,p->name)!=0)
            {
                p = p->next;
                continue;
            }
            else
            {
                urterminalstationnum = p->num;
                if(urterminalstationnum <= urstartstationnumber)
                {
                    printf("您的到达站不能选在出发站或出发站之前!请重新输入:");
                    scanf("%s",terminalstation);
                    p = p1;
                    continue;
                }
                terminalprice = p->price; //钱
                break;
            }
        }
        //乘客姓名
        printf("请输入您的姓名:");
        scanf("%s",pname);
        while(1)
        {
            if(strcmp("0",pname) == 0)
                menu();
            if(strcmp("",pname) == 0)
            {
                printf("您的姓名不能为空，请重新输入:");
                scanf("%s",pname);
                continue;
            }
            break;
        }
        //乘客身份证
        printf("请输入您的身份证号:");
        scanf("%s",pid);
        while(1)
        {
            if(strcmp("0",pid) == 0)
                menu();
            if(isAlldigit(pid) == 0)
            {
                printf("!!!!!!!!!!存在非法字符！应都是数字,请重新输入:");
                scanf("%s",pid);
                continue;
            }
            if(strlen(pid) != 18)
            {
                printf("!!!!!!!!!!身份证应都为18位,请重新输入:");
                scanf("%s",pid);
                continue;
            }
            break;
        }
        //将乘客信息写入 passengers.txt中
        FILE * fwritetopassengers;
        fwritetopassengers = fopen ("passengers.txt", "a+");
        for(int i = 0;i < orderticketamount; i++) //上限为乘客买的票数
            fprintf(fwritetopassengers, "\n%s %s %s %s %s %s", urdate, ptrainno, startstation, terminalstation, pname, pid);
        fclose(fwritetopassengers);
        //总金额
        sumprice = (terminalprice - startprice) * orderticketamount;
        printf("❤❤❤❤❤❤❤❤❤❤❤❤❤❤❤❤❤❤❤❤❤❤❤❤❤订票成功!所需金额:%.2lf❤❤❤❤❤❤❤❤❤❤❤❤❤❤❤❤❤❤❤❤❤❤❤❤❤\n", sumprice);//写入txt后才算成功
        printf("————————————————————————————————————订票结束————————————————————————————————————\n");
        menu();
    }
}

//查询
void query()
{
    char pname[10], pid[20], ptrainno[10], startstation[20], terminalstation[20],orderdate[10],sysdate[16],urdate[16],st[10],tn[10];;
    FILE *fp, *drivingrecords;
    struct train a;
    struct trainInfo A[10];
    struct stationInfo *front,*rear,*p,*p1;
    struct passengersInfo pp[9999];//姑且假设有9999个用户
    int Cnt=0, pgcnt = 0, soldticket = 0, orderticketamount, isexist[9999], existnum = 0, urstartstationnumber, urterminalstationnum;
    double startprice, terminalprice, sumprice;
    //取当前系统时间再过7天-月日
    NowDate time8 = getTime(8);
    NowDate timenow = getTime(0);

    strcpy(sysdate,timenow.tmp0);//取0823的字符串赋值给sysdate变量
    strcpy(orderdate,time8.tmp0);//取0823的字符串赋值给sysdate变量
    //读取文件初始化车次信息
    fp = fopen("train.txt","r");
    if (fp== NULL)
    {
        printf("File Open Error(train.txt)\n");
        exit(0);
    }
    while(!feof(fp))
    {
        //先取总车次信息
        fscanf(fp,"%s %s %s %d %d",a.trainNo,a.start,a.terminus,&a.total,&a.stationsNum);
        strcpy(A[Cnt].trainNo ,a.trainNo);
        strcpy(A[Cnt].start,a.start);
        strcpy(A[Cnt].terminus,a.terminus);
        A[Cnt].total=a.total;
        A[Cnt].stationsNum = a.stationsNum;
        A[Cnt].SI = NULL;
        front=rear = NULL;
        //再取该车次的各个站点信息
        for(int i=0;i<a.stationsNum ;i++)
        {
            p = (struct stationInfo *)malloc(sizeof(struct stationInfo));
            fscanf(fp,"%d %s %s %s %s %lf",&p->num,p->name,st,tn,p->interval,&p->price);
            strcpy(p->arrivetime,st);
            strcpy(p->leavetime,tn);
            if (front ==NULL)
                front = p;
            else
                rear->next = p;
            rear = p;
        }
        rear->next = NULL;
        A[Cnt].SI = front;
        Cnt++;
    }
    fclose(fp);
    //从第一列车次开始检索（与订票相反，查询先输入出发/到达站）
    printf("请输入出发站:");
    scanf("%s",startstation);
    if(strcmp("0",startstation) == 0)
        menu();
    printf("请输入到达站:");
    scanf("%s",terminalstation);
    if(strcmp("0",terminalstation) == 0)
        menu();
    for(int i = 0;i <= Cnt;i++)
    {
        int havestartstation = 1, haveterminalstation = 1, isError = 0;
        if((i == Cnt) && (existnum > 0))
            break;
        if(i == Cnt)
        {
            printf("所有列车都匹配不到您的选择,请重新开始!\n");
            query();
        }
        p = A[i].SI;
        p1 =  A[i].SI;
        while (1)
        {
            if(strcmp("0",startstation) == 0)
                menu();
            if(!p)
            {
                havestartstation = 0;
                break;
            }
            if(strcmp(startstation,p->name) != 0)
            {
                p = p->next;
                continue;
            }
            else
            {
                urstartstationnumber = p->num;//始发站序号
//                printf("%d-", urstartstationnumber);
//                if(urstartstationnumber == A[i].stationsNum)
//                {
//                    printf("您的出发站不能选终点站!请重新输入:");
//                    scanf("%s",startstation);
//                    p = p1;
//                    continue;
//                }
                startprice = p->price; //钱
                break;
            }
        }
        if(havestartstation == 0)
            continue;
        //乘客到达站
        p = p1;
        while (1)
        {
            if(strcmp("0",terminalstation) == 0)
                menu();
            if(!p)
            {
                haveterminalstation = 0;
                break;
            }
            if(strcmp(terminalstation,p->name)!=0)
            {
                p = p->next;
                continue;
            }
            else
            {
                urterminalstationnum = p->num;
//                printf("%d-", urterminalstationnum);
                if(urterminalstationnum <= urstartstationnumber)
                {
                    i = Cnt - 1;
                    existnum = 0;
                    isError = 1; //出发站序号大于到达站
                    break;
                }
                terminalprice = p->price; //钱
                sumprice = terminalprice - startprice;
                break;
            }
        }
        if(isError == 1)
            continue;
        if(haveterminalstation == 0)
            continue;
        isexist[existnum++] = i;
    }
    printf("↬现在是北京时间--%s（提示:任何过程都可以输入0退至菜单）\n请输入出发日期（如0818）:",sysdate);
    scanf("%s", urdate);
    //检索完满足(出发/到达站)的一列车次，开始选择日期
    while(1)
    {
        if(strcmp("0",urdate) == 0)
            menu();
        if(isAlldigit(urdate) == 0)
        {
            printf("!!!!!!!!!!存在非法字符！应都是数字！请重新输入\n");
            scanf("%s", urdate);
            continue;
        }
        else if(strlen(urdate) != 4)
        {
            printf("!!!!!!!!!!输入的数字日期共为4个！请重新输入\n");
            scanf("%s", urdate);
            continue;
        }
        else if(strcmp(urdate,sysdate) < 0)
        {
            printf("!!!!!!!!!!不能查看今天之前的车票！请重新输入\n");
            scanf("%s", urdate);
            continue;
        }
        else if(day_diff(sysdate,urdate) > 7)
        {
            printf("!!!!!!!!!!无法查看7天后的车票！请重新输入！\n");
            scanf("%s", urdate);
            continue;
        }
        break;
    }
    //打印
    for(int i = 0; i < existnum; i++)
    {
        pgcnt = 0;
        //取乘车记录
        drivingrecords = fopen("passengers.txt","r");
        if(drivingrecords == NULL)
        {
            printf("File Open Error(passengers.txt)\n");
            exit(0);
        }
        while(!feof(drivingrecords))
        {
            char date[10];
            char trainNo[10];
            char name[10];
            char startstat[20];
            char terminalstat[20];
            char id[20];
            fscanf(drivingrecords, "%s %s %s %s %s %s", date, trainNo, startstat, terminalstat, name, id);
            strcpy(pp[pgcnt].date,date);
            strcpy(pp[pgcnt].trainNo,trainNo);
            strcpy(pp[pgcnt].startstation,startstat);
            strcpy(pp[pgcnt].terminalstation, terminalstat);
            strcpy(pp[pgcnt].name,name);
            strcpy(pp[pgcnt].id,id);
            pgcnt++;
        }
        fclose(drivingrecords);
        printf("(%d)\n",i+1);
        printf("车次:【%s】,始发站:【%s】,终点站:【%s】,初始票数:【%d】,途径站数:【%d】\n",A[isexist[i]].trainNo,A[isexist[i]].start,A[isexist[i]].terminus,A[isexist[i]].total,A[isexist[i]].stationsNum);
        p = A[isexist[i]].SI;
        while (p)
        {
            printf("%02d\t%s\t%s\t%s\t%s\t%.2lf\n",p->num,p->name,p->arrivetime,p->leavetime,p->interval,p->price);
            p = p->next;
        }
        printf("——————————————————————————————————————————————————————————————————————————————\n");
        //显示余票
        soldticket = 0;
        for(int j = 0; j < pgcnt; j++)
        {
            if((strcmp(urdate,pp[j].date) == 0)&&(strcmp(A[isexist[i]].trainNo,pp[j].trainNo) == 0))
                soldticket++;
        }
        int surplusticket = A[isexist[i]].total-soldticket;
        printf("❤❤❤❤❤❤❤❤❤❤❤❤❤❤❤❤❤❤❤❤❤❤❤❤❤该时段车次余票:%d❤❤❤❤❤❤❤❤❤❤❤❤❤❤❤❤❤❤❤❤❤❤❤❤❤\n", surplusticket);
        //总金额
//        sumprice = (terminalprice - startprice) * orderticketamount;
        printf("❤❤❤❤❤❤❤❤❤❤❤❤❤❤❤❤❤❤❤❤❤❤❤❤❤所需金额:%.2lf❤❤❤❤❤❤❤❤❤❤❤❤❤❤❤❤❤❤❤❤❤❤❤❤❤❤❤\n", sumprice);
        printf("——————————————————————————————————————————————————————————————————————————————\n");

    }
    printf("\n\n————————————————————————————————————查询结束————————————————————————————————————\n");
    menu();
}

//退票
void refund()
{
    char pname[10], pid[20], ptrainno[10], startstation[20], terminalstation[20],orderdate[10],sysdate[16],urdate[16],st[10],tn[10];;
    FILE *fp, *drivingrecords;
    struct train a;
    struct trainInfo A[10];
    struct stationInfo *front,*rear,*p,*p1;
    struct passengersInfo pp[9999];//姑且假设有9999个用户
    int Cnt=0, pgcnt = 0, soldticket = 0, orderticketamount, urstartstationnumber, urterminalstationnum;
    double startprice, terminalprice, sumprice;
    //取当前系统时间再过7天-月日
    NowDate time8 = getTime(8);
    NowDate timenow = getTime(0);

    strcpy(sysdate,timenow.tmp0);//取0823的字符串赋值给sysdate变量
    strcpy(orderdate,time8.tmp0);//取0823的字符串赋值给sysdate变量
    //读取文件初始化车次信息
    fp = fopen("train.txt","r");
    if (fp== NULL)
    {
        printf("File Open Error(train.txt)\n");
        exit(0);
    }
    while(!feof(fp))
    {
        //先取总车次信息
        fscanf(fp,"%s %s %s %d %d",a.trainNo,a.start,a.terminus,&a.total,&a.stationsNum);
        strcpy(A[Cnt].trainNo ,a.trainNo);
        strcpy(A[Cnt].start,a.start);
        strcpy(A[Cnt].terminus,a.terminus);
        A[Cnt].total=a.total;
        A[Cnt].stationsNum = a.stationsNum;
        A[Cnt].SI = NULL;
        front=rear = NULL;
        //再取该车次的各个站点信息
        for(int i=0;i<a.stationsNum ;i++)
        {
            p = (struct stationInfo *)malloc(sizeof(struct stationInfo));
            fscanf(fp,"%d %s %s %s %s %lf",&p->num,p->name,st,tn,p->interval,&p->price);
            strcpy(p->arrivetime,st);
            strcpy(p->leavetime,tn);
            if (front ==NULL)
                front = p;
            else
                rear->next = p;
            rear = p;
        }
        rear->next = NULL;
        A[Cnt].SI = front;
        Cnt++;
    }
    fclose(fp);
    //取乘车记录
    drivingrecords = fopen("passengers.txt","r");
    if(drivingrecords == NULL)
    {
        printf("File Open Error(passengers.txt)\n");
        exit(0);
    }
    while(!feof(drivingrecords))
    {
        char date[10];
        char trainNo[10];
        char name[10];
        char startstat[20];
        char terminalstat[20];
        char id[20];
        fscanf(drivingrecords, "%s %s %s %s %s %s", date, trainNo, startstat, terminalstat, name, id);
        strcpy(pp[pgcnt].date,date);
        strcpy(pp[pgcnt].trainNo,trainNo);
        strcpy(pp[pgcnt].startstation,startstat);
        strcpy(pp[pgcnt].terminalstation, terminalstat);
        strcpy(pp[pgcnt].name,name);
        strcpy(pp[pgcnt].id,id);
        pgcnt++;
    }
    fclose(drivingrecords);
    printf("请输入您的身份证号:");
    scanf("%s",pid);
    while(1)
    {
        if(strcmp("0",pid) == 0)
            menu();
        if(isAlldigit(pid) == 0)
        {
            printf("!!!!!!!!!!存在非法字符！应都是数字,请重新输入:");
            scanf("%s",pid);
            continue;
        }
        if(strlen(pid) != 18)
        {
            printf("!!!!!!!!!!身份证应都为18位,请重新输入:");
            scanf("%s",pid);
            continue;
        }
        break;
    }
    int urticketamount = 0;
    for(int j = 0; j < pgcnt; j++)
        if(strcmp(pid, pp[j].id) == 0)
            urticketamount++;
    if(urticketamount == 0)
    {
        printf("您没有订票！请检查身份证输入是否正确！系统将重新开始......");
        refund();
    }
    printf("您一共订了%d张票以下是您的订票信息\n日期 车次 出发站 到达站 姓名 身份证号\n",urticketamount);
    for(int j = 0; j < pgcnt; j++)
    {
        if(strcmp(pid, pp[j].id) == 0)
            printf("%s %s %s %s %s %s \n", pp[j].date, pp[j].trainNo, pp[j].startstation, pp[j].terminalstation, pp[j].name, pp[j].id);
    }

    int urrefundchoice = 0;
    printf("\n选择退票日期（如0818）:",sysdate);
    scanf("%s", urdate);
    while (1)
    {
        if(strcmp("0",urdate) == 0)
            menu();
        if(isAlldigit(urdate) == 0)
        {
            printf("!!!!!!!!!!存在非法字符！应都是数字请重新输入:");
            scanf("%s", urdate);
            continue;
        }
        else if(strlen(urdate) != 4)
        {
            printf("!!!!!!!!!!输入的数字日期共为4个！请重新输入:");
            scanf("%s", urdate);
            continue;
        }
        if(day_diff(sysdate,urdate) > 7)
        {
            printf("!!!!!!!!!!不能买7天后的票！请重新输入:");
            scanf("%s", urdate);
            continue;
        }
        for(int j = 0; j < pgcnt; j++)
        {
            if((strcmp(urdate, pp[j].date) == 0) && (strcmp(pid, pp[j].id) == 0))
                urrefundchoice++;
        }
        if(urrefundchoice == 0)
        {
            printf("日期选择有误，找不到该车票记录！请重新输入:");
            scanf("%s", urdate);
            continue;
        }
        break;
    }
    urrefundchoice = 0;
    int deleteline[9999], j = 0, k = 0;
    printf("请选择退票车次号:");
    scanf("%s",ptrainno);
    while (1)
    {
        if(strcmp("0",ptrainno) == 0)
            menu();
        for(j = 0; j < pgcnt; j++)
        {
            if((strcmp(ptrainno, pp[j].trainNo) == 0) && (strcmp(urdate, pp[j].date) == 0) && (strcmp(pid, pp[j].id) == 0))
            {
                urrefundchoice++;
                deleteline[k++] = j+1;
            }
        }
        if(urrefundchoice == 0)
        {
            printf("车次选择有误，找不到该车票记录！请重新选择:");
            scanf("%s",ptrainno);
            continue;
        }
        break;
    }

    urrefundchoice = 0;
    printf("\n以下是您的票据......\n");
    for(int j = 0; j < pgcnt; j++)
    {
        if((strcmp(ptrainno, pp[j].trainNo) == 0) && (strcmp(urdate, pp[j].date) == 0) && (strcmp(pid, pp[j].id) == 0))
        {
            printf("%s %s %s %s %s %s \n", pp[j].date, pp[j].trainNo, pp[j].startstation, pp[j].terminalstation, pp[j].name, pp[j].id);
            urrefundchoice++;
        }
    }
    printf("您最多能退%d张票，请选择退票数量:",urrefundchoice);
    int urrealrefundticketamount = 1;
    scanf("%d", &urrealrefundticketamount);
    while(1)
    {
        if(urrealrefundticketamount == 0)
            menu();
        if(urrealrefundticketamount > urrefundchoice)
        {
            printf("您最多能退%d张票，请重新选择退票数量:",urrefundchoice);
            scanf("%d", &urrealrefundticketamount);
            continue;
        }
        break;
    }
    if(urrealrefundticketamount == 1)
    {
        deleteLine("passengers.txt",deleteline[0]);
    }
    else if(urrealrefundticketamount > 1)
    {
        for(int i = 0; i < k; i++)
        {
            deleteLine("passengers.txt",deleteline[i]-i);
        }
    }
        printf("❤❤❤❤❤❤❤❤❤❤❤❤❤❤❤❤❤❤❤❤❤❤❤❤❤退票成功!❤❤❤❤❤❤❤❤❤❤❤❤❤❤❤❤❤❤❤❤❤❤❤❤❤\n");
        printf("————————————————————————————————————结束————————————————————————————————————\n");
        menu();
}

void reservation()
{

}


