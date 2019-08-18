//
// Created by Chane William on 2019-08-16.
//

#ifndef RAILWAY_OTHER_METHOD_H
#define RAILWAY_OTHER_METHOD_H

#endif //RAILWAY_OTHER_METHOD_H

struct NowDate
{
    char tmp0[16]; //年月日
};

//日期差-天数
int day_diff(char nowdate[20],char urdate[20])
{
    char cmonth_start[20] , cday_start[20], cmonth_end[20], cday_end[20];
    int month_start , day_start, month_end, day_end;

    strncpy(cmonth_start,nowdate,2);//前两位
    strncpy(cday_start,nowdate+2,2);//后两位
    strncpy(cmonth_end,urdate,2);//前两位
    strncpy(cday_end,urdate+2,2);//后两位
    month_start = atoi(cmonth_start);
    month_end = atoi(cmonth_end);
    day_start = atoi(cday_start);
    day_end = atoi(cday_end);

    int m2, d2;
    int m1, d1;

    m1 = (month_start + 9) % 12;
    d1 = (m1*306 + 5)/10 + (day_start - 1);
    m2 = (month_end + 9) % 12;
    d2 = (m2*306 + 5)/10 + (day_end - 1);

    return (d2 - d1);
}
void deleteLine(char* filepath, int n)
{
    char buf[4096];    //每读取一行，都将内容放到该数组中
    FILE* fp = fopen(filepath, "r");     //filepath里是原内容
    FILE* fpt = fopen("passengerstemp.txt", "w");    //将filepath文件第n行内容删除后存入temp.txt中
    int i = 0;
    while(!feof(fp))
    {
        i++;
        if (i == n)
        {
            fgets(buf,sizeof(buf), fp);   //移动文件指针
        }
        else
        {
            fgets(buf, sizeof(buf), fp);
            fprintf(fpt, "%s", buf);
        }
    }
    fclose(fp);
    fclose(fpt);
    fpt = fopen("passengerstemp.txt", "r");
    //清空filepath文件
    fp = fopen(filepath, "wb");
    fclose(fp);
    //
    fp = fopen(filepath, "a");
    while(!feof(fpt))
    {
        fgets(buf, sizeof(buf), fpt);
        fprintf(fp, "%s", buf);
    }

    fclose(fp);
    fclose(fpt);

    remove("passengerstemp.txt");
//    system("DEL passengerstemp.txt");   //删除文件passengerstemp.txt,使用该命令需要包含<stdlib.h>文件
}


//获取当前时间+overday的月日
NowDate getTime(int overDay)
{
    time_t timep;
    time (&timep);

    timep += 24 * 3600 * overDay;
    NowDate date;

    strftime(date.tmp0, sizeof(date.tmp0), "%m%d",localtime(&timep));

    return date;
}
//判断一个字符串是否都是数字
int isAlldigit(char a[])
{
    if (strspn(a, "0123456789") == strlen(a))
        return 1;
    else
        return 0;
}
