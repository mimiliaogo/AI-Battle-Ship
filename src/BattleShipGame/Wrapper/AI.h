#pragma once

#include <BattleShipGame/Board.h>
#include <BattleShipGame/Ship.h>

#include <chrono>
#include <vector>
#include <utility>
#include <algorithm>
#include <random>
#include <ctime>

#define AI_ABI_VER 20190528

class AIInterface
{
public:
    AIInterface() = default;
    virtual ~AIInterface() = default;

    int abi(){ return AI_ABI_VER; }
    std::vector<std::pair<int,int>> way;
    /*
        pass game information in the beginning
        it should return how to put you ships
    */
    virtual std::vector<TA::Ship> init(int size, std::vector<int> ship_size, bool order, std::chrono::milliseconds runtime){
        (void)ship_size;
        (void)runtime;

        std::vector<TA::Ship> tmp;
        tmp.push_back({3, 0,  0,TA::Ship::State::Available});
        tmp.push_back({3, 5,  0,TA::Ship::State::Available});
        tmp.push_back({5, 0,  5,TA::Ship::State::Available});
        tmp.push_back({7, 10, 10,TA::Ship::State::Available});
        
        for(int i=0;i<size;++i)
            for(int j=0;j<size;++j)
                way.emplace_back(i,j);

        std::mt19937 mt;
        mt.seed( std::time(nullptr) + 7122 + (order?1:0) );
        std::shuffle(way.begin(), way.end(), mt);
        return tmp;
    }

    /*
        for each each round, the host will call callbackEnemyReport to pass where the enemy hit
        first player in first round, vector size will be zero
    */
    virtual void callbackReportEnemy(std::vector<std::pair<int,int>>){

    }

    /*
        for each ship, if the ship state is Ship::State::Available,
        The host will use queryHowToMoveShip(Ship) to query how to move
    */
    virtual std::pair<int,int> queryWhereToHit(TA::Board){
        auto res = way.back();
        way.pop_back();
        return res;
    }

    /*
        it's be called after queryWhereToHit
        report the state of the enemy
    */
    virtual void callbackReportHit(bool){

    }

    /*
        for each ship, if the ship state is Ship::State::Available,
        The host will use queryHowToMoveShip(Ship) to query how to move

        this function should return (x, y) using std::pair

        return : i-th element is new position of the i-th ship
    */
    virtual std::vector<std::pair<int,int>> queryHowToMoveShip(std::vector<TA::Ship>){
        return {};
    }
};
