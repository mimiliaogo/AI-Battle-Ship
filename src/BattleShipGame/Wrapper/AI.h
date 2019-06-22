#pragma once

#include <BattleShipGame/Board.h>
#include <BattleShipGame/Ship.h>

#include <chrono>
#include <vector>
#include <utility>
#include <algorithm>
#include <random>
#include <ctime>
#include <queue>

#define AI_ABI_VER 20190528

class AIInterface
{
    std::queue<std::pair<int,int>> way;
    std::queue<std::pair<int,int>> hitway;
    int valid[23][23];
    int attack_x, attack_y;
    int hit_x;
    int report_hit_x, report_hit_y;
    int cnt_shipsize;
    int right_bound, up_bound;
    bool hit_state;

public:
    AIInterface() = default;
    virtual ~AIInterface() = default;

    int abi(){ return AI_ABI_VER; }
    virtual std::vector<TA::Ship> init(int size ,std::vector<int> ship_size, bool order, std::chrono::milliseconds runtime)
    {
        (void)ship_size;
        (void)runtime;

        std::vector<TA::Ship> tmp;
        tmp.push_back({3, 0,  0,TA::Ship::State::Available});
        tmp.push_back({3, 5,  0,TA::Ship::State::Available});
        tmp.push_back({5, 0,  5,TA::Ship::State::Available});
        tmp.push_back({7, 10, 10,TA::Ship::State::Available});

        int i,j;
        for(i = 0; i < 20; i++){
            for(j = 0; j < 20; j++){
                valid[i][j] = 1;
            }
        }
        hit_state = false;

        for(i = 2; i < 20; i += 6){
            for(j = 2; j < 20; j+= 6){
                way.emplace(i, j);
            }
        }
        for(i = 5; i < 20; i += 6){
            for(j = 5; j < 20; j+= 6){
                way.emplace(i, j);
            }
        }
        for(i = 2; i < 20; i += 6){
            for(j = 5; j < 20; j+= 6){
                way.emplace(i, j);
            }
        }
        for(i = 2; i < 20; i += 6){
            for(j = 5; j < 20; j+= 6){
                way.emplace(i, j);
            }
        }


        return tmp;
    }

    virtual void callbackReportEnemy(std::vector<std::pair<int,int>> pos) 
    {
        report_hit_x = pos.back().first;
        report_hit_y = pos.back().second;
    }

    virtual std::pair<int,int> queryWhereToHit(TA::Board) 
    {
        if(hit_state){
            while(true){
                auto res = hitway.front();
                attack_x = res.first;
                attack_y = res.second;
                hitway.pop();
                if(valid[attack_x][attack_y]){
                    valid[attack_x][attack_y] = 0;
                    return res;
                }
            }
        }
        else{
            while(true){
                auto res = way.front();
                attack_x = res.first;
                attack_y = res.second;
                way.pop();
                if(valid[attack_x][attack_y]){
                    valid[attack_x][attack_y] = 0;
                    return res;
                }
            }
        }

    }

    virtual void callbackReportHit(bool hit)  
    {
        if(hit && hit_state==false){
            hit_state = true;
            hit_x = attack_x;
            cnt_shipsize = 0;
        }

        if(!hit_state) return;

        if(cnt_shipsize <= 10){
            if(hit){
                cnt_shipsize++;
                while(true){
                    if(attack_x+1 >= 20){
                        right_bound = 20;
                        attack_x = hit_x;
                        cnt_shipsize += 10;
                        break;
                    }
                    if(valid[attack_x+1][attack_y]){
                        hitway.emplace(attack_x+1, attack_y);
                        return;
                    }
                    else{
                        attack_x += 1;
                        cnt_shipsize++;
                    }
                }
            }
            else{
                right_bound = attack_x;
                attack_x = hit_x;
                cnt_shipsize += 10;
            }
        }

        if(cnt_shipsize >= 10 && cnt_shipsize <= 20){
            if(hit){
                while(true){
                    if(attack_x-1 < 0){
                        attack_x = right_bound / 2;
                        cnt_shipsize += 10;
                        break;
                    }
                    if(valid[attack_x-1][attack_y]){
                        hitway.emplace(attack_x-1, attack_y);
                        return;
                    }
                    else{
                        attack_x -= 1;
                        cnt_shipsize++;
                    }
                }
            }
            else{
                hit_x = right_bound - attack_x;   // hit_x serves as the boat's size now
                attack_x += hit_x / 2;
                cnt_shipsize += 10;
            }
        }

        if(cnt_shipsize >= 20 && cnt_shipsize <= 30){
            if(hit){
                while(true){
                    if(attack_y >= 20){
                        up_bound = 20;
                        cnt_shipsize += 10;
                        break;
                    }
                    if(valid[attack_x][attack_y+1]){
                        hitway.emplace(attack_x, attack_y+1);
                        return;
                    }
                    else{
                        attack_y += 1;
                        cnt_shipsize++;
                    }
                }
            }
            else{
                up_bound = attack_y;
                cnt_shipsize += 10;
            }
        }

        if(cnt_shipsize >= 30){
            attack_y = up_bound - hit_x/2;
            hitway.emplace(attack_x, attack_y);
            for(int i = right_bound-hit_x+1; i < right_bound; i++){
                for(int j = up_bound-hit_x+1; j < up_bound; j++){
                    valid[i][j] = 0;
                }
            }
            hit_state = false;
        }

    }

    virtual std::vector<std::pair<int,int>> queryHowToMoveShip(std::vector<TA::Ship> sh) 
    {
        /*
        int dx = sh.x - hit_x;
        int dy = sh.y - hit_y;
        int ab_dx = max(dx, -dx);
        int ab_dy = max(dy. -dy);

        if(ab_dx > ab_dy){
            sh.y = y + dy;
        }else{
            sh.x = x + dx;
        }
    */
    }
    
};
