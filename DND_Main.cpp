# include <iostream>
# include <ctime>

/*
角色数据需要拥有HP和MP，金币Coins。
角色开局选择需要询问是想玩战士，法师还是游侠，来确定初始数值
角色会被要求进行一项任务，达成目标即可获得胜利
在城镇中，你可以在四个不同的领域展开行动，
在城镇里你可以进行不同的行动
钓鱼：在钓鱼小游戏中随机获取一个鱼
商店：售卖你的鱼或战利品，购买提升你的能力
许愿：用钱向河神许愿，随机获得或失去钱
战斗：在回合制战斗中磨练你的能力

*/

void buildChar(char *spaces);

class Char{
    public:
        std::string Charname;
        int HP;
        int MP;
        
};

int main(){
    Char player;

    bool running = true;


    return 0
}