#include <ftxui/dom/elements.hpp>  // for text, center, separator, operator|, flex, Element, vbox, Fit, hbox, border
#include <ftxui/screen/screen.hpp>  // for Full, Screen
#include <ftxui/component/screen_interactive.hpp>
#include <memory>                   // for allocator

#include "ftxui/dom/node.hpp"      // for Render
#include "ftxui/screen/color.hpp"  // for ftxui

#include <chrono>
#include <thread>


int main() {
    using namespace ftxui;
    
    auto make_position = [](std::string position) {
        return text(position) | borderEmpty | size(WIDTH, EQUAL, 4) | size(HEIGHT, EQUAL, 2);
    };

    auto make_card = [](std::wstring card) {
        return text(card) | borderDouble | size(WIDTH, EQUAL, 5) | size(HEIGHT, EQUAL, 2);
    };

    auto make_faces = []() {
        // TODO
        return vbox({
            text(L"QQK"),
            text(L"♠♠♠")
        }) | borderEmpty | size(WIDTH, EQUAL, 5) | size(HEIGHT, EQUAL, 2);
    };

    auto make_caravan_card = [&](std::string position, std::wstring card) {
        return hbox({
            make_position(position),
            make_card(card),
            make_faces(),
        }) | hcenter | size(HEIGHT, EQUAL, 4);
    };

    auto make_player_card = [&](std::string position, std::wstring card) {
        return hbox({
            make_position(position),
            make_card(card),
            separatorEmpty() | borderEmpty | size(WIDTH, EQUAL, 4)
        }) | hcenter | size(HEIGHT, EQUAL, 4);
    };
    
    auto make_caravan_p1 = [&](std::string title) {
        return window(
            text(title) | hcenter | bold,
            vbox({
                make_caravan_card("1", L" K♠"),
                make_caravan_card("2", L" K♠"),
                make_caravan_card("3", L" K♠"),
                make_caravan_card("4", L" K♠"),
                make_caravan_card("5", L" K♠"),
                make_caravan_card("6", L" K♠"),
                make_caravan_card("7", L" K♠"),
                make_caravan_card("8", L" K♠"),
            })
        ) | size(HEIGHT, EQUAL, 32);
    };

    auto make_caravan_p2 = [&](std::string title) {
        return window(
            text(title) | hcenter | bold,
            vbox({
                make_caravan_card("8", L" K♠"),
                make_caravan_card("7", L" K♠"),
                make_caravan_card("6", L" K♠"),
                make_caravan_card("5", L" K♠"),
                make_caravan_card("4", L" K♠"),
                make_caravan_card("3", L" K♠"),
                make_caravan_card("2", L" K♠"),
                make_caravan_card("1", L" K♠"),
            })
        ) | size(HEIGHT, EQUAL, 32);
    };

    auto make_player_p1 = [&](std::string title) {
        return window(
            text(title) | hcenter | bold,
            vbox({
                make_player_card("1", L" K♠"),
                make_player_card("2", L" K♠"),
                make_player_card("3", L" K♠"),
                make_player_card("4", L" K♠"),
                make_player_card("5", L" K♠"),
                make_player_card("6", L" K♠"),
                make_player_card("7", L" K♠"),
                make_player_card("8", L" K♠"),
            })
        ) | size(WIDTH, EQUAL, 17) | size(HEIGHT, EQUAL, 32);
    };

    auto make_player_p2 = [&](std::string title) {
        return window(
            text(title) | hcenter | bold,
            vbox({
                make_player_card(" ", L"###"),
                make_player_card(" ", L"###"),
                make_player_card(" ", L"###"),
                make_player_card(" ", L"###"),
                make_player_card(" ", L"###"),
                make_player_card(" ", L"###"),
                make_player_card(" ", L"###"),
                make_player_card(" ", L"###"),
            })
        ) | size(WIDTH, EQUAL, 17) | size(HEIGHT, EQUAL, 32);
    };

    auto document = hbox({  // outermost box
    
        vbox({  // game area
        
            hbox({  // opp game area
                
                separatorEmpty(),
                make_caravan_p2(" D (100, ASC) "),
                separatorEmpty(),
                make_caravan_p2(" E "),
                separatorEmpty(),
                make_caravan_p2(" F "),
                separatorEmpty(),

            }) | size(HEIGHT, EQUAL, 35),

            hbox({  // you game area
                
                separatorEmpty(),
                make_caravan_p1(" A "),
                separatorEmpty(),
                make_caravan_p1(" B "),
                separatorEmpty(),
                make_caravan_p1(" C "),
                separatorEmpty(),

            }) | size(HEIGHT, EQUAL, 35),
        
        }) | size(HEIGHT, EQUAL, 67),

        separatorEmpty(),
        separatorEmpty(),
        separatorEmpty(),

        vbox({  // deck area
        
            hbox({  // opp deck area
                
                separatorEmpty(),
                make_player_p2(" OPP "),
                separatorEmpty(),

            }) | size(HEIGHT, EQUAL, 35),

            hbox({  // you deck area
                
                separatorEmpty(),
                make_player_p1(" YOU "),
                separatorEmpty(),

            }) | size(HEIGHT, EQUAL, 35),
        
        }) | size(HEIGHT, EQUAL, 67),
        /*
        separatorEmpty(),
        separatorEmpty(),
        separatorEmpty(),

        window(  // input area
            text(" INPUT ") | hcenter | bold,
            vbox({
                text("YOU played QC on F7, changing F's direction"),
                text("OPP played JS on A5, removing 3D"),

                separatorEmpty(),

                text("YOU > "),
            }) | borderEmpty
        ) | size(WIDTH, EQUAL, 60) | vcenter,  // add height
        */
    
    }) | border | hcenter | size(HEIGHT, EQUAL, 70);

    auto terminal_size = Terminal::Size();
    auto document_width = 76;
    auto document_height = 70;

    if (terminal_size.dimx < document_width) {
        printf("Terminal: x=%d y=%d\n", terminal_size.dimx, terminal_size.dimy);
        printf("Document: x=%d y=%d\n", document_width, document_height);
        printf("Terminal too small.\n");
        exit(1);
    }

    auto screen = Screen::Create(Dimension::Fixed(document_width), Dimension::Fixed(document_height));
    screen.Clear();

    Render(screen, document);
    screen.Print();
    
    std::this_thread::sleep_for(std::chrono::milliseconds(1000 * 3));

    screen.Clear();

    printf("Terminal: x=%d y=%d\n", terminal_size.dimx, terminal_size.dimy);
    printf("Document: x=%d y=%d\n", document_width, document_height);

    return 0;
}

