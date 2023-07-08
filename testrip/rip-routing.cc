#include <iostream>
#include <fstream>
#include <algorithm>
#include <string>
#include <vector>
#include <cstdio>
#include <unistd.h>
#include <sys/time.h>
using namespace std;

const int maxR = 6; // 最大路由器数量

// 时间间隔辅助函数
typedef long long ll;
ll getNowTime()
{ // 返回秒数
    struct timeval tv;
    struct timezone tz;
    gettimeofday(&tv, &tz);
    return tv.tv_sec; // 零头的微秒忽略
}

// 路由表项
struct RIP
{
    char name;     // 本路由器名称
    int N;         // 到目的网络N
    int d;         // 距离d
    char X;        // 下一跳路由器
    ll time;       // 上次更新时间
    bool is_valid; // 是否有效
    RIP() { is_valid = true; }
};

// 发送的报文打包
vector<RIP> Send(vector<RIP> r, char c)
{
    for (int i = 0; i < r.size(); i++)
    {
        r[i].d++;
        r[i].X = c;
        r[i].time = getNowTime(); // 时间也要更新
    }
    return r;
}

vector<RIP> rip[maxR];       // 路由表，二维数组
char idToRname[] = "ABCDEF"; // int编号改为字母标号 0-A 1-B 2-C 3-D 4-E 5-F
vector<char> ve[maxR];       // 一个vector<RIP>包含多条记录是一个路由表 ve则可表示路由器分布图邻接表

// 用文件file的内容更新各个路由器
void init(char *file)
{
    fstream infile(file);
    if (!infile.is_open())
        cout << "未成功打开文件!" << endl;
    int N;
    char name;
    RIP rp;
    infile >> N;
    for (int i = 0; i < N; i++)
    {                   // 此处N和maxR设置得一样大 当然可小不可大
        infile >> name; // 因为按顺序的所以不处理了
        rp.name = name; // 处理下也行 不过一次处理一次就够了  冗余但安全性高了
        while (infile >> rp.N >> rp.d >> rp.X && rp.N)
        {
            rp.time = getNowTime();
            rip[i].push_back(rp);
        }
    }
    infile.close();
}

// 打印路由表
void printR()
{
    vector<RIP> r;
    for (int i = 0; i < maxR; i++)
    {
        r = rip[i];
        cout << "***********************路由表" << idToRname[i] << "************************\n";
        cout << "到目的网络N\t\t距离d\t\t下一跳路由器X\n";
        for (int i = 0; i < r.size(); i++)
        {
            cout << r[i].N << "\t\t\t\t" << r[i].d << "\t\t" << r[i].X << endl;
        }
    }
}

// 用领接表初始化网络结构图
void initve(char *file)
{
    fstream infile(file);
    if (!infile.is_open())
        cout << "未成功打开文件!" << endl;
    char v, v1;
    while (infile >> v)
    { // 第一个是当前结点 后面的都是当前结点的所有邻居 0表示结束
        while (infile >> v1 && v1 != '0')
        {
            ve[v - 'A'].push_back(v1);
        }
    }
    infile.close();
}

// 打印图
void printG()
{
    cout << "**********************网络拓扑结构********************\n";
    for (int i = 0; i < maxR; i++)
    {
        cout << char(i + 'A') << " ";
        for (int j = 0; j < ve[i].size(); j++)
        {
            cout << ve[i][j];
            if (j < ve[i].size() - 1)
                cout << " ";
        }
        cout << endl;
    }
}

// 路由表排序依据
bool cmp(RIP r1, RIP r2)
{
    return r1.N < r2.N;
}

// 正式发送更新报文更新路由表了
bool update()
{
    bool hasEdit = false; // 本次更新有无修改
    vector<RIP> send;
    vector<RIP> rip1[maxR];
    for (int i = 0; i < maxR; i++)
        rip1[i] = rip[i]; // 不一定赋值成功
    for (int i = 0; i < maxR; i++)
    { // 遍历每个路由结点
        // rip里是本次更新前所有路由表的状态   rip1是更新过程中每个路由表的状态
        // 为了达到同时发送不产生连锁效应 故先备份新状态rip1 全部更新完毕再一次性协会rip

        // 传旧的         i号结点传的
        send = Send(rip[i], i + 'A'); // 向邻居发送的路由更新报文 i就是路由器编号 rip和ve的存储顺序一致
        /*** Send函数代码
        vector<RIP> Send(vector<RIP> r, char c)
            for (int i = 0; i < r.size(); i++)
            {
                r[i].d++;
                r[i].X = c;
                r[i].time = getNowTime(); // 时间也要更新
            }
            return r;
        ***/

        for (int j = 0; j < ve[i].size(); j++)
        { // 遍历第i个路由器的所有邻居 目的就是更新邻居啊
            // 本次给邻居ve[i][j] 一个字母 -'A'就是序号
            int lid = ve[i][j] - 'A';
            for (int x = 0; x < send.size(); x++)
            { // 一条一条遍历发过来的更新报文 一条一条插入
                int k;
                // 检查筛选时要筛选新的
                for (k = 0; k < rip1[lid].size(); k++)
                { // 遍历邻居路由表 查找更新报文的每一条
                    // 开始rip1是rip的备份 新的都写到rip1里
                    /*rip[lid]是第lid个路由表  rip[lid][k]是第lid个路由表的第k条记录*/
                    if (send[x].N == rip1[lid][k].N)
                    { // 有 判断逻辑:目的地址相同
                        if (send[x].X == rip1[lid][k].X)
                        { // 下一跳相同 要新的
                            if (rip1[lid][k].d != send[x].d)
                            { // N X d都相同  更新却无任何更改(仅仅改时间...)
                                rip1[lid][k] = send[x];
                                hasEdit = true;
                            }
                        }
                        else
                        { // 下一跳不同要短的
                            if (send[x].d < rip1[lid][k].d)
                            {
                                rip1[lid][k] = send[x];
                                hasEdit = true;
                            }
                        }
                        break;
                    }
                }

                if (k == rip1[lid].size())
                { // 没找到 是新的 直接加入
                    rip1[lid].push_back(send[x]);
                    hasEdit = true;
                }
            }
        }
    }

    // 一次更新完毕 才写回rip
    for (int i = 0; i < maxR; i++)
    {
        rip[i] = rip1[i];                        // 不一定赋值成功
        sort(rip[i].begin(), rip[i].end(), cmp); // 按目的地址排序
    }
    return hasEdit; // 返回本次有无更改
}

// 检查有无长期未更新的路由表项
void check()
{
    ll t1 = getNowTime();
    for (int i = 0; i < maxR; i++)
    {
        for (int j = 0; j < rip[i].size(); j++)
        {
            if (rip[i][j].is_valid && (t1 - rip[i][j].time) > 180)
                rip[i][j].is_valid = false; // 180s无更新 标记无效
            else if (!rip[i][j].is_valid && (t1 - rip[i][j].time) > 120)
                rip[i].erase(rip[i].begin() + j); // 无效后120s无更新 删除
        }
    }
}

int main()
{

    // freopen("运行结果.txt","w",stdout);
    init("../routes.txt"); // 用文件routes.txt初始化
    printR();
    initve("../netGraphic.txt"); // 初始化网络拓扑图
    printG();
    int T = 0, n = 0;
    while (true)
    {
        sleep(1); // 每隔3秒更新路由表 为了演示运行结果方便不设置为30s秒了
        cout << "\n------------------------第" << ++T << "次更新------------------------\n";
        if (update())
        {             // 一次更新
            printR(); // 打印路由表
            n++;
        }
        else
        {
            cout << "本次没有任何有价值的修改!说明前面" << n << "次修改后,每一个路由器都得到了全局路由信息!\n";
        }
        check();
    }
    return 0;
}