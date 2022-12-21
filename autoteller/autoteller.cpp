

#include "autoteller.hpp"

/** TELLER*/
Teller::Teller(){}
Teller::~Teller(){}

void Teller::store(std::pair<std::string, std::string>input){
    m_Inputs.push_back(input);
}

void Teller::store(std::shared_ptr<Teller> S_ptr){
    m_Items.push_back(std::move(S_ptr));
}

void Teller::push_back(std::shared_ptr<Teller> S_ptr){
    //m_Items.push_back(S_ptr);
    std::cout << "Teller::push_back\n";

}

auto Teller::validityFirst(std::string first){
    if(first != "w"){
        boost::lexical_cast<int>(first);
        return true;
    }else if(first == "w"){
        return true;
    }
    return false;
}


auto Teller::validitySecond(std::string second){
    boost::lexical_cast<int>(second);
    return true;
}

auto Teller::validity(std::pair<std::string, std::string> input){
    try{
        Teller::validityFirst(input.first);
    }catch(std::exception ex){
        std::cout << " Invalid Horse Number : "<< input.first << " \n\n";
        return false;
    }

    try{
        Teller::validitySecond(input.second);
    }catch(std::exception ex){
        std::cout << " Invalid Bet : "<< input.second << " \n\n";
        return false;
    }
    return true;
}


void Teller::dispense(int payout_amount){
    json dispense;
    dispense["1"]["deno"] = 1;
    dispense["1"]["inven"] = 0;

    dispense["2"]["deno"] = 5;
    dispense["2"]["inven"] = 0;

    dispense["3"]["deno"] = 10;
    dispense["3"]["inven"] = 0;

    dispense["4"]["deno"] = 20;
    dispense["4"]["inven"] = 0;

    dispense["5"]["deno"] = 100;
    dispense["5"]["inven"] = 0;

    while(payout_amount){
        if(payout_amount >= 100){
            dispense["5"]["inven"] = payout_amount/100;
            payout_amount = payout_amount%100;
        }else if( payout_amount >= 20){
            dispense["4"]["inven"] = payout_amount/20;
            payout_amount = payout_amount%20;
        }else if(payout_amount >= 10){
            dispense["3"]["inven"] = payout_amount/10;
            payout_amount = payout_amount%10;
        }else if(payout_amount >= 5){
            dispense["2"]["inven"] = payout_amount/5;
            payout_amount = payout_amount%5;
        }else if(payout_amount >= 1){
            dispense["1"]["inven"] = payout_amount/1;
            payout_amount = payout_amount%1;
        }else{
            break;
        }
    }
    m_Items[0]->display("Dispensing", dispense);
    m_Items[0]->modify(dispense);


}


void Teller::payout(std::string name, int payout_amount){
    std::cout << "Payout: " << name << ", " << payout_amount<<"\n";
}

void Teller::bet(){
    for(auto& input: m_Inputs){
        if(Teller::validity(input) and m_Items[1]->get()[input.first]["wl"] == "won"){
            int odd =  m_Items[1]->get()[input.first]["odd"] ;
            int bet_amount = boost::lexical_cast<int>(input.second);
            int payout_amount = bet_amount* odd;
            Teller::payout(m_Items[1]->get()[input.first]["name"], payout_amount );
            Teller::dispense( payout_amount);

        }else{
            std::cout << "No Payout: " << m_Items[1]->get()[input.first]["name"] <<"\n";
        }
        m_Items[0]->display("Inventory");
        m_Items[1]->display("Horses");

    }


    // std::cout <<"\n" << m_Items[1]->get()["1"]["wl"] <<"\n\n";
    // std::cout <<"\n" << m_Items[1]->get()["1"]["odd"] <<"\n\n";
}



/** AUTOTELLER*/
Autoteller::Autoteller(){}
Autoteller::~Autoteller(){}



void Autoteller::push_back(std::shared_ptr<Teller> S_ptr){
    m_Items.push_back(S_ptr);
}
void Autoteller::display(const char* msg){
    std::cout << "Autoteller::display\n";
    std::cout <<" \n\nm_Items[0].get(): " << m_Items[0].get();
    std::cout <<" \n\nm_Items[1].get(): " << m_Items[1].get();


}


/** MENU*/
Menu::Menu(){}
Menu::~Menu(){}

json Menu::get()const{
    // std::cout << "Menu::get()const\n";
    return m_menu;
}

void Menu::push_back(std::shared_ptr<Teller> S_ptr){
    m_Items = std::move(S_ptr);
    std::cout << "Menu::push_back\n";
}

void Menu::set(const std::string filename){
    m_filename = filename;
    std::cout << "Menu::set\n";
}

void Menu::read(const std::string filename){
    std::ifstream fs(filename);
    m_menu = json::parse(fs);
    // std::cout << "Menu::read\n";

}

void Menu::display(const char* msg){
    std::cout <<msg << ": \n";
    for(auto& [key, value] : m_menu.items()){
        std::cout << key << ", ";
        for(auto& [k, val]: value.items()){
            if( k == "name"){
                std::cout << static_cast<std::string>(val) <<", ";
            }else if( k == "wl"){
                std::cout << static_cast<std::string>(val) <<" ";
            }
            else{
                std::cout << val<<", ";
            }    
        }  
        std::cout << "\n";    
    }
}


/** INVENTORY*/

Inventory::Inventory(){}
Inventory::~Inventory(){}

json Inventory::get()const{
    return m_inventory;
}

void Inventory::modify(json dispenseData){
    for (json::iterator it_d = dispenseData.begin(), it_mi = m_inventory.begin() ; it_d != dispenseData.end(); ++it_d, ++it_mi) {
        it_mi.value()["inven"] = it_mi.value()["inven"].get<int>() - it_d.value()["inven"].get<int>(); 
    }
}

void Inventory::push_back(std::shared_ptr<Teller> S_ptr){
    m_Items = std::move(S_ptr);
    std::cout << "Inventory::push_back\n";
}

void Inventory::set(const std::string filename){
    //m_Items = std::move(S_ptr);
    std::cout << "Inventory::set\n";
}

void Inventory::read(const std::string filename){
    std::ifstream fs(filename);
    m_inventory = json::parse(fs);
    // std::cout << "\nInventory::read\n";

}

void Inventory::display(const char* msg){
    std::cout <<msg << ": \n";
    for(auto& [key, value] : m_inventory.items()){
        // std::cout << key << " : " << value;
        for(auto& [k, val]: value.items()){
            if( k == "deno")
                std::cout << "$"<< val <<", ";
            else{
                std::cout << val<<" ";
            }    
        }  
        std::cout << "\n";    
    }
}

void Inventory::display(const char* msg, json data){
    std::cout <<msg << ": \n";
    for(auto& [key, value] : data.items()){
        // std::cout << key << " : " << value;
        for(auto& [k, val]: value.items()){
            if( k == "deno")
                std::cout << "$"<< val <<", ";
            else{
                std::cout << val<<" ";
            }    
        }  
        std::cout << "\n";    
    }
}


std::shared_ptr<Teller> Autoteller_t::get_instance(){
    return std::make_shared<Autoteller>();
}

std::shared_ptr<Teller> Menu_t::get_instance(){
    return std::make_shared<Menu>();
}

std::shared_ptr<Teller> Inventory_t::get_instance(){
    return std::make_shared<Inventory>();
}

