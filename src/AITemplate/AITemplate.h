#pragma once

#include <BattleShipGame/Wrapper/AI.h>
#include <BattleShipGame/Ship.h>
#include <BattleShipGame/Board.h>
#include <algorithm>
#include <random>
#include <ctime>
#include <iostream>
#include <vector>

class AI : public AIInterface
{
    std::vector<std::pair<int,int>> way;
    std::vector<TA::Ship> ship;
    int x_buffer, y_buffer;
    int board_size;
    int enemy_board[21][21] = {0};
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
        board_size = size;
        return ship;
    }

    virtual void callbackReportEnemy(std::vector<std::pair<int,int>>) override
    {
        return ;
    }

    virtual std::pair<int,int> queryWhereToHit(TA::Board) override
    {    
        auto res = way.back();
        while(1){
            res = way.back();
            if(enemy_board[res.first][res.second] == 1){
                way.pop_back();
            }
            else if(enemy_board[res.first][res.second] == 0){
                enemy_board[res.first][res.second] = 1;
                break;
            }
        }
        x_buffer = res.first;
        y_buffer = res.second;
        std::cout<<"attack: "<<x_buffer<<" "<<y_buffer<<" ";
        way.pop_back();
        return res;
        
    }

    virtual void callbackReportHit(bool s)  override
    {
        if(s == true){
            std::cout<<"Hit ";
            //std::cout<<isValidPlace(x_buffer+1, y_buffer)<<" "<<(enemy_board[x_buffer+1][y_buffer] == 0)<<" ";
            if(isValidPlace(x_buffer+1, y_buffer) && enemy_board[x_buffer+1][y_buffer] == 0){
                //std::cout<<x_buffer + 1<<" "<<y_buffer<<" ";
                way.emplace_back(x_buffer+1, y_buffer);
            }
            //std::cout<<isValidPlace(x_buffer-1, y_buffer)<<" "<<(enemy_board[x_buffer-1][y_buffer] == 0)<<" ";
            if(isValidPlace(x_buffer-1, y_buffer) && enemy_board[x_buffer-1][y_buffer] == 0){
                //std::cout<<x_buffer - 1<<" "<<y_buffer<<" ";
                way.emplace_back(x_buffer-1, y_buffer);
            }
            //std::cout<<isValidPlace(x_buffer, y_buffer+1)<<" "<<(enemy_board[x_buffer][y_buffer+1] == 0)<<" ";
            if(isValidPlace(x_buffer, y_buffer+1) && enemy_board[x_buffer][y_buffer+1] == 0){
                //std::cout<<x_buffer<<" "<<y_buffer + 1<<" ";
                way.emplace_back(x_buffer, y_buffer+1);
            }
            //std::cout<<isValidPlace(x_buffer, y_buffer-1)<<" "<<(enemy_board[x_buffer+1][y_buffer-1] == 0)<<" ";
            if(isValidPlace(x_buffer, y_buffer-1) && enemy_board[x_buffer][y_buffer-1] == 0){
                //std::cout<<x_buffer<<" "<<y_buffer - 1<<" ";
                way.emplace_back(x_buffer, y_buffer-1);
            }
        }
        std::cout<<std::endl;
    }

    virtual std::vector<std::pair<int,int>> queryHowToMoveShip(std::vector<TA::Ship>) override
    {
        std::vector<std::pair<int,int>> ship_locate;
        for(auto i : ship){
            ship_locate.emplace_back(i.x, i.y);
        }
        return ship_locate;
    }
    bool isValidPlace(int x, int y){
        if(0<=x && x<board_size && 0<=y && y<board_size){
            return true;
        }
        else {
            return false;
        }
    }
    void showWay(void){
        std::cout<<"way:"<<std::endl;
        for(auto i : way){
            std::cout<<i.first<<" "<<i.second<<std::endl;
        }
        return ;
    }
};