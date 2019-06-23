#pragma once

#include <BattleShipGame/Wrapper/AI.h>
#include <BattleShipGame/Ship.h>
#include <BattleShipGame/Board.h>
#include <algorithm>
#include <random>
#include <ctime>
#include <iostream>

class AI : public AIInterface
{
    std::vector<std::pair<int,int>> way;
    std::vector<std::pair<int,int>> priority_way;
    std::vector<TA::Ship> ship;
    int pos_x, pos_y;
    int x_size, y_size;
    int attack_status;
    int board[21][21];
public:
    virtual std::vector<TA::Ship> init(int size ,std::vector<int> ship_size, bool order, std::chrono::milliseconds runtime) override
    {
        (void)ship_size;
        (void)runtime;
        
        ship.push_back({3, 0,  0,TA::Ship::State::Available});
        ship.push_back({3, 0,  3,TA::Ship::State::Available});
        ship.push_back({5, 14, 0,TA::Ship::State::Available});
        ship.push_back({7, 10, 10,TA::Ship::State::Available});
        
        for(int i=2;i<size;i+=3)
            for(int j=2;j<size;j+=3)
                way.emplace_back(i,j);

        std::mt19937 mt;
        mt.seed( std::time(nullptr) + 7122 + (order?1:0) );
        std::shuffle(way.begin(), way.end(), mt);
        return ship;
    }

    virtual void callbackReportEnemy(std::vector<std::pair<int,int>> pos) override
    {
        if(pos.empty()){
            return ;
        }
        else{
            for(auto i : pos){
                
            }
        }
    }

    virtual std::pair<int,int> queryWhereToHit(TA::Board) override
    {
        if(way.empty()) std::cout<<"empty";
        auto res = way.back();
        way.pop_back();
        return res;
    }

    virtual void callbackReportHit(bool)  override
    {

    }

    virtual std::vector<std::pair<int,int>> queryHowToMoveShip(std::vector<TA::Ship>) override
    {
        std::vector<std::pair<int,int>> ship_locate;
        for(auto i : ship){
            ship_locate.emplace_back(i.x, i.y);
        }
        return ship_locate;
    }
};