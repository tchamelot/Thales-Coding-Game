#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <cmath>

using namespace std;

class Flag
{
public:
    Flag()
    {
        m_team = "none";
    }
    
    ~Flag()
    {
        
    }
    
    void set_info()
    {
        cin >> m_x >> m_y;
        cin.ignore();
        if(m_team == "none")
            if(m_x <= 1000)
                m_team = "orange";
            else
                m_team = "purple";
        
        if(m_x ==-1 && m_y == -1)
            m_in_base = false;
        else
            m_in_base = true;
    }
    
    int get_x()
    {
        return m_x;
    }
    
    int get_y()
    {
        return m_y;
    }
    
    string get_team()
    {
        return m_team;
    }
    
    bool is_in_base()
    {
        return m_in_base;
    }
    
private:
    int m_x;
    int m_y;
    string m_team;
    
    bool m_in_base;
};

class Ovni
{
public:
    Ovni()
    {
        m_team = "none";
    }
    
    ~Ovni()
    {
        
    }
    
    void set_info()
    {
        cin >> m_x >> m_y >> m_vx >> m_vy >> m_flag;
        cin.ignore();
    }
    
    void set_team(string team)
    {
        if(m_team == "none")
            m_team = team;
    }
    
    int get_x()
    {
        return m_x;
    }
    
    int get_y()
    {
        return m_y;
    }
  
  	int get_vx()
    {
      return m_vx;
    }
  
  	int get_vy()
    {
      return m_vy;
    }
    
    bool has_flag()
    {
        return m_flag;
    }
    
    void set_target(int x, int y, string power)
    {
        if(x >= 0)
            m_x_target = x;
        if(y >= 0)
            m_y_target = y;
        m_power = power;
    }
    
    void command()
    {
        cout << m_x_target << ' ' << m_y_target << ' ' << m_power << endl;
    }
    
    float distance_from(int x, int y)
    {
        return sqrt(pow(m_x - x, 2) + pow(m_y - y, 2));
    }
    
    float distance_from(Ovni ovni)
    {
        return sqrt(pow(m_x - ovni.get_x(), 2) + pow(m_y - ovni.get_y(), 2));
    }
    
    float distance_from(Flag flag)
    {
        return sqrt(pow(m_x - flag.get_x(), 2) + pow(m_y - flag.get_y(), 2));
    }
    
    void regulate_speed()
    {
        int speed;
        float d = this->distance_from(m_x_target, m_y_target);
        if(d < 5000)
        {
            speed = d/50;
        }
        
        if(speed < 100 && speed > 50)
        {
            m_power = to_string(speed);
        }
        else
        {
            if(speed > 100)
                m_power = "100";
            if(speed < 50)
                m_power = "50";
        }
        
    }
    
    void return_flag()
    {
        int x_target;
        int y_target;
        float theta;
        
        if(m_team == "orange")
            x_target = 0;
        else
            x_target = 10000;
            
        theta = atan2(m_vx, m_vy);
        y_target = m_y - (m_x - x_target)/tan(theta);
        
        if(y_target > 8000)
            y_target = 8000;
            
        if(y_target < 0)
            y_target = 0;
        
        this->set_target(x_target, y_target, "100");
    }
    
    void attack_flag(Flag ally_flag, Flag enemy_flag, Ovni enemy_1, Ovni enemy_2)
    {
        // Check if it has to defend
        if(!ally_flag.is_in_base())
        {
            // If it is closer to the enemy target the enemy else target the flag
            if(enemy_1.has_flag())
            {
                if(this->distance_from(enemy_1) < this->distance_from(enemy_flag))
                    this->set_target(enemy_1.get_x(), enemy_1.get_y(), "BOOST");
                else
                    this->set_target(enemy_flag.get_x(), enemy_flag.get_y(), "BOOST");
            }
            else
            {
               if(this->distance_from(enemy_2) < this->distance_from(enemy_flag))
                    this->set_target(enemy_2.get_x(), enemy_2.get_y(), "BOOST");
                else
                    this->set_target(enemy_flag.get_x(), enemy_flag.get_y(), "BOOST"); 
            }
        }
        // Atack
        else
        {
            if(this->distance_from(enemy_flag) > 5000)
                this->set_target(enemy_flag.get_x(), enemy_flag.get_y(), "BOOST");
            else
                this->set_target(enemy_flag.get_x(), enemy_flag.get_y(), "100");
        }
    }
    
    void defend_flag(Flag ally_flag, Flag enemy_flag, Ovni enemy_1, Ovni enemy_2)
    {
        if(!ally_flag.is_in_base())
        {
            if(enemy_1.has_flag())
            {
                this->set_target(enemy_1.get_x() + enemy_1.get_vx()*3, enemy_1.get_y() + enemy_1.get_vy()*3, "BOOST");
            }
            else
            {
                this->set_target(enemy_2.get_x() + enemy_2.get_vx()*3, enemy_2.get_y() + enemy_2.get_vy()*3, "BOOST");
            }
        }
        else
        {
            int x_target;
            int y_target;
            
            if(enemy_1.distance_from(ally_flag) < enemy_2.distance_from(ally_flag))
            {
                x_target = (ally_flag.get_x() + enemy_1.get_x())/2;
                y_target = (ally_flag.get_y() + enemy_1.get_y())/2;
            }
            else
            {
                x_target = (ally_flag.get_x() + enemy_2.get_x())/2;
                y_target = (ally_flag.get_y() + enemy_2.get_y())/2;
            }
            
            if(m_team == "orange")
            {
                if(x_target > ally_flag.get_x() + 500)
                {
                   x_target = ally_flag.get_x() + 500;
                }   
            }
            else
            {
                if(x_target < ally_flag.get_x() - 500)
                {
                    x_target = ally_flag.get_x() - 500;
                }   
            }
            
            this->set_target(x_target, y_target, "100");
            
            this->regulate_speed();
        }
    }
    
    void debug()
    {
        cerr << "x: " << m_x << ", y: " << m_y << ", vx: " << m_vx << ", vy: " << m_vy << endl;
    }
    
private:
 int m_x;
 int m_y;
 int m_vx;
 int m_vy;
 bool m_flag;
 
 string m_team;
 
 int m_x_target;
 int m_y_target;
 string m_power;
    
};

/**
 * Auto-generated code below aims at helping you parse
 * the standard input according to the problem statement.
 **/
int main()
{
    Ovni ally_1, ally_2, enemy_1, enemy_2;
    
    Flag ally_flag, enemy_flag;
    
    // game loop
    while (1) {
        // Get the data
        
        // Ally flag
        ally_flag.set_info();
        
        // Enemy flag
        enemy_flag.set_info();
        
        // Ally 1
        ally_1.set_info();
        ally_1.set_team(ally_flag.get_team());
        
        // Ally 2
        ally_2.set_info();
        ally_2.set_team(ally_flag.get_team());
        
        // Enemy 1
        enemy_1.set_info();
        enemy_1.set_team(enemy_flag.get_team());
        
        // Enemy 2
        enemy_2.set_info();
        enemy_2.set_team(enemy_flag.get_team());
        
// Process game

        if(enemy_flag.is_in_base())
        {
            if(ally_1.distance_from(enemy_flag) < ally_2.distance_from(enemy_flag))
            {
                ally_1.attack_flag(ally_flag, enemy_flag, enemy_1, enemy_2);
                ally_2.defend_flag(ally_flag, enemy_flag, enemy_1, enemy_2);
            }
            else
            {
                ally_1.defend_flag(ally_flag, enemy_flag, enemy_1, enemy_2);
                ally_2.attack_flag(ally_flag, enemy_flag, enemy_1, enemy_2);
            }
        }
        else
        {
            if(ally_1.has_flag())
            {
                ally_1.return_flag();
                ally_2.defend_flag(ally_flag, enemy_flag, enemy_1, enemy_2);
            }
            else
            {
                ally_1.defend_flag(ally_flag, enemy_flag, enemy_1, enemy_2);
                ally_2.return_flag();
            }
        }
                
        ally_1.debug();
        
        // Ouptut command
        // Command ovni 1
        ally_1.command();
        
        // Command ovni 2
        ally_2.command();
    }
}
