#include <nall/platform.hpp>
#include <nall/http/server.hpp>
#include <nall/http/response.hpp>
#include <nall/string.hpp>
#include <nall/vector.hpp>

#include <string>
#include <iostream>
#include <sstream>

#include <NLTemplate/NLTemplate.h>

// FIXME: lock state based on cookie instead of not doing anything at all

using namespace nall;
using namespace nall::BML;
using namespace nall::HTTP;
using namespace NL::Template;

struct asdf
{
    Markup::Node mystate;
    Markup::Node mydeck;
    vector<intmax_t> UIDs;
    intmax_t UIDindex = -1;
    asdf()
    {
        mystate = unserialize(file::read("state.txt"));
        mydeck = unserialize(file::read(mystate["deck"].text().data()));
        // mystate["UID"].integer()
        for(auto node : mydeck)
        {
            if(node.name() == "card")
            {
                if(node["UID"].integer() == mystate["UID"].integer())
                    UIDindex = UIDs.size();
                UIDs.append(node["UID"].integer());
            }
        }
    }
} state;

Response monicathread (Request& request)
{
    LoaderFile loader;
    Template main(loader);
    
    main.load(state.mydeck["meta"]["template"].value().data());
    main.set("deckname", state.mydeck["meta"]["name"].value().data());
    
    if(request.post("docard") == state.UIDs[state.UIDindex])
    {
        state.UIDindex = (state.UIDindex+1)%state.UIDs.size();
    }
    
    for(auto node : state.mydeck)
    {
        if(node.name() == "card")
        {
            if(node["UID"].integer() == state.UIDs[state.UIDindex])
            {
                for(auto key : node)
                    main.set(key.name().data(), key.value().data());
            }
        }
    }
    
    std::stringstream scratch;
    main.render(scratch);
    Response response(request);
    response.setText(scratch.str().data()).setResponseType(200);
    return response;
}

#include <nall/main.hpp>
void nall::main(lstring arguments)
{
    Server monicaserver;
    monicaserver.main(monicathread);
    
    if(monicaserver.open())
    {    
        std::cout<<"running\n";
        
        while(1)
        {
            usleep(10000);
            monicaserver.scan();
        }
    }
    else
        std::cout<<"apparent failure";
    
}
