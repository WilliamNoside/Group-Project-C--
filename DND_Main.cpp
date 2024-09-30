# include <iostream>
# include <cstdlib>
# include <ctime>
# include <chrono> 
# include <thread> 
# include <map>
# include <future> 

/*
角色数据需要拥有HP和MP，金币Coins。 ✅
角色开局选择需要询问是想玩战士，法师还是游侠，来确定初始数值 ✅
角色会被要求进行一项任务，达成目标即可获得胜利 ✅
在城镇中，你可以在四个不同的领域展开行动，✅
在城镇里你可以进行不同的行动 ✅
钓鱼：在钓鱼小游戏中随机获取一个鱼 ✅
- 钓鱼图鉴 ✅
- 钓鱼背包 ✅
- 钓鱼时间 ✅
- 钓鱼结局 ✅
商店：售卖你的鱼或战利品，购买提升你的能力
- 
许愿：用钱向河神许愿，随机获得或失去钱 
- 
战斗：在回合制战斗中磨练你的能力
- 
目前进度
*/

//图鉴类
class FishCatalog{
    private:
        std::map<std::string, bool> fishLog;  // 每种鱼的名字且是否钓到
    public:
        FishCatalog(){
            //初始化图鉴，false为未钓到
            fishLog.insert(std::make_pair("金鱼", false));
            fishLog.insert(std::make_pair("鲤鱼", false));
            fishLog.insert(std::make_pair("鲈鱼", false));
            fishLog.insert(std::make_pair("鲢鱼", false));
            fishLog.insert(std::make_pair("草鱼", false));
            fishLog.insert(std::make_pair("大马哈鱼", false));
            fishLog.insert(std::make_pair("金枪鱼", false));
            fishLog.insert(std::make_pair("河豚", false));
        }


        // 玩家钓到鱼后更新图鉴
        void catchFish(const std::string &fishName){
            if (fishLog.count(fishName)) {
                fishLog[fishName] = true;
                std::cout << "你写进了图鉴！\n";
            }
        }

        // 图鉴显示
        void showCatalog() const {
            std::cout << "*****钓鱼图鉴*****\n";
            for (const auto& entry : fishLog){
                std::cout << entry.first << " : " << (entry.second ? "已钓到" : "未钓到") << '\n';
            }
        }
        
        // 图鉴进度显示
        void showProgress() const {
            int caught = 0;
            int total = fishLog.size();
            for (const auto & entry  : fishLog) {
                if (entry.second) {
                    caught++;
                }
            }
            std::cout << "已钓到鱼的数量: " << caught << " / " << total << '\n';
        }

        //全收集判定
        bool allFishCaught() const {
            for (const auto & entry : fishLog) {
                if(!entry.second){
                    return false;
                }
            }
            return true;
        }
};

//背包类
class Package{
    private:
        std::map<std::string, int> items;  // 物品名称,数量
    public:
        //添加物品
        void addItem(const std::string & itemName, int quantity = 1){
            items[itemName] += quantity;
            std::cout << "你将一个" << quantity << "个" << itemName << "收到了背包\n";
        }

        //移除物品
        bool removeItem(const std::string & itemName, int quantity = 1){
            if(items.count(itemName) && items[itemName] >= quantity){
                items[itemName] -= quantity;
                if (items[itemName] == 0){
                    items.erase(itemName);
                }
                std::cout << "你使用了" << quantity << "个" << itemName << "!\n";
                return true;
            }else{
                std::cout << "没有足够的" << itemName << "!\n";
                return false;
            }
        }

        //背包物品展示
        void showPackage() const {
            if (items.empty()){
                std::cout << "你的背包里没有东西\n";
            }else{
                std::cout << "^^^^^^^^^^^^^^^^^^^\n";
                std::cout << "背包内容:\n";
                for(const auto & item : items){
                    std::cout << item.first << " x " << item.second << "\n";
                }
            }
        }

        //物品检测
        bool hasItem(const std::string& itemName, int quantity = 1) const {
            return items.count(itemName) && items.at(itemName) >= quantity;
        }
};

//角色类
class Character{
    public:
        std::string Name; //名字
        std::string Classname; //职业
        int HP; //力量
        int MP; //智慧
        int Coins; //金币
        Package package;//背包
        FishCatalog fishCatalog;//图鉴
        bool hasGoldenRod = false;
        
        // 初始化角色数值，在定义怪物和初始角色时使用。

        Character(std::string playername,std::string classname, int hp, int mp, int coins){
            Name = playername;
            Classname = classname; 
            HP = hp;
            MP = mp;
            Coins = coins;
        }

        // 显示角色数据
        void showStatus(){
            std::cout << Classname << Name << "你好\n";
            std::cout << "你的HP为" << HP << "，这决定了你的攻击力和生命值\n";
            std::cout << "你的MP为" << MP << "，这决定了你的法术伤害和法力值\n";
            std::cout << "你的金币数量为" << Coins << "，你可以使用金币来兑换商店的物品\n";
            package.showPackage();  // 显示背包内容
        }

        // 背包整合角色类

        // 添加物品到背包
        void addItemToPackage(const std::string & itemName, int quantity = 1) {
            package.addItem(itemName, quantity);
        }

        // 移除物品
        bool moveItemFromPackage(const std::string & itemName, int quantity = 1) {
            return package.removeItem(itemName, quantity);
        }
        
        // 检查物品
        bool hasItem(const std::string & itemName, int quantity = 1) const {
            return package.hasItem(itemName, quantity);
        }
        void showPackage(){
            package.showPackage();  // 显示背包内容
        }

        // 图鉴类整合进角色类

        // 钓鱼后更新图鉴
        void catchFish(const std::string& fishName) {
            fishCatalog.catchFish(fishName);
            if (fishCatalog.allFishCaught()) {
                getGoldenRod();  // 授予金色钓竿
                fishingendGame();   //达成钓鱼结局
            }
        }

        // 授予金色钓竿
        void getGoldenRod() {
            hasGoldenRod = true;
            std::cout << "恭喜你！你钓上了所有的鱼，你的钓竿突然飞了起来，变成了金色钓竿！游戏结束！\n";
        }

        // 结束游戏
        void fishingendGame() {
            std::cout << "你完成了钓鱼结局！恭喜你，你是天生的钓鱼大师！\n " ;
            exit(0);
        }

        // 展示图鉴
        void viewCatalog() const {
            fishCatalog.showCatalog();
            fishCatalog.showProgress();
        }   
};

//钓鱼竿类
class FishingRod{
    public:
        int maxDurability;//最大耐久度
        int durability;//目前耐久度
        int level;//钓鱼竿等级
        int upgradeCost;//钓鱼竿花费
        int rareChance;//稀有鱼几率

        //初始化鱼竿设定
        FishingRod(int maxDur,int rodLV,int cost,int rareChancePercent){
            maxDurability = maxDur;
            durability = maxDur;
            level = rodLV;
            upgradeCost = cost;
            rareChance = rareChancePercent;
        }

        //鱼竿耐久度减少
        void decreaseDurability() {
            if (durability > 0) {
                durability--;
            }
        }

        //检测鱼竿是否报废
        bool isBroken(){
            return durability <= 0;
        }

        //修理钓竿
        void repair(){
            durability = maxDurability;
            std::cout << "钓鱼竿已修复，耐久度恢复至 " << durability << '\n';
        }
        //
        bool upgrade(Character &player){
            if (player.Coins >= upgradeCost){
                player.Coins -= upgradeCost;
                level++;
                maxDurability += 5;
                rareChance += 5;
                upgradeCost += 50;
                repair();
                std::cout << "钓鱼竿升级成功！等级提升至 " << level << "，新耐久度为 " << maxDurability << "，钓上鱼的几率提升了！\n";
                return true;
            }else {
            std::cout << "金币不足，无法升级钓鱼竿。\n";
            return false;
            }
        }

        // 钓鱼竿状态显示
        void showDurability() {
        std::cout << "钓鱼竿等级: " << level << ",钓鱼竿耐久度: " << durability << "/" << maxDurability << '\n';
    }
};

//商店类
class Shop {
public:
    // 商品价格
    const int FISH_SELL_PRICE = 10;  // 鱼的出售价格
    const int UPGRADE_COST = 50;      // 升级钓竿的费用

    // 售卖鱼
    void sellFish(Character &player) {
        std::string fishName;
        std::cout << "请输入你想出售的鱼的名字: ";
        std::cin >> fishName;

        if (player.hasItem(fishName)) {
            player.moveItemFromPackage(fishName);
            player.Coins += FISH_SELL_PRICE;
            std::cout << "成功出售 " << fishName << "，获得金币 " << FISH_SELL_PRICE << "！\n";
        } else {
            std::cout << "背包中没有 " << fishName << "！\n";
        }
    }

    // 升级钓鱼竿
    void upgradeFishingRod(FishingRod &rod, Character &player) {
        if (player.Coins >= UPGRADE_COST) {
            player.Coins -= UPGRADE_COST;
            rod.upgradeCost += 20; // 升级费用递增
            rod.maxDurability += 5; // 最大耐久度增加
            rod.level++;            // 钓鱼竿等级提升
            std::cout << "钓鱼竿升级成功！新等级: " << rod.level << ", 新耐久度: " << rod.maxDurability << ".\n";
        } else {
            std::cout << "金币不足，无法升级钓鱼竿。\n";
        }
    }

    // 商店主界面
    void shopInterface(Character &player, FishingRod &rod) {
        int choice;
        do {
            std::cout << "欢迎来到商店！\n";
            std::cout << "1. 出售鱼\n";
            std::cout << "2. 升级钓鱼竿\n";
            std::cout << "3. 购买黄金鱼竿\n";
            std::cout << "4. 退出商店\n";
            std::cin >> choice;

            //清理缓冲区，避免错误
            std::cin.clear();
            fflush(stdin);

            switch (choice) {
                case 1:
                    sellFish(player);
                    break;
                case 2:
                    upgradeFishingRod(rod, player);
                    break;
                case 3:
                    if (player.Coins >= 1000) {
                        player.Coins -= 1000;
                        rod.level = 10;
                        rod.maxDurability = 100;
                        std::cout << "购买成功！你的太有实力了，恭喜你通过了黄金鱼竿的考验！\n";
                        std::cout << "感谢游玩，游戏结束\n";
                        exit(0);
                    }else{
                        std::cout << "金币不足，无法购买黄金鱼竿。\n";
                    }
                case 4:
                    std::cout << "感谢光临！\n";
                    break;
                
                default:
                    std::cout << "请输入有效的选项！\n";
                    break;
            }
        } while (choice != 4);
    }
};

//角色类，创建角色模块
Character playerCreateCharacter(){
    std::string playerName;
    int choice;
    
    //让玩家选择名字
    std::cout << "你的名字是什么？\n";
    std::cin >> playerName;

    //让玩家选择职业
    std::cout << "请选择你的职业！\n";
    std::cout << "1. 战士 擅长肉搏和近战攻击的力量角色\n2. 游侠 穿梭在树林中的均衡角色\n";
    std::cout << "3. 法师 擅长法术博览群书的智慧角色\n4. 商人 除了有钱一无是处的经济角色\n";
    std::cin >> choice;

    //选择职业并设定初始数值
    if (choice == 1) {
        return Character(playerName,"战士", 180 , 30 , 50); //战士拥有高力量 低智慧
    }else if (choice == 3) {
        return Character(playerName,"法师", 80 , 150 , 50); //法师拥有低力量 高智慧
    }else if (choice == 2) {
        return Character(playerName,"游侠", 100 , 60 , 50); //游侠拥有中力量 中智慧
    }else {
        return Character(playerName,"商人", 80 , 30 , 300); //商人拥有中力量 低智慧
    }
};

//钓鱼奖励模块
std::string fishReward(FishingRod &rod){
    const char* garbage[] = {"旧报纸" , "破眼镜" , "垃圾" , "可乐罐" , "破布"};
    const char* commonFish[] = {"金鱼" , "鲤鱼" , "鲈鱼" , "鲢鱼" , "草鱼"};
    const char* rareFish[] = {"大马哈鱼" , "金枪鱼" , "河豚"};
    const char* treasure[] = {"宝藏箱" , "古代项链" , "金戒指"};

    int fishChance = rand() % 100;

    if (fishChance < 30 - (rod.rareChance/5)) {
        int i = rand() % (sizeof(garbage) / sizeof(garbage[0]));
        return garbage[i];
    }else if(fishChance < 80 - rod.rareChance) {
        int i = rand() % (sizeof(commonFish) / sizeof(commonFish[0]));
        return commonFish[i];
    }else if(fishChance < 95) {
        int i = rand() % (sizeof(rareFish) / sizeof(rareFish[0]));
        return rareFish[i];
    }else{
        int i = rand() % (sizeof(treasure) / sizeof(treasure[0]));
        return treasure[i];}
}

//钓鱼模块
void fishing (Character &player , FishingRod &rod){
    char choice;
    //准备钓鱼
    std::cout << player.Name << "来到了河边，准备钓鱼!\n";
    do {
        if(rod.isBroken()){
            std::cout << "你的钓竿坏了，快去修修吧！";
            break;
        }

        //开始钓鱼
        std::cout << "--------------\n";
        std::cout << "开始钓鱼！\n";
        rod.decreaseDurability(); //耐久度减少
        rod.showDurability(); //显示钓鱼竿数据


        //设置随机数，1-5秒后上钩
        int waitTime = rand() % 5 + 1 ;
        std::cout << "鱼将在" << waitTime << "秒后上钩，做好准备！\n";

        //等待时间
        std::this_thread::sleep_for(std::chrono::seconds(waitTime));

        //等待结束后咬勾
        std::cout << "鱼上钩了！快在两秒内按下'f'键来拉钩！\n";

        //咬勾时间
        std::future<char> inputFuture = std::async(std::launch::async, []() {
            char input;
            std::cin >> input;
            return input;
        });

        bool success = false;

        //咬勾循环，两秒内若没有输入F则失败，输入则成功
        if (inputFuture.wait_for(std::chrono::seconds(2)) == std::future_status::ready) {
            char input = inputFuture.get();
            if (input == 'f' || input == 'F') {
                success = true;
            }
        }

        //根据Success变量判断是否上钩
        if(success){
            std::string fishResult = fishReward(rod);
            std::cout << "====================" << "\n";
            std::cout << "恭喜！你钓到了: " << fishResult << "！\n";

            //判定战利品，并给予奖励
            if (fishResult == "垃圾"|| fishResult == "可乐罐" || fishResult == "破布"){
                std::cout << "真没素质，往河里丢垃圾！\n";
            }else if(fishResult == "破眼镜"){
                std::cout << "谁掉河里了？\n";
            }else if(fishResult == "旧报纸"){
                std::cout << "这是10年前的新闻啊！\n";
            }else if(fishResult == "金鱼"|| fishResult == "鲤鱼"){
                std::cout << "小鱼也是鱼！\n";
                player.addItemToPackage(fishResult); 
                player.catchFish(fishResult);
            }else if(fishResult == "鲈鱼"){
                std::cout << "是鲈鱼不是鱼露\n";
                player.addItemToPackage(fishResult); 
                player.catchFish(fishResult);
            }else if(fishResult == "鲢鱼"|| fishResult == "草鱼"){
                std::cout << "钓鱼佬永不空军！\n";
                player.addItemToPackage(fishResult); 
                player.catchFish(fishResult);
            }else if(fishResult == "大马哈鱼"|| fishResult == "金枪鱼"){
                std::cout << "好大一条鱼！\n";
                player.addItemToPackage(fishResult); 
                player.catchFish(fishResult);
            }else if(fishResult == "河豚"){
                std::cout << "小心别被扎到了！\n";
                player.addItemToPackage(fishResult); 
                player.catchFish(fishResult);
            }else if(fishResult == "宝藏箱"||fishResult == "古代项链"||fishResult == "金戒指"){
                std::cout << "哇，发财！\n";
                player.Coins += 200;
            }
        }
        else{
            std::cout << "==============\n";
            std::cout << "哎呀，鱼跑掉了！\n";
        }
        //继续钓鱼判定
        std::cout << "是否继续钓鱼？(y/n): \n";
        std::cin >> choice;
    }while (choice == 'y' || choice == 'Y');
}

//许愿模块
void wish(Character &player) {
        char choice;
        bool goldfishingrod = false;

        do {
            if (player.Coins <= 0) {
                std::cout << player.Name << "你来到河边，向河神许愿，河神并没有出现！\n";
                break;
            } else {
                std::cout << player.Name << "你来到河边，向河神许愿，河神出现！\n";
                player.Coins -= 10; 
                int num = (rand() % 20) + 1; 
                
                switch (num) {
                    case 1:
                        std::cout << "你向河神许愿，获得了一些钱！\n";
                        player.Coins += 50;
                        break;
                    case 2:
                        std::cout << "你向河神许愿，失去了一些钱！\n";
                        player.Coins -= 50;
                        break;
                    case 3:
                        std::cout << "你向河神许愿，什么都没有获得！\n";
                        break;
                    case 4:
                        std::cout << "你向河神许愿，喝了一口核废水！\n";
                        player.HP -= 10;
                        std::cout << "你的生命值为" << player.HP << "。\n";
                        break;
                    case 5:
                        std::cout << "你向河神许愿，河神给了你祝福！\n";
                        player.MP += 30;
                        std::cout << "你的法术值为" << player.MP << "。\n";
                        break;
                    case 6:
                        std::cout << "你向河神许愿，河神给了你诅咒！\n";
                        player.MP -= 10;
                        std::cout << "你的法术值为" << player.MP << "。\n";
                        break;
                    case 7:
                        std::cout << "你向河神许，获得了一个蛋！\n";
                        player.HP += 10;
                        std::cout << "你的生命值为" << player.HP << "。\n";
                        break;
                    case 8:
                        std::cout << "你向河神许愿，商店打折！\n";
                        player.Coins += 30;
                        break;
                    case 9:
                        std::cout << "你向河神许愿，商店加价！\n";
                        player.Coins -= 30;
                        break;    
                    case 10:
                        std::cout << "你向河神许愿，获得垃圾！\n";
                        break;
                    case 11:
                        std::cout << "你向河神许愿，获得了金币！\n";
                        player.Coins += 30;
                        break;   
                    case 12:
                        std::cout << "你向河神许愿，获得银币！\n";
                        player.Coins += 20;
                        break;         
                    case 13:
                        std::cout << "你向河神许愿，获得铜币！\n";
                        player.Coins += 10;
                        break;    
                    case 14:
                        std::cout << "你向河神许愿，什么都没有获得！\n";
                        break;
                    case 15:
                        std::cout << "你向河神许愿，获得了黄金鱼竿！\n";
                        std::cout << "恭喜你，你获得了胜利!\n";
                        exit(0);
                }
            }
            std::cout << "当前金币数量: " << player.Coins << "\n";
            std::cout << "是否继续许愿？(y/n): ";
            std::cin >> choice;
        } while (choice == 'y' || choice == 'Y');
    }


//小镇模块
void town (Character &player , FishingRod & rod){
    int move;
    Shop shop;
    do{
        std::cout << "++++++++++++++++++++\n";
        std::cout << "你现在在鲈鱼镇中，请选择你接下来要去干什么\n";
        std::cout << "++++++++++++++++++++\n";
        std::cout << "1.商店\n";
        std::cout << "2.钓鱼\n";
        std::cout << "3.钓鱼图鉴\n";
        std::cout << "4.前去郊外\n";
        std::cout << "5.查看背包和状态\n";
        std::cout << "6.许愿\n";
        std::cout << "7.我不玩了\n";
        std::cout << "+++++++++++++++++++++\n";

        //移动
        std::cin >> move;

        //清理缓冲区，避免错误
        std::cin.clear();
        fflush(stdin);
        
        //检测移动，并执行
        switch(move){
            case 1:
                shop.shopInterface(player, rod); // 进入商店
                break;
            case 2:
                std::cout << "正在前往小溪！\n";
                fishing(player,rod);
                break;
            case 3:
                player.viewCatalog();
                break;
            case 4:
                std::cout << "战斗系统仍在开发中，敬请期待！\n";
                //battling(player);
                break;
            case 5:
                player.showStatus();
                break;
            case 6:
                wish(player);
                break;
            case 7:
                std::cout << "下次再玩！\n";
                std::cout << "+++++++++++++++++++++\n";
                break;
            default:
                std::cout << "请输入有效的数字！\n" ;
                break;
        }
    }while (move != 7);
};


//Main方法
int main(){
    //随机数种子
    srand(time(NULL));

    //角色创建模块
    Character player = playerCreateCharacter();
    player.showStatus();

    // 故事剧情
    std::cout << "下次再玩！\n";

    //创建初始钓鱼竿
    FishingRod rod(10, 1, 50, 0);

    // 进入城镇
    town(player,rod);

    return 0;
}