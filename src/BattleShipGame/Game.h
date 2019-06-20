#pragma once

#include <BattleShipGame/Board.h>
#include <BattleShipGame/Wrapper/AI.h>
#include <GUI/GUIInterface.h>

#include <iostream>
#include <cassert>
#include <chrono>
#include <cstdarg>
#include <future>
#include <type_traits>

namespace TA
{
    class BattleShipGame
    {
    public:
        BattleShipGame(
            int size,
            std::chrono::milliseconds runtime_limit = std::chrono::milliseconds(1000)
        ):
            m_size(size),
            m_runtime_limit(runtime_limit),
            m_P1(nullptr),
            m_P2(nullptr),
            m_P1Board(size),
            m_P2Board(size)
        {
            gui = new ASCII;
            m_ship_size = {3,3,5,7};
        }

        void setPlayer1(AIInterface *ptr) { assert(checkAI(ptr)); m_P1 = ptr; }
        void setPlayer2(AIInterface *ptr) { assert(checkAI(ptr)); m_P2 = ptr; }

        void run()
        {
            gui->title();
            if( !prepareState() ) return ;

            updateGuiGame();
            //Todo: Play Game

            while(1){//we will break out the game if lose
                //=========================Player1 attack=========================
                //-----------------------------update-----------------------------
                //hint : callbackReportEnemy
                call(&AIInterface::callbackReportEnemy, m_P1, m_P1_callbackReportEnemy);
                m_P1_callbackReportEnemy.clear();
                //-----------------------------attack-----------------------------
                //hint : queryWhereToHit
                //hint : callbackReportHit
                int m_P1_can_attack = 0;
                bool m_P1_report_hit = false;
                Ship m_P1_attacked_ship;
                std::pair<int, int> m_P1_attack_place;

                std::cout<<"player1's turn"<<std::endl;
                
                for(auto i : m_P1Ship){
                    if(i.state != Ship::State::Sink)
                        m_P1_can_attack++;
                }

                for(int i=0; i<m_P1_can_attack; i++){
                    m_P1_attack_place = call(&AIInterface::queryWhereToHit, m_P1, m_P2Board);
                    if(m_P2Board[m_P1_attack_place.first][m_P1_attack_place.second] != Board::State::Unknown){
                        std::cout<<"Player 1 AI choose a place not Unknown"<<std::endl;
                        i--;
                        continue;
                    }
                    m_P2_callbackReportEnemy.push_back(m_P1_attack_place);
                    for(auto s : m_P2Ship){
                        if(s.x <= m_P1_attack_place.first && m_P1_attack_place.first < s.x + s.size &&
                            s.y <= m_P1_attack_place.second && m_P1_attack_place.second < s.y + s.size){
                            m_P1_report_hit = true;
                            m_P1_attacked_ship = s;
                            break;
                        }
                    }
                    call(&AIInterface::callbackReportHit, m_P1, m_P1_report_hit);
                    if(m_P1_report_hit){
                        m_P2Board[m_P1_attack_place.first][m_P1_attack_place.second] = Board::State::Hit;
                        if(m_P1_attacked_ship.x == m_P1_attack_place.first + m_P1_attacked_ship.size/2 && 
                            m_P1_attacked_ship.y == m_P1_attack_place.second + m_P1_attacked_ship.size/2){
                            m_P1_attacked_ship.state = Ship::State::Sink;
                        }
                        else{
                            m_P1_attacked_ship.state = Ship::State::Hit;
                        }
                    }
                    else{
                        m_P2Board[m_P1_attack_place.first][m_P1_attack_place.second] = Board::State::Empty;
                    }
                    if(checkGameover()){
                        std::cout<<"Player 1 defeated Player 2"<<std::endl;
                        return ;
                    }
                }
                //----------------------------moveship----------------------------
                //hint : queryHowToMoveShip

                //=========================Player2 attack=========================
                //-----------------------------update-----------------------------
                //hint : callbackReportEnemy
                call(&AIInterface::callbackReportEnemy, m_P2, m_P2_callbackReportEnemy);
                m_P2_callbackReportEnemy.clear();
                //-----------------------------attack-----------------------------
                //hint : queryWhereToHit
                //hint : callbackReportHit
                int m_P2_can_attack = 0;
                bool m_P2_report_hit = false;
                Ship m_P2_attacked_ship;
                std::pair<int, int> m_P2_attack_place;

                std::cout<<"player2's turn"<<std::endl;
                
                for(auto i : m_P2Ship){
                    if(i.state != Ship::State::Sink)
                        m_P2_can_attack++;
                }

                for(int i=0; i<m_P2_can_attack; i++){
                    m_P2_attack_place = call(&AIInterface::queryWhereToHit, m_P2, m_P1Board);
                    if(m_P1Board[m_P2_attack_place.first][m_P2_attack_place.second] != Board::State::Unknown){
                        std::cout<<"Player 2 AI choose a place not Unknown"<<std::endl;
                        i--;
                        continue;
                    }
                    m_P2_callbackReportEnemy.push_back(m_P2_attack_place);
                    for(auto s : m_P1Ship){
                        if(s.x <= m_P2_attack_place.first && m_P2_attack_place.first < s.x + s.size &&
                            s.y <= m_P2_attack_place.second && m_P2_attack_place.second < s.y + s.size){
                            m_P2_report_hit = true;
                            m_P2_attacked_ship = s;
                            break;
                        }
                    }
                    call(&AIInterface::callbackReportHit, m_P2, m_P2_report_hit);
                    if(m_P2_report_hit){
                        m_P1Board[m_P2_attack_place.first][m_P2_attack_place.second] = Board::State::Hit;
                        if(m_P2_attacked_ship.x == m_P2_attack_place.first + m_P2_attacked_ship.size/2 && 
                            m_P2_attacked_ship.y == m_P2_attack_place.second + m_P2_attacked_ship.size/2){
                            m_P2_attacked_ship.state = Ship::State::Sink;
                        }
                        else{
                            m_P2_attacked_ship.state = Ship::State::Hit;
                        }
                    }
                    else{
                        m_P1Board[m_P2_attack_place.first][m_P2_attack_place.second] = Board::State::Empty;
                    }
                    if(checkGameover()){
                        std::cout<<"Player 2 defeated Player 1"<<std::endl;
                        return ;
                    }
                }
                //----------------------------moveship----------------------------
                //hint : queryHowToMoveShip

            }
            return ;
        } 

   private:
        void updateGuiGame()
        {
            gui->updateGame(m_P1Board, m_P1Ship, m_P2Board, m_P2Ship);
        }

        bool checkGameover()
        {
            bool flag = true;

            for( Ship s : m_P1Ship )
                if( s.state != Ship::State::Sink )
                    flag = false;
            if( flag )
            {
                putToGui("P2 win!\n");
                return true;
            }
            for( Ship s : m_P2Ship )
                if( s.state != Ship::State::Sink )
                    return false;
            putToGui("P1 win!\n");
            return true;
        }

        bool prepareState()
        {
            std::vector<Ship> initPos;

            putToGui("P1 Prepareing...\n");
            initPos = call(&AIInterface::init, m_P1, m_size, m_ship_size, true, m_runtime_limit);
            if( !checkShipPosition(initPos) )
            {
                putToGui("P1 Init() Invaild...\n");
                return false;
            }
            for( auto &ship : initPos )
                ship.state = Ship::State::Available;
            m_P1Ship = initPos;
            putToGui("Done.\n");

            initPos.clear();

            putToGui("P2 Prepareing...\n");
            initPos = call(&AIInterface::init, m_P2, m_size, m_ship_size, false, m_runtime_limit);
            if( !checkShipPosition(initPos) )
            {
                putToGui("P2 Init() Invaild...\n");
                return false;
            }
            for( auto &ship : initPos )
                ship.state = Ship::State::Available;
            m_P2Ship = initPos;
            putToGui("Done.\n");
            return true;
        }
        bool moveshipState(int who)
        {
            std::vector<Ship> newPos;
            if (who==1) {
                newPos = call(&AIInterface::init, m_P1,m_P1Ship, m_runtime_limit);
            }
                
            else if (who==2) {
                newPos = call(&AIInterface::init, m_P2,m_P2Ship, m_runtime_limit);
            }

            //checking the position range of the ship
            if( !checkShipPosition(newPos))
            {
                putToGui("Ship move to position which is out of range");
                return false;
            }
            //checking the type of the ship
            for (auto [size, x, y, state] : newPos) {
                if (state!=Available) {
                    putToGui("Your ship isn't Available");
                    return false;
                }
            }

            //checking whether the position is on Hit 
            if (who==1) {
                for(auto [size, x, y, state] : newPos) {
                    for (int i=-((size-1)/2); i<=((size-1)/2); i++) {
                        for (int j=-1; j<1; j++) {
                            if (m_P1Board[x+i][y+j]==Hit) {
                                putToGui("Your ship is on Hit place");
                                return false;
                            }
                        }
                    }
                }
            }

            if (who==2) {
                for(auto [size, x, y, state] : newPos) {
                    for (int i=-((size-1)/2); i<=((size-1)/2); i++) {
                        for (int j=-((size-1)/2); j<((size-1)/2); j++) {
                            if (m_P2Board[x+i][y+j]==Hit) {
                                putToGui("Your ship is on Hit place");
                                return false;
                            }
                        }
                    }
                }
            }


            if (who==1) m_P1Ship = newPos;
            else if (who==2) m_P2Ship = newPos;
            return true;

        }
        template<typename Func ,typename... Args, 
            std::enable_if_t< std::is_void<
                    std::invoke_result_t<Func, AIInterface, Args...>
                >::value , int> = 0 >
        void call(Func func, AIInterface *ptr, Args... args)
        {
            std::future_status status;
            auto val = std::async(std::launch::async, func, ptr, args...);
            status = val.wait_for(std::chrono::milliseconds(m_runtime_limit));

            if( status != std::future_status::ready )
            {
                exit(-1);
            }
            val.get();
        }

        template<typename Func ,typename... Args, 
            std::enable_if_t< std::is_void<
                    std::invoke_result_t<Func, AIInterface, Args...>
                >::value == false, int> = 0 >
        auto call(Func func, AIInterface *ptr, Args... args)
            -> std::invoke_result_t<Func, AIInterface, Args...>
        {
            std::future_status status;
            auto val = std::async(std::launch::async, func, ptr, args...);
            status = val.wait_for(std::chrono::milliseconds(m_runtime_limit));

            if( status != std::future_status::ready )
            {
                exit(-1);
            }
            return val.get();
        }

        void putToGui(const char *fmt, ...)
        {
            va_list args1;
            va_start(args1, fmt);
            va_list args2;
            va_copy(args2, args1);
            std::vector<char> buf(1+std::vsnprintf(nullptr, 0, fmt, args1));
            va_end(args1);
            std::vsnprintf(buf.data(), buf.size(), fmt, args2);
            va_end(args2);

            if( buf.back() == 0 ) buf.pop_back();
            gui->appendText( std::string(buf.begin(), buf.end()) );
        }

        bool checkAI(AIInterface *ptr) 
        {
            return ptr->abi() == AI_ABI_VER;
        }

        bool checkShipPosition(std::vector<Ship> ships)
        {
            
            if( ships.size() != m_ship_size.size() )
            {
                putToGui("Ship number not match : real %zu ,expect %zu,\n",ships.size(), m_ship_size.size());
                return false;
            }
            
            std::sort(ships.begin(), ships.end(), [](Ship a, Ship b){
                return a.size < b.size; 
            });

            std::vector<std::vector<int>> map(m_size, std::vector<int>(m_size));

            int id = -1;
            for( auto [size, x, y, state] : ships )
            {
                id++;
                if( size != m_ship_size[id] )
                {
                    putToGui("Ship %d size not match : real %zu ,expect %zu,\n", id, size, m_ship_size[id]);
                    return false;
                }

                for( int dx = 0 ; dx < size ; dx++ )
                    for( int dy = 0 ; dy < size ; dy++ )
                        {
                            int nx = x + dx;
                            int ny = y + dy;

                            if( nx < 0 || nx >= m_size || ny < 0 || ny >= m_size )
                            {
                                putToGui("Ship %d out of range at (%d,%d),\n", id, nx, ny);
                                return false;
                            }

                            if( map[nx][ny] != 0 )
                            {
                                putToGui("Ship %d and %d overlapping at (%d,%d),\n", id,  map[nx][ny]-1, nx, ny);
                                return false;
                            }
                            map[nx][ny] = id + 1;
                        }
            }
            return true;
        }

        int m_size;
        std::vector<int> m_ship_size;
        std::chrono::milliseconds m_runtime_limit;

        AIInterface *m_P1;
        AIInterface *m_P2;
        GUIInterface *gui;

        std::vector<Ship> m_P1Ship;
        std::vector<Ship> m_P2Ship;
        Board m_P1Board;
        Board m_P2Board;

        std::vector<std::pair<int, int>> m_P1_callbackReportEnemy;
        std::vector<std::pair<int, int>> m_P2_callbackReportEnemy;
        /*
        if P2 fight P1 at place(x, y)
        then push back (x, y) into m_P1_callbackReportEnemy
        */
    } ;
}
