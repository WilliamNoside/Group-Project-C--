#include <iostream>
#include <string>
#include <vector>
#include <ctime>
#include <cstdlib>

using namespace std;

// 基础怪物类
class Monster {
public:
    string name;
    int hp;  // 生命值
    int mp;  // 魔法值

    Monster(string name, int hp, int mp) : name(name), hp(hp), mp(mp) {}

    // 显示怪物属性
    virtual void showStats() {
        cout << "Monster: " << name << ", HP: " << hp << ", MP: " << mp << endl;
    }

    virtual ~Monster() {} // 虚析构函数
};

// 小怪类派生自 Monster
class SmallMonster : public Monster {
public:
    SmallMonster(string name, int hp, int mp) : Monster(name, hp, mp) {}

    void showStats() {
        cout << "Small Monster: " << name << ", HP: " << hp << ", MP: " << mp << endl;
    }
};

// Boss类派生自 Monster
class Boss : public Monster {
public:
    Boss(string name, int hp, int mp) : Monster(name, hp, mp) {}

    void showStats() {
        cout << "Boss: " << name << ", HP: " << hp << ", MP: " << mp << endl;
    }
};

// 初始化小怪
vector<SmallMonster> initSmallMonsters() {
    vector<SmallMonster> monsters;
    monsters.push_back(SmallMonster("哥布林", 50, 10));
    monsters.push_back(SmallMonster("兽人", 70, 15));
    monsters.push_back(SmallMonster("小骷髅", 40, 5));
    monsters.push_back(SmallMonster("史莱姆", 30, 0));
    monsters.push_back(SmallMonster("野狼", 60, 10));
    monsters.push_back(SmallMonster("僵尸", 55, 0));
    monsters.push_back(SmallMonster("蝙蝠", 35, 20));
    monsters.push_back(SmallMonster("蜘蛛", 45, 15));
    monsters.push_back(SmallMonster("变异鼠", 25, 5));
    monsters.push_back(SmallMonster("幽灵", 40, 30));
    return monsters;
}

// 初始化Boss
vector<Boss> initBosses() {
    vector<Boss> bosses;
    bosses.push_back(Boss("双足飞龙", 300, 150));
    bosses.push_back(Boss("骷髅王", 350, 200));
    bosses.push_back(Boss("巫妖", 280, 250));
    bosses.push_back(Boss("九头蛇", 320, 180));
    bosses.push_back(Boss("比蒙巨兽", 400, 100));
    return bosses;
}

// 显示所有怪物的属性
void showMonsters(vector<SmallMonster> &smallMonsters, vector<Boss> &bosses) {
    cout << "Small Monsters:\n";
    for (int i = 0; i < smallMonsters.size(); ++i) {
        smallMonsters[i].showStats();
    }
    cout << "\nBosses:\n";
    for (int i = 0; i < bosses.size(); ++i) {
        bosses[i].showStats();
    }
}

int main() {
    srand(static_cast<unsigned int>(time(0))); // 初始化随机种子

    // 初始化小怪和Boss
    vector<SmallMonster> smallMonsters = initSmallMonsters();
    vector<Boss> bosses = initBosses();

    // 显示所有怪物
    showMonsters(smallMonsters, bosses);

    return 0;
}
