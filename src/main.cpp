#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>
#include <algorithm>
#include <wex.h>
#include "cStarterGUI.h"

#include "cGraph.h"

class cGUI : public cStarterGUI
{
public:
    cGUI()
        : cStarterGUI(
              "HiMan",
              {50, 50, 500, 400}),
          graphPanel(wex::maker::make<wex::panel>(fm))
    {
        myG.addLink("a1", "b1");
        myG.addLink("a1", "b2");
        myG.addLink("b1", "c1");
        myG.addLink("b2", "c1");
        myG.addLink("b2", "c2");
        myG.findoradd("a2");
        myG.findoradd("b3");

        graphPanel.move(20, 20, 350, 750);

        graphPanel.events().draw(
            [&](PAINTSTRUCT &ps)
            {
                draw(ps);
            });

        show();
        run();
    }

private:
    wex::panel &graphPanel;
    raven::graph::cGraph myG;

    void draw(PAINTSTRUCT &ps);
};

void cGUI::draw(PAINTSTRUCT &ps)
{
    const int dist = 100; // distance between nodes
    const int sz = 20;    // size of nodes

    // store node locations, keyed by node index
    std::map<int, std::pair<int, int>> mploc;

    // locate first node
    int x = 20;
    int y = 20;

    // loop over levels
    std::vector<std::string> vprefix{"a", "b", "c"};
    for (auto prefix : vprefix)
    {
        // loop over nodes
        for (auto &n : myG.nodes())
        {
            // is node in this level?
            if (n.second.myName.substr(0, 1) == prefix)
            {
                // store node location
                mploc.insert(std::make_pair(
                    n.first,
                    std::make_pair(x, y)));
                
                // move down for next node at this level
                y += dist;
            }
        }

        // location of first node in next level
        x += dist;
        y = 20;
    }

    // draw lines between connected nodes
    wex::shapes S(ps);
    for (auto &n : myG.nodes())
    {
        for (auto &e : n.second.myLink)
        {
            // std::cout << n.first <<" "<< e.first << "\n";
            S.line({mploc[n.first].first,
                    mploc[n.first].second,
                    mploc[e.first].first,
                    mploc[e.first].second});
        }
    }

    // draw nodes
    S.bgcolor(0xFFFFFF);
    for (auto &n : myG.nodes())
    {
        S.circle(mploc[n.first].first,
                 mploc[n.first].second,
                 sz);
        S.text(
            n.second.myName,
            {mploc[n.first].first - 10,
             mploc[n.first].second - 10});
    }
}

main()
{
    cGUI theGUI;
    return 0;
}
