
#include <memory>

#include "autoteller/autoteller.hpp"


int main(){
    std::shared_ptr<Creator> inventory_t_ptr = std::make_shared<Inventory_t>();
    auto inventory_ptr = inventory_t_ptr->get_instance();
    inventory_ptr->read("data/inventory_copy.json");
    inventory_ptr->display("Inventory");
    // std::cout << std::setw(4) << inventory_ptr->get() <<"\n\n";

    std::shared_ptr<Creator> menu_t_ptr = std::make_shared<Menu_t>();
    auto menu_ptr = menu_t_ptr->get_instance();
    menu_ptr->read("data/menu_copy.json");
    menu_ptr->display("Horses");
    // std::cout << std::setw(4)<< menu_ptr->get() << "\n\n";

    std::shared_ptr<Creator> autoteller_t_ptr = std::make_shared<Autoteller_t>();
    auto autoteller_ptr = autoteller_t_ptr->get_instance();
    autoteller_ptr->store(inventory_ptr);
    autoteller_ptr->store(menu_ptr);


    for(int i=0; i<4; ++i){
        std::string first, second;
        std::cin >> first >> second;
        autoteller_ptr->store(std::pair<std::string, std::string>(first, second));
    }

    autoteller_ptr->bet();
    

    



}