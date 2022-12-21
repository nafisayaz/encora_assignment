
#pragma once

#include <iostream>
#include <utility>
#include <fstream>
#include <vector>
#include <memory>
#include <string>
#include <nlohmann/json.hpp>

#include <boost/algorithm/string.hpp>
#include <boost/lexical_cast.hpp>

using json = nlohmann::json;



class Teller{
    public:
        Teller();
        virtual ~Teller();
        void store(std::pair<std::string, std::string>input );
        void store(std::shared_ptr<Teller> S_ptr );
        auto validityFirst(std::string frist);
        auto validitySecond(std::string second);
        auto validity(std::pair<std::string, std::string> input);
        void dispense(int payout_amount);
        void bet();
        void payout(std::string name, int payout_amount);

        virtual json get()const=0;
        virtual void modify(json dispenseData){}

        virtual void push_back(std::shared_ptr<Teller> S_ptr);
        virtual void set(const std::string filename){};
        virtual void read(const std::string filename){};
        virtual void display(const char* msg){}
        virtual void display(const char* msg, json data){};



    
    private:
        std::vector<std::shared_ptr<Teller>>     m_Items;
        std::vector<std::pair<std::string, std::string>>    m_Inputs;

};


class Autoteller : public Teller{
    public:
        Autoteller();
        ~Autoteller();
        virtual json get()const override{}
        void display(const char* msg, json data)override{}
        void display(const char* msg)override;
        void push_back(std::shared_ptr<Teller> S_ptr)override;
        

    private:
        std::vector<std::shared_ptr<Teller>>     m_Items;


};

class Menu : public Teller{
    public:
        Menu();
        ~Menu();
        json get()const override;
        void display(const char* msg)override;
        void push_back(std::shared_ptr<Teller> S_ptr)override;
        void set(const std::string filename)override;
        void read(const std::string filename)override;
        

    private:
        std::shared_ptr<Teller>         m_Items;
        std::string                     m_filename;
        json                            m_menu;

};


class Inventory : public Teller{
    public:
        Inventory();
        ~Inventory();
        json get()const override;
        void display(const char* msg)override;
        void display(const char* msg, json data)override;
        void modify(json dispenseData)override;

        void push_back(std::shared_ptr<Teller> S_ptr)override;
        void set(const std::string filename)override;
        void read(const std::string filename)override;

    private:
        std::shared_ptr<Teller>         m_Items;
        std::string                     m_filename;
        json                            m_inventory;


};


class Creator{
    public:
        Creator(){}
        ~Creator(){}
        virtual std::shared_ptr<Teller> get_instance()=0;


};


class Autoteller_t : public Creator{
    public:
        Autoteller_t(){}
        ~Autoteller_t(){}
        std::shared_ptr<Teller> get_instance()override;

};


class Menu_t : public Creator{
    public:
        Menu_t(){}
        ~Menu_t(){}
        std::shared_ptr<Teller> get_instance()override;

};


class Inventory_t : public Creator{
    public:
        Inventory_t(){}
        ~Inventory_t(){}
        std::shared_ptr<Teller> get_instance()override;


};




