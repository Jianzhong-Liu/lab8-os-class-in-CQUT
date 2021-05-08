#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<stdlib.h>
#include<conio.h>

#define n 10 /*假设系统允许最大作业为 n=10*/ 
#define m 10 /*假设系统允许的空闲区表最大为 m=10*/ 
#define minisize 100 

struct 
{ 
    float address; /*已分分区的起始地址*/ 
    float length; /*已分分区的长度，单位为字节*/ 
    int flag; /*已分配区表登记栏的标志，用"0"表示空栏目*/ 
}used_table[n]; /*已分配区表*/ 

struct 
{
    float address; /*空闲区起始地址*/ 
    float length; /*空闲区长度，单位为字节*/ 
    int flag; /*空闲区表登记栏标志，用"0"表示空栏目，用"1"表示未分配*/ 
}free_table[m]; /*空闲区表*/ 

int allocate(char J,float xk)  /*采用最优分配算法分配xk大小的空间*/ 
{ 
    int i,k; 
    float ad; 
    k=-1; 
    for(i=0;i<m;i++) /*寻找空间大于xk的最小空闲区登记项k*/ 
    if(free_table[i].length>=xk&&free_table[i].flag==1) 
    if(k==-1||free_table[i].length<free_table[k].length) 
    k=i; 
    if(k==-1)/*未找到可用空闲区，返回*/ 
    { 
        printf("Cannot find the idle partition\n"); 
        return 0; 
    }       
    /*找到可用空闲区，开始分配：若空闲区大小与要求分配的空间差小于msize大小，则空闲区全部分配；若空闲区大小与要求分配的空间差大于minisize大小，则从空闲区划出一部分分配*/ 
    if(free_table[k].length-xk<=minisize) 
    { 
        free_table[k].flag=0; 
        ad=free_table[k].address; 
        xk=free_table[k].length; 
    } 
    else 
    { 
        free_table[k].length=free_table[k].length-xk; 
        ad=free_table[k].address+free_table[k].length; 
    } 
    /*修改已分配区表*/ 
    i=0; 
    while(used_table[i].flag!=0&&i<n) /*寻找空项目表*/ 
    i++; 
    if(i>=n) /*无表目填写已分分区*/ 
    { 
        printf("无表目填写已分分区，错误\n"); 
        /*修正空闲表*/ 
        if(free_table[k].flag==0) 
    /*找到的是空闲分区*/ 
       free_table[k].flag=1; 
       else 
       {/*前面找到的是某个空闲分区的一部分*/ 
          free_table[k].length=free_table[k].length+xk; 
          return 0; 
       } 
    } 

    else 
    {/*修改已分配表*/ 
        used_table[i].address=ad; 
        used_table[i].length=xk; 
        used_table[i].flag=J; 
    } 
    return 0; 
}/*主存分配函数结束*/ 

int reclaim(char J)  /*回收作业名为J的作业所占的空间*/ 
{ 
    int i,k,j,s,t; 
    float S,L; 
    /*寻找已分配表中对应登记项*/ 
    s=0; 
    while((used_table[s].flag!=J||used_table[s].flag==0)&&s<n) 
    {
    s++; 
    if(s>=n)/*在已分配表中找不到名字为J的作业*/ 
    { 
        printf("can not find that job"); 
        return 0; 
    }
    }
    /*修改已分配表*/ 
    used_table[s].flag=0; 
    /*取得归还分区的起始地址S和长度L*/ 
    S=used_table[s].address; 
    L=used_table[s].length; 
    j=-1;k=-1;i=0; 
    /*寻找回收分区的空闲上下邻，上邻表目k，下邻表目j*/ 
    while(i<m&&(j==-1||k==-1)) 
    { 
        if(free_table[i].flag==1) 
        { 
            if(free_table[i].address+free_table[i].length==S) k=i;
            /*找到上邻*/ 
            if(free_table[i].address==S+L) j=i;/*找到下邻*/ 
        }    
        i++; 
    } 

    if(k!=-1)
    { 
        if(j!=-1) 
        /* 上邻空闲区，下邻空闲区，三项合并*/ 
        { 
            free_table[k].length=free_table[j].length+free_table[k].length+L; 
            free_table[j].flag=0; 
        } 
        else 
        /*上邻空闲区，下邻非空闲区，与上邻合并*/ 
        free_table[k].length=free_table[k].length+L;
    } 
    else 
    {
        if(j!=-1) 
        /*上邻非空闲区，下邻为空闲区，与下邻合并*/ 
        { 
            free_table[j].address=S; 
            free_table[j].length=free_table[j].length+L; 
        } 
        else 
        /*上下邻均为非空闲区，回收区域直接填入上下邻均为非空闲区，回收区域直接填入*/ 
        {    
            /*在空闲区表中寻找空栏目*/
            t=0; 
            while(free_table[t].flag==1&&t<m) 
            {
            t++; 
            if(t>=m)/*空闲区表满,回收空间失败,将已分配表复原*/ 
            { 
                printf("主存空闲表没有空间,回收空间失败\n"); 
                used_table[s].flag=J; 
                return 0; 
            }
            } 
            free_table[t].address=S; 
            free_table[t].length=L; 
            free_table[t].flag=1; 
        } 
    }
    return 0; 
}/*主存回收函数结束*/ 

int main( ) 
{ 
    int i,a; 
    float xk; 
    char J; 
    /*空闲分区表初始化*/ 
    free_table[0].address=1000; 
    free_table[0].length=300; 
    free_table[0].flag=1; 
    /*新增空闲分区*/
    free_table[1].address=1301; 
    free_table[1].length=250; 
    free_table[1].flag=1;
    free_table[2].address=1552; 
    free_table[2].length=500; 
    free_table[2].flag=1;
    free_table[3].address=2500; 
    free_table[3].length=150; 
    free_table[3].flag=1;
    free_table[4].address=3000; 
    free_table[4].length=450; 
    free_table[4].flag=1;
    free_table[5].address=10240; 
    free_table[5].length=102400; 
    free_table[5].flag=1;
    for(i=6;i<m;i++) 
    free_table[i].flag=0; 
/*已分配表初始化：*/ 
    for(i=0;i<n;i++) 
    used_table[i].flag=0; 
    while(1) 
    { 
        printf("Functions: 0-exit,1-allocate the main memory,2-reclaim the main memory,3-display main memory\n"); 
        printf("please select functions(0~3) :"); 
        scanf("%d",&a); 
        switch(a) 
        { 
            case 0: exit(0); /*a=0程序结束*/ 

            case 1: /*a=1分配主存空间*/ 
            printf("input job name J and job required length xk: "); 
            scanf("%*c%c%f",&J,&xk); 
            allocate(J,xk);/*分配主存空间*/ 
            break;

            case 2: /*a=2回收主存空间*/ 
            printf("please input the job name which needs to be reclaimed"); 
            scanf("%*c%c",&J); 
            reclaim(J);/*回收主存空间*/ 
            break;

            case 3: /*a=3显示主存情况*/ 
            /*输出空闲区表和已分配表的内容*/ 
            printf("display idle partition table:  \naddress length     flag\n"); 
            for(i=0;i<m;i++) 
            printf("%6.0f%9.0f%6d\n",free_table[i].address,free_table[i].length, free_table[i].flag); 
            printf(" press random button to display allocated table\n"); 
            getch(); 
            printf(" Display allocated partition table:  \naddress  length     flag\n"); 
            for(i=0;i<n;i++) 
            if(used_table[i].flag!=0) 
            printf("%6.0f%9.0f%6c\n",used_table[i].address,used_table[i].length, used_table[i].flag); 
            else 
            printf("%6.0f%9.0f%6d\n",used_table[i].address,used_table[i].length, used_table[i].flag); 
            break;

            default:printf("Selection fault \n"); 
        }/*case*/ 
    }/*while*/ 
}/*主函数结束*/
